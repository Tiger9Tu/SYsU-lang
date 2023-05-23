//===- CSE.cpp - Simple and fast CSE pass ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass performs a simple dominator tree walk that eliminates trivially
// redundant instructions.
//
//===----------------------------------------------------------------------===//
#include "cse.hh"
#include "domain/exp.hpp"
#include "domain/variable.hpp"
#include <unordered_map>
#include <vector>

using namespace llvm;

bool eliminateCSECode(Function &F)
{
    bool changed = false;

    std::vector<Instruction *> toRemove;

    for (auto &BB : F)
    {
        std::unordered_map<dfa::Variable, Value *> Var2Value;
        std::unordered_map<dfa::Expression, Value *> Exp2Value;
        for (auto &I : BB)
        {
            if (isa<StoreInst>(I))
            {
                dfa::Variable Var(I.getOperand(1));
                Var2Value.erase(Var);
            }
            else if (isa<LoadInst>(I))
            {
                dfa::Variable Var(I.getOperand(0));
                if (Var2Value.find(Var) != Var2Value.end())
                {
                    I.replaceAllUsesWith(Var2Value[Var]);
                    toRemove.push_back(&I);
                    changed = true;
                }
                else
                {
                    Var2Value[Var] = dyn_cast<Value>(&I);
                }
            }
            else if (I.isBinaryOp())
            {
                dfa::Expression Exp(*dyn_cast<BinaryOperator>(&I));
                if (Exp2Value.find(Exp) != Exp2Value.end())
                {
                    I.replaceAllUsesWith(Exp2Value[Exp]);
                    toRemove.push_back(&I);
                }
                else
                {
                    Exp2Value[Exp] = dyn_cast<Value>(&I);
                }
            }
        }
    }

    if (toRemove.size() > 0)
        changed = true;

    for (auto I : toRemove)
        I->eraseFromParent();

    return changed;
}

bool sysu::FunctionCSE::skipFunction(Function &F)
{
    return false;
}

PreservedAnalyses sysu::FunctionCSE::run(Function &F, FunctionAnalysisManager &AM)
{
    if (skipFunction(F))
        return PreservedAnalyses::all();

    if (!eliminateCSECode(F))
        return PreservedAnalyses::all();

    PreservedAnalyses PA;
    PA.preserveSet<CFGAnalyses>();
    return PA;
}
