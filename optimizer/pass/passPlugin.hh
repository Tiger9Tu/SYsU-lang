#pragma once
#ifndef __SYSU_OPTIMIZER_PLUGIN_HH_
#define __SYSU_OPTIMIZER_PLUGIN_HH_

#include <llvm/ADT/MapVector.h>
#include <llvm/IR/AbstractCallSite.h>
#include <llvm/IR/Module.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Analysis/AliasAnalysis.h>

#include "scc.hh"
#include "dce.hh"
#include "cse.hh"
#include "llvm/Transforms/Scalar/EarlyCSE.h"

extern "C"
{
  llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo();
}

#endif