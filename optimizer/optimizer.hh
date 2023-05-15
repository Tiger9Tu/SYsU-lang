#pragma once
#ifndef __SYSU_OPTIMIZER_PLUGIN_HH_
#define __SYSU_OPTIMIZER_PLUGIN_HH_

#include <llvm/ADT/MapVector.h>
#include <llvm/IR/AbstractCallSite.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Pass.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Analysis/AliasAnalysis.h>

#include <llvm/Transforms/Scalar/DCE.h>
#include <llvm/ADT/SetVector.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instruction.h>
#include <llvm/InitializePasses.h>
#include <llvm/Support/DebugCounter.h>
#include <llvm/Support/Debug.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Utils/AssumeBundleBuilder.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <llvm/Transforms/Utils/Local.h>

// using namespace llvm;

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

  class FunctionDCE : public llvm::PassInfoMixin<FunctionDCE>
  {
  public:
    using Result = llvm::PreservedAnalyses;
    Result run(llvm::Function &M, llvm::FunctionAnalysisManager &FAM);
  };

} // namespace sysu

extern "C"
{
  llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo();
}

#endif