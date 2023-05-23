#include <llvm/ADT/MapVector.h>
#include <llvm/ADT/SetVector.h>
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
    class FunctionDCE : public llvm::PassInfoMixin<FunctionDCE>
    {
    public:
        using Result = llvm::PreservedAnalyses;
        Result run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM);

    private:
        bool skipFunction(llvm::Function &F);
    };
}