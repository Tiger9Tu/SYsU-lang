//===- myEarlyCSE.cpp - Simple and fast CSE pass ----------------------------===//
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

using namespace llvm;

bool eliminateCSECode(Function &F)
{

    for (auto &BB : F)
    {
        SmallVector<Instruction, 16> toDelete;
        initializeDomainTree(BB);
        for (auto &I : BB)
        {

            if (I.isBinaryOp())
            {
                std::string expr = I.getOpcodeName();
                expr += I.getOperand(0)->getName().str();
                expr += I.getOperand(1)->getName().str();

                if (exprMap.find(expr) != exprMap.end())
                {
                    toDelete.push_back(&I);
                }
                else
                {
                    exprMap[expr] = &I;
                }
            }
        }
    }

    for (auto &BB : F)
    {
        for (auto &I : BB)
        {
        }
    }

    for (auto &I : toDelete)
    {
        I->eraseFromParent();
    }

    return !toDelete.empty();
}
return false;
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
