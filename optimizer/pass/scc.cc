#include "scc.hh"
//===--------------------------------------------------------------------===//
// Function call pass implementation
//
using namespace llvm;
PreservedAnalyses
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
sysu::StaticCallCounter::run(Module &M, ModuleAnalysisManager &)
{
    MapVector<const Function *, unsigned> Res;

    for (auto &Func : M)
    {
        for (auto &BB : Func)
        {
            for (auto &Ins : BB)
            {

                // If this is a call instruction then CB will be not null.
                auto *CB = dyn_cast<CallBase>(&Ins);
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

AnalysisKey sysu::StaticCallCounter::Key;