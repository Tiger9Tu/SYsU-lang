#include "optimizer.hh"

#include <llvm/Passes/PassBuilder.h>

llvm::PreservedAnalyses
sysu::StaticCallCounterPrinter::run(llvm::Module &M,
                                    llvm::ModuleAnalysisManager &MAM)
{
  auto DirectCalls = MAM.getResult<sysu::StaticCallCounter>(M);

  OS << "=================================================\n";
  OS << "sysu-optimizer: static analysis results\n";
  OS << "=================================================\n";
  const char *str1 = "NAME", *str2 = "#N DIRECT CALLS";
  OS << llvm::format("%-20s %-10s\n", str1, str2);
  OS << "-------------------------------------------------\n";

  for (auto &CallCount : DirectCalls)
  {
    OS << llvm::format("%-20s %-10lu\n",
                       CallCount.first->getName().str().c_str(),
                       CallCount.second);
  }

  OS << "-------------------------------------------------\n\n";
  return llvm::PreservedAnalyses::all();
}

sysu::StaticCallCounter::Result
sysu::StaticCallCounter::run(llvm::Module &M, llvm::ModuleAnalysisManager &)
{
  llvm::MapVector<const llvm::Function *, unsigned> Res;

  for (auto &Func : M)
  {
    for (auto &BB : Func)
    {
      for (auto &Ins : BB)
      {

        // If this is a call instruction then CB will be not null.
        auto *CB = llvm::dyn_cast<llvm::CallBase>(&Ins);
        if (nullptr == CB)
        {
          continue;
        }

        // If CB is a direct function call then DirectInvoc will be not null.
        auto DirectInvoc = CB->getCalledFunction();
        if (nullptr == DirectInvoc)
        {
          continue;
        }

        // We have a direct function call - update the count for the function
        // being called.
        auto CallCount = Res.find(DirectInvoc);
        if (Res.end() == CallCount)
        {
          CallCount = Res.insert({DirectInvoc, 0}).first;
        }
        ++CallCount->second;
      }
    }
  }

  return Res;
}

llvm::AnalysisKey sysu::StaticCallCounter::Key;

//===--------------------------------------------------------------------===//
// DeadCodeElimination pass implementation
//

bool isInstructionDead(llvm::Instruction *I,
                       const llvm::TargetLibraryInfo *TLI = (const llvm::TargetLibraryInfo *)nullptr)
{
  if (llvm::isInstructionTriviallyDead(I, TLI))
  {

    if (I->hasNUsesOrMore(1))
      return false;

    switch (I->getOpcode())
    {
    case llvm::Instruction::Ret:
    // case llvm::Instruction::Alloca:
    case llvm::Instruction::Br:
    case llvm::Instruction::Switch:
    case llvm::Instruction::IndirectBr:
    case llvm::Instruction::Invoke:
    case llvm::Instruction::Unreachable:
    case llvm::Instruction::Fence:
    case llvm::Instruction::Call:
    case llvm::Instruction::Store:
    case llvm::Instruction::AtomicCmpXchg:
    case llvm::Instruction::AtomicRMW:
    case llvm::Instruction::Resume:
    case llvm::Instruction::LandingPad:
      return false;
    case llvm::Instruction::Load:
      return !llvm::dyn_cast<llvm::LoadInst>(I)->isVolatile();
    default:
      return true;
    }
  }
  return false;
}

static bool DCEInstruction(llvm::Instruction *I,
                           llvm::SmallSetVector<llvm::Instruction *, 16> &WorkList,
                           const llvm::TargetLibraryInfo *TLI)
{
  if (isInstructionDead(I, TLI))
  {
    llvm::salvageDebugInfo(*I);
    llvm::salvageKnowledge(I);

    // Null out all of the instruction's operands to see if any operand becomes
    // dead as we go.
    for (unsigned i = 0, e = I->getNumOperands(); i != e; ++i)
    {
      llvm::Value *OpV = I->getOperand(i);
      I->setOperand(i, nullptr);

      if (!OpV->use_empty() || I == OpV)
        continue;

      // If the operand is an instruction that became dead as we nulled out the
      // operand, and if it is 'trivially' dead, delete it in a future loop
      // iteration.
      if (llvm::Instruction *OpI = llvm::dyn_cast<llvm::Instruction>(OpV))
        if (llvm::isInstructionTriviallyDead(OpI, TLI))
          WorkList.insert(OpI);
    }
    I->eraseFromParent();
    //++DCEEliminated;
    return true;
  }
  return false;
}

static bool eliminateDeadCode(llvm::Function &F, llvm::TargetLibraryInfo *TLI)
{
  bool MadeChange = false;
  llvm::SmallSetVector<llvm::Instruction *, 16> WorkList;
  // Iterate over the original function, only adding insts to the worklist
  // if they actually need to be revisited. This avoids having to pre-init
  // the worklist with the entire function's worth of instructions.
  for (llvm::Instruction &I : llvm::make_early_inc_range(instructions(F)))
  {
    // We're visiting this instruction now, so make sure it's not in the
    // worklist from an earlier visit.
    if (!WorkList.count(&I))
      MadeChange |= DCEInstruction(&I, WorkList, TLI);
  }

  while (!WorkList.empty())
  {
    llvm::Instruction *I = WorkList.pop_back_val();
    MadeChange |= DCEInstruction(I, WorkList, TLI);
  }
  return MadeChange;
}

llvm::PreservedAnalyses sysu::FunctionDCE::run(llvm::Function &F, llvm::FunctionAnalysisManager &AM)
{
  if (!eliminateDeadCode(F, &AM.getResult<llvm::TargetLibraryAnalysis>(F)))
    return llvm::PreservedAnalyses::all();

  llvm::PreservedAnalyses PA;
  PA.preserveSet<llvm::CFGAnalyses>();
  return PA;
}

extern "C"
{
  llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo()
  {
    return {LLVM_PLUGIN_API_VERSION, "sysu-optimizer-pass", LLVM_VERSION_STRING,
            [](llvm::PassBuilder &PB)
            {
              // #1 REGISTRATION FOR "opt -passes=sysu-optimizer-pass"
              PB.registerPipelineParsingCallback(
                  [&](llvm::StringRef Name, llvm::ModulePassManager &MPM,
                      llvm::ArrayRef<llvm::PassBuilder::PipelineElement>)
                  {
                    if (Name == "sysu-optimizer-pass")
                    {
                      MPM.addPass(sysu::StaticCallCounterPrinter(llvm::errs()));
                      return true;
                    }
                    return false;
                  });
              // #2 REGISTRATION FOR
              // "MAM.getResult<sysu::StaticCallCounter>(Module)"
              PB.registerAnalysisRegistrationCallback(
                  [](llvm::ModuleAnalysisManager &MAM)
                  {
                    MAM.registerPass([&]
                                     { return sysu::StaticCallCounter(); });
                  });
            }};
  }
}