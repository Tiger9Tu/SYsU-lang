#include <llvm/ADT/MapVector.h>
#include <llvm/IR/AbstractCallSite.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Analysis/AliasAnalysis.h>

namespace sysu
{

    class StaticCallCounter : public llvm::AnalysisInfoMixin<StaticCallCounter>
    {
    public:
        using Result = llvm::MapVector<const llvm::Function *, unsigned>;
        Result run(llvm::Module &M, llvm::ModuleAnalysisManager &);

    private:
        // A special type used by analysis passes to provide an address that
        // identifies that particular analysis pass type.
        static llvm::AnalysisKey Key;
        friend struct llvm::AnalysisInfoMixin<StaticCallCounter>;
    };

    class StaticCallCounterPrinter
        : public llvm::PassInfoMixin<StaticCallCounterPrinter>
    {
    public:
        explicit StaticCallCounterPrinter(llvm::raw_ostream &OutS) : OS(OutS) {}
        llvm::PreservedAnalyses run(llvm::Module &M,
                                    llvm::ModuleAnalysisManager &MAM);

    private:
        llvm::raw_ostream &OS;
    };

} // namespace sysu
