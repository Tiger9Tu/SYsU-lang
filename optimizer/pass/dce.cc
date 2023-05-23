#include "dce.hh"
using namespace llvm;
//===--------------------------------------------------------------------===//
// DeadCodeElimination pass implementation
//

bool myIsInstructionTriviallyDead(Instruction *I, const TargetLibraryInfo *TLI)
{
  if (!I->use_empty())
    return false;
  if (I->isTerminator())
    return false;

  // We don't want the landingpad-like instructions removed by anything this
  // general.
  if (I->isEHPad())
    return false;

  if (!I->mayHaveSideEffects())
    return true;

  return false;
}

static bool DCEInstruction(Instruction *I,
                           SmallSetVector<Instruction *, 16> &WorkList,
                           const TargetLibraryInfo *TLI)
{
  if (myIsInstructionTriviallyDead(I, nullptr))
  {

    // Null out all of the instruction's operands to see if any operand becomes
    // dead as we go.
    for (unsigned i = 0, e = I->getNumOperands(); i != e; ++i)
    {
      Value *OpV = I->getOperand(i);
      I->setOperand(i, nullptr);

      if (!OpV->use_empty() || I == OpV)
        continue;

      // If the operand is an instruction that became dead as we nulled out the
      // operand, and if it is 'trivially' dead, delete it in a future loop
      // iteration.
      if (Instruction *OpI = dyn_cast<Instruction>(OpV))
        if (myIsInstructionTriviallyDead(OpI, nullptr))
          WorkList.insert(OpI);
    }
    I->eraseFromParent();
    return true;
  }
  return false;
}

static bool eliminateDeadCode(Function &F, TargetLibraryInfo *TLI)
{
  bool MadeChange = false;
  SmallSetVector<Instruction *, 16> WorkList;
  // Iterate over the original function, only adding insts to the worklist
  // if they actually need to be revisited. This avoids having to pre-init
  // the worklist with the entire function's worth of instructions.
  for (Instruction &I : make_early_inc_range(instructions(F)))
  {
    // We're visiting this instruction now, so make sure it's not in the
    // worklist from an earlier visit.
    if (!WorkList.count(&I))
      MadeChange |= DCEInstruction(&I, WorkList, TLI);
  }

  while (!WorkList.empty())
  {
    Instruction *I = WorkList.pop_back_val();
    MadeChange |= DCEInstruction(I, WorkList, TLI);
  }
  return MadeChange;
}

bool sysu::FunctionDCE::skipFunction(Function &F)
{
  return false;
}

PreservedAnalyses sysu::FunctionDCE::run(Function &F, FunctionAnalysisManager &AM)
{
  if (skipFunction(F))
    return PreservedAnalyses::all();

  if (!eliminateDeadCode(F, nullptr))
    return PreservedAnalyses::all();

  PreservedAnalyses PA;
  PA.preserveSet<CFGAnalyses>();
  return PA;
}

/*
//===--------------------------------------------------------------------===//
// Function alive variable analysis pass implementation
//

bool isRootAliveInst(Instruction &I)
{
  return !I.isSafeToRemove() ||
         I.isTerminator() || // ||
         isa<CallInst>(I) ||
         (isa<StoreInst>(I) && isa<GlobalValue>(I.getOperand(1))); // I.mayReadOrWriteMemory();
}

bool isAliveInst(Instruction &I)
{
  if (isRootAliveInst(I))
    return true;
}

sysu::FunctionAliveVariableAnalyser::Result
sysu::FunctionAliveVariableAnalyser::run(Function &F, FunctionAnalysisManager &)
{
  SmallVector<Instruction *, 128> Res;

  SmallVector<Value *, 128> aliveValues;
  for (Instruction &I : instructions(F))
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
bool isDeadInst(Instruction &I)
{
  if (isAliveInst(I))
    return false;
  if (I.use_empty())
    return true;
  for (Use &OI : I.operands())
  {
    if (Instruction *Inst = dyn_cast<Instruction>(OI.get()))
    {
      if (!isDeadInst(*Inst))
        return false;
    }
  }
  return true;
}

bool sysu::FunctionDCE::eliminateDeadCode(Function &F)
{ */
/*
std::vector<Instruction *> deadInsts;
for (Instruction &I : instructions(F))
  if (isDeadInst(I))
    deadInsts.push_back(&I);

for (Instruction *I : deadInsts)
{
  I->dropAllReferences();
  I->eraseFromParent();
}
return !deadInsts.empty();

*/
/*
  SmallPtrSet<Instruction *, 32> Alive;
 SmallVector<Instruction *, 128> Worklist;

 for (Instruction &I : instructions(F))
 {
   if (isAliveInst(I))
   {

     Alive.insert(&I);
     Worklist.push_back(&I);
   }
 }

 while (!Worklist.empty())
 {
   Instruction *LiveInst = Worklist.pop_back_val();
   for (Use &OI : LiveInst->operands())
   {
     if (Instruction *Inst = dyn_cast<Instruction>(OI))
     {

       if (Alive.insert(Inst).second)
       {
         Worklist.push_back(Inst);
       }
     }
   }
 }

 for (Instruction &I : instructions(F))
 {
   if (!Alive.count(&I))
   {
     Worklist.push_back(&I);
     I.dropAllReferences();
   }
 }

 for (Instruction *&I : Worklist)
 {
   I->eraseFromParent();
 }
 return !Worklist.empty();
}

*/