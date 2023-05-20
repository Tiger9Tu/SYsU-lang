#include "optimizer.hh"

#include <llvm/Passes/PassBuilder.h>

//===--------------------------------------------------------------------===//
// Function call pass implementation
//

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
/*
//===--------------------------------------------------------------------===//
// Function alive variable analysis pass implementation
//

bool isRootAliveInst(llvm::Instruction &I)
{
  return !I.isSafeToRemove() ||
         I.isTerminator() || // ||
         llvm::isa<llvm::CallInst>(I) ||
         (llvm::isa<llvm::StoreInst>(I) && llvm::isa<llvm::GlobalValue>(I.getOperand(1))); // I.mayReadOrWriteMemory();
}

bool isAliveInst(llvm::Instruction &I)
{
  if (isRootAliveInst(I))
    return true;
}

sysu::FunctionAliveVariableAnalyser::Result
sysu::FunctionAliveVariableAnalyser::run(llvm::Function &F, llvm::FunctionAnalysisManager &)
{
  llvm::SmallVector<llvm::Instruction *, 128> Res;

  llvm::SmallVector<llvm::Value *, 128> aliveValues;
  for (llvm::Instruction &I : instructions(F))
  {
    // 搜集所有位于root的活跃变量
    int opn = I.getNumOperands();
    if (isRootAliveInst(I))
    {
      for (int i = 0; i < opn; i++)
        aliveValues.push_back(I.getOperand(i));
    }
  }
}
*/
//===--------------------------------------------------------------------===//
// DeadCodeElimination pass implementation
//
/*
bool isDeadInst(llvm::Instruction &I)
{
  if (isAliveInst(I))
    return false;
  if (I.use_empty())
    return true;
  for (llvm::Use &OI : I.operands())
  {
    if (llvm::Instruction *Inst = llvm::dyn_cast<llvm::Instruction>(OI.get()))
    {
      if (!isDeadInst(*Inst))
        return false;
    }
  }
  return true;
}

bool sysu::FunctionDCE::eliminateDeadCode(llvm::Function &F)
{ */
/*
std::vector<llvm::Instruction *> deadInsts;
for (llvm::Instruction &I : llvm::instructions(F))
  if (isDeadInst(I))
    deadInsts.push_back(&I);

for (llvm::Instruction *I : deadInsts)
{
  I->dropAllReferences();
  I->eraseFromParent();
}
return !deadInsts.empty();

*/
/*
  llvm::SmallPtrSet<llvm::Instruction *, 32> Alive;
 llvm::SmallVector<llvm::Instruction *, 128> Worklist;

 for (llvm::Instruction &I : instructions(F))
 {
   if (isAliveInst(I))
   {

     Alive.insert(&I);
     Worklist.push_back(&I);
   }
 }

 while (!Worklist.empty())
 {
   llvm::Instruction *LiveInst = Worklist.pop_back_val();
   for (llvm::Use &OI : LiveInst->operands())
   {
     if (llvm::Instruction *Inst = llvm::dyn_cast<llvm::Instruction>(OI))
     {

       if (Alive.insert(Inst).second)
       {
         Worklist.push_back(Inst);
       }
     }
   }
 }

 for (llvm::Instruction &I : instructions(F))
 {
   if (!Alive.count(&I))
   {
     Worklist.push_back(&I);
     I.dropAllReferences();
   }
 }

 for (llvm::Instruction *&I : Worklist)
 {
   I->eraseFromParent();
 }
 return !Worklist.empty();
}

*/
//===--------------------------------------------------------------------===//
// DeadCodeElimination pass implementation
//

bool myIsInstructionTriviallyDead(llvm::Instruction *I, const llvm::TargetLibraryInfo *TLI)
{
  if (!I->use_empty())
    return false;
  if (I->isTerminator())
    return false;

  // We don't want the landingpad-like instructions removed by anything this
  // general.
  if (I->isEHPad())
    return false;

  // We don't want debug info removed by anything this general.
  if (llvm::isa<llvm::DbgVariableIntrinsic>(I))
    return false;

  if (!I->mayHaveSideEffects())
    return true;

  return false;
}

static bool DCEInstruction(llvm::Instruction *I,
                           llvm::SmallSetVector<llvm::Instruction *, 16> &WorkList,
                           const llvm::TargetLibraryInfo *TLI)
{
  if (myIsInstructionTriviallyDead(I, nullptr))
  {

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
        if (isInstructionTriviallyDead(OpI, nullptr))
          WorkList.insert(OpI);
    }
    llvm::errs() << "erase ";
    I->print(llvm::errs());
    llvm::errs() << "\n";
    I->eraseFromParent();
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
  for (llvm::Instruction &I : llvm::make_early_inc_range(llvm::instructions(F)))
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

bool sysu::FunctionDCE::skipFunction(llvm::Function &F)
{
  return false;
}
#include "llvm/Pass.h"

llvm::PreservedAnalyses sysu::FunctionDCE::run(llvm::Function &F, llvm::FunctionAnalysisManager &AM)
{
  if (skipFunction(F))
    return llvm::PreservedAnalyses::all();

  if (!eliminateDeadCode(F, nullptr))
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