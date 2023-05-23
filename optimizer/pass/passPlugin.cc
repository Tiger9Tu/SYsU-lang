#include "passPlugin.hh"
#include <llvm/Passes/PassBuilder.h>

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