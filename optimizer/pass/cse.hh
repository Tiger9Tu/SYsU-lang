
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/MapVector.h>
#include <llvm/ADT/SetVector.h>
#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/AbstractCallSite.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/IR/InstIterator.h>

namespace sysu
{
    class FunctionCSE : public llvm::PassInfoMixin<FunctionCSE>
    {
    public:
        using Result = llvm::PreservedAnalyses;
        Result run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM);

    private:
        bool skipFunction(llvm::Function &F);
    };

} // end namespace llvm
