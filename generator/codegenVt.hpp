#include <llvm/ADT/APFloat.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/JSON.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/ValueSymbolTable.h>
#include <unordered_map>
#include "asg.hpp"

struct CodegenVisitor : public Visitor
{
private:
    enum State
    {
        LITERAL,
        NORMAL,
    };

public:
    int visit(TranslationUnitDecl *p)
    {
        for (auto i : p->externalDecls)
            i->accept(this);
        return 1;
    }

    int visit(VarDecl *p)
    {
        // opt
        if (p->isUsed == false)
            return 1;

        if (p->is_global)
        {
            module_p->getOrInsertGlobal(p->name, typeToLLVMType(&p->type));
            llvm::GlobalVariable *globalVar = module_p->getNamedGlobal(p->name); // global value is not in symbol table
            if (auto globInitExp = p->initExp)
            {
                globInitExp->accept(this);
                if (p->type.dims.size() > 0)
                {
                    globalVar->setInitializer(llvm::cast<llvm::ConstantArray>(pop()));
                }
                else
                {
                    // 对非数组的全局变量的vardecl 节点赋值，从而进行直接用value进行常量传播（llvm API有bug）
                    if (p->isSingleConst)
                        castV(&p->V, &globInitExp->V, p->type.base_type, globInitExp->type.base_type);
                    globalVar->setInitializer(llvm::cast<llvm::Constant>(pop()));
                }
            }
            else
            {
                globalVar->setInitializer(llvm::Constant::getNullValue(typeToLLVMType(&p->type)));
            }
        }
        else
        {
            // 局部变量在Vist FunctionDecl时已经分配了空间，这里只需要赋值
            llvm::AllocaInst *allo = llvm::cast<llvm::AllocaInst>(getOrInsertLocal(p->id, nullptr));
            if ((p->type.dims.size() > 0))
            {
                uint length = 1;
                for (auto dim : p->type.dims)
                    length *= dim;

                if (p->type.base_type == Type::CHAR)
                {
                    // warning: 默认字符串数组必须初始化
                    p->initExp->accept(this);
                    builder_p->CreateMemCpy(allo, llvm::MaybeAlign(1),
                                            /*src*/ pop(), llvm::MaybeAlign(1),
                                            /*size*/ llvm::ConstantInt::get(*context_p, llvm::APInt(32, length)));
                }
                else
                {
                    builder_p->CreateMemSet(
                        /*ptr*/ allo,
                        /*value*/ llvm::ConstantInt::get(*context_p, llvm::APInt(8, 0)),
                        /*size*/ llvm::ConstantInt::get(*context_p, llvm::APInt(32, length * 4)),
                        llvm::MaybeAlign(8));

                    if (auto arrInitExp = p->initExp)
                    {
                        buildArray(arrInitExp, allo); // 数组初始化的赋值方式需要特殊处理
                    }
                }
            }
            else
            {
                if (auto localInitExp = p->initExp)
                {
                    localInitExp->accept(this);
                    builder_p->CreateStore(pop(), allo);
                    if (p->isSingleConst)
                        castV(&p->V, &localInitExp->V, p->type.base_type, localInitExp->type.base_type);
                }
            }
        }
        return 1;
    }

    int visit(FunctionDecl *p)
    {
        // opt
        if (p->isUsed == false && p->name != "main"){
            return 1;
        }

        llvm::Function *function_value = llvm::Function::Create(reinterpret_cast<llvm::FunctionType *>(typeToLLVMType(&p->type)),
                                                                llvm::Function::ExternalLinkage,
                                                                p->name,
                                                                *module_p);
        if (auto functionBody = p->compoundStmt) // 有函数体
        {
            llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(*context_p, "entry", function_value);
            llvm::BasicBlock *returnBB = llvm::BasicBlock::Create(*context_p, "return", function_value);
            returnBB->moveAfter(entryBB);

            builder_p->SetInsertPoint(entryBB); // 将指令创建器定位到当前块
            buildVar(p, function_value);

            functionBody->accept(this);

            if (!hasBr(builder_p->GetInsertBlock()))
                builder_p->CreateBr(returnBB);

            builder_p->SetInsertPoint(returnBB);
            if (p->type.base_type != Type::VOID)
            {
                llvm::Value *retReg = builder_p->CreateLoad(builder_p->getCurrentFunctionReturnType(), getOrInsertLocal("retval"));
                builder_p->CreateRet(retReg);
            }
            else
                builder_p->CreateRetVoid();
        }
        llvm::verifyFunction(*function_value);

        return 1;
    }

    int visit(ParmVarDecl *p)
    {
        value_stack.push_back(getOrInsertLocal(p->id));
        return 1;
    }

    int visit(CompoundStmt *p)
    {
        for (auto it : p->stmts)
            it->accept(this);
        return 1;
    }

    int visit(ReturnStmt *p)
    {
        if (auto retVal = p->returnExp) // 有返回值
        {
            retVal->accept(this);
            builder_p->CreateStore(pop(), getOrInsertLocal("retval"));
        }
        builder_p->CreateBr(getNamedSucBB({"return"}));
        return 1;
    }

    int visit(DeclStmt *p)
    {
        for (auto decl : p->decls)
            decl->accept(this);
        return 1;
    }

    int visit(ExprStmt *p)
    {
        return p->exp->accept(this);
    }

    int visit(IfStmt *p)
    {
        llvm::Function *TheFunction = builder_p->GetInsertBlock()->getParent();

        // Create blocks for the then and else cases.  Insert the 'then' block at the
        // end of the function.
        llvm::BasicBlock *preBB = builder_p->GetInsertBlock();
        llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(*context_p, "if.then", TheFunction);
        ThenBB->moveAfter(preBB);
        preBB = ThenBB;
        llvm::BasicBlock *ElseBB;
        if (auto elseP = p->elseStmt)
        {
            ElseBB = llvm::BasicBlock::Create(*context_p, "if.else", TheFunction);
            ElseBB->moveAfter(ThenBB);
            preBB = ElseBB;
        }
        llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*context_p, "if.end", TheFunction);
        MergeBB->moveAfter(preBB);

        p->condExp->accept(this);
        llvm::Value *condVal = pop();
        if (!condVal)
            return -1;

        // Convert condition to a bool by comparing non-equal to 0.0.
        if (p->condExp->isSingleConst)
        {
            long double condLDV = getLDV(&p->condExp->V, p->condExp->type.base_type);
            condVal = llvm::ConstantInt::get(*context_p, llvm::APInt(1, bool(condLDV))); // true
        }

        if (condVal->getType() != llvm::Type::getInt1Ty(*context_p))
            condVal = builder_p->CreateICmpNE(condVal, llvm::ConstantInt::get(condVal->getType(), 0), "tobool");

        if (auto elseP = p->elseStmt)
            builder_p->CreateCondBr(condVal, ThenBB, ElseBB);
        else
            builder_p->CreateCondBr(condVal, ThenBB, MergeBB);

        // Emit then value.
        builder_p->SetInsertPoint(ThenBB);
        p->thenStmt->accept(this);
        int brCount = 0; // 当if和else都以br结尾时，删除ifend

        if (!hasBr(builder_p->GetInsertBlock()))
            builder_p->CreateBr(MergeBB);
        else
            brCount++;

        // Emit else block.
        if (auto elseP = p->elseStmt)
        {
            builder_p->SetInsertPoint(ElseBB);
            elseP->accept(this);
            if (!hasBr(builder_p->GetInsertBlock()))
                builder_p->CreateBr(MergeBB);
            else
                brCount++;
        }

        // 如果if/else中有一个block是跳转到mergeBB的，就创建mergeBB
        if (brCount < 2)
        {
            if (!hasBr(builder_p->GetInsertBlock()))
                builder_p->CreateBr(MergeBB);
            builder_p->SetInsertPoint(MergeBB);
        }
        else
            MergeBB->eraseFromParent();
        // llvm::PHINode *PN = builder_p->CreatePHI(llvm::Type::getDoubleTy(*context_p), 2, "iftmp");
        // 先不管PHI
        // PN->addIncoming(then_v, ThenBB);
        // PN->addIncoming(else_v, ElseBB);
        // value_stack.push_back(PN);
        return 1;
    }

    int visit(WhileStmt *p)
    {
        llvm::Function *funcVal = builder_p->GetInsertBlock()->getParent();

        // LoopEnd作为名字的字串，从而在break stmt处可以识别
        llvm::BasicBlock *whileCond = llvm::BasicBlock::Create(*context_p, "while.cond", funcVal);
        whileCond->moveAfter(builder_p->GetInsertBlock());
        llvm::BasicBlock *whileBody = llvm::BasicBlock::Create(*context_p, "while.body", funcVal);
        whileBody->moveAfter(whileCond);
        llvm::BasicBlock *whileEnd = llvm::BasicBlock::Create(*context_p, "while.end", funcVal);
        whileEnd->moveAfter(whileBody);

        builder_p->CreateBr(whileCond);

        // whilecond
        builder_p->SetInsertPoint(whileCond);
        p->condExp->accept(this);
        llvm::Value *condVal = pop();

        if (p->condExp->isSingleConst)
        {
            long double condLDV = getLDV(&p->condExp->V, p->condExp->type.base_type);
            condVal = llvm::ConstantInt::get(*context_p, llvm::APInt(1, bool(condLDV))); // true
        }

        if (condVal->getType() != llvm::Type::getInt1Ty(*context_p))
            condVal = builder_p->CreateICmpNE(condVal, llvm::ConstantInt::get(condVal->getType(), 0), "tobool");

        builder_p->CreateCondBr(condVal, whileBody, whileEnd);

        // whileBody
        builder_p->SetInsertPoint(whileBody);
        p->whileBodyStmt->accept(this);

        if (builder_p->GetInsertBlock()->getTerminator() == nullptr)
            builder_p->CreateBr(whileCond);

        builder_p->SetInsertPoint(whileEnd);

        return 1;
    }

    int visit(DoStmt *p)
    {
        llvm::Function *funcVal = builder_p->GetInsertBlock()->getParent();

        llvm::BasicBlock *doBodyBB = llvm::BasicBlock::Create(*context_p, "do.body", funcVal);
        doBodyBB->moveAfter(builder_p->GetInsertBlock());
        llvm::BasicBlock *doCondBB = llvm::BasicBlock::Create(*context_p, "do.cond", funcVal);
        doCondBB->moveAfter(doBodyBB);
        llvm::BasicBlock *doEndBB = llvm::BasicBlock::Create(*context_p, "do.end", funcVal);
        doEndBB->moveAfter(doCondBB);

        builder_p->CreateBr(doBodyBB);
        builder_p->SetInsertPoint(doBodyBB);
        p->doBodyStmt->accept(this);

        if (builder_p->GetInsertBlock()->getTerminator() == nullptr)
            builder_p->CreateBr(doCondBB);

        builder_p->SetInsertPoint(doCondBB);
        p->condExp->accept(this);
        llvm::Value *condVal = pop();

        if (p->condExp->isSingleConst)
        {
            long double condLDV = getLDV(&p->condExp->V, p->condExp->type.base_type);
            condVal = llvm::ConstantInt::get(*context_p, llvm::APInt(1, bool(condLDV))); // true
        }

        // Convert condition to a bool by comparing non-equal to 0.0.
        if (condVal->getType() != llvm::Type::getInt1Ty(*context_p))
            condVal = builder_p->CreateICmpNE(condVal, llvm::ConstantInt::get(condVal->getType(), 0), "tobool");

        builder_p->CreateCondBr(condVal, doBodyBB, doEndBB);
        builder_p->SetInsertPoint(doEndBB);

        return 1;
    }

    int visit(BreakStmt *)
    {
        builder_p->CreateBr(getNamedSucBB({"while.end"})); // 好像没有for
        return 1;
    }

    int visit(ContinueStmt *)
    {
        builder_p->CreateBr(getNamedPreBB({"while.cond"}));
        return 1;
    }

    int visit(NullStmt *)
    {
        return 1;
    }

    int visit(DeclRefExpr *p)
    {
        if (p->isSingleConst)
            castV(&p->V, &p->referencedDecl->V, p->type.base_type, p->referencedDecl->type.base_type);

        llvm::Value *decl_ref_value;
        if (auto local_var = getOrInsertLocal(p->referencedDecl->id))
            decl_ref_value = local_var;
        else if (auto global_value = module_p->getNamedGlobal(p->referencedDecl->name))
            decl_ref_value = global_value;
        else if (auto func = module_p->getFunction(p->referencedDecl->name))
            decl_ref_value = func;
        else
            return -1;

        value_stack.push_back(decl_ref_value);
        return 1;
    }

    int visit(InitListExpr *p)
    {
        llvm::ArrayType *arrType = llvm::cast<llvm::ArrayType>(typeToLLVMType(&p->type));
        std::vector<llvm::Constant *> const_array_elems;
        for (auto iexpr : p->initExps)
        {
            iexpr->accept(this);
            const_array_elems.push_back(llvm::cast<llvm::Constant>(pop()));
        }

        while (const_array_elems.size() < arrType->getArrayNumElements())
            const_array_elems.push_back(llvm::Constant::getNullValue(arrType->getArrayElementType()));

        value_stack.push_back(llvm::ConstantArray::get(arrType, const_array_elems));
        return 1;
    }

    int visit(ImplicitCastExpr *p)
    {
        p->toCastExp->accept(this);
        bool is_global = false;
        if (!builder_p->GetInsertBlock())
            is_global = true;

        if (p->isSingleConst)
        {
            castV(&p->V, &p->toCastExp->V, p->type.base_type, p->toCastExp->type.base_type);
            pop(); // 替换掉栈顶的value*
            value_stack.push_back(valueToLLVMValue(&p->V, p->type.base_type));
            return 1;
        }

        if (p->castKind == "LValueToRValue")
        {
            llvm::Value *tocast = pop();
            std::string name = "cast";
            if (auto dre = p->toCastExp->dcast<DeclRefExpr>())
            {
                if (auto pvd = dre->referencedDecl->dcast<ParmVarDecl>())
                    name = "parm.arr.cast";
            }
            value_stack.push_back(builder_p->CreateLoad(typeToLLVMType(&p->type), tocast, name));
        }
        else if (p->castKind == "IntegralCast")
        {
            if (p->toCastExp->type.base_type == Type::CHAR)
                value_stack.push_back(builder_p->CreateSExt(pop(), typeToLLVMType(&p->type), "conv"));
            else
                value_stack.push_back(builder_p->CreateTrunc(pop(), typeToLLVMType(&p->type), "conv"));
        }
        else if (p->castKind == "IntegralToFloating")
        {
            value_stack.push_back(builder_p->CreateSIToFP(pop(), typeToLLVMType(&p->type), "conv"));
        }
        else if (p->castKind == "FloatingToIntegral")
        {
            value_stack.push_back(builder_p->CreateFPToSI(pop(), typeToLLVMType(&p->type), "conv"));
        }
        else if (p->castKind == "FloatingCast")
        {
            if (p->toCastExp->type.base_type == Type::FLOAT && p->type.base_type == Type::DOUBLE)
                value_stack.push_back(builder_p->CreateFPExt(pop(), typeToLLVMType(&p->type), "conv"));
            else
                value_stack.push_back(builder_p->CreateFPTrunc(pop(), typeToLLVMType(&p->type), "conv"));
        }
        return 1;
    }

    int visit(ArraySubscriptExpr *p)
    {
        p->indexExp->accept(this);
        llvm::Value *idxVal = pop();
        p->arrayPointerExp->accept(this);
        llvm::Value *arrVal = pop();

        llvm::SmallVector<llvm::Value *, 3> indexes{llvm::ConstantInt::get(*context_p, llvm::APInt(32, 0))};
        llvm::Type *etype = arrVal->getType()->getPointerElementType();
        if (arrVal->getName().str().find("parm.arr.cast") != std::string::npos)
            indexes.pop_back();
        indexes.push_back(idxVal);
        llvm::Value *base = builder_p->CreateInBoundsGEP(etype, arrVal, indexes);

        value_stack.push_back(base);
        return 1;
    }

    int visit(CallExpr *p)
    {
        p->functionPointerExp->accept(this);
        llvm::Function *funcVal = llvm::cast<llvm::Function>(pop());

        std::vector<llvm::Value *> args;
        for (auto argp : p->parmExps)
        {
            argp->accept(this);
            llvm::Type *targetType = typeToLLVMType(&argp->type);
            llvm::Value *arg = pop();
            llvm::Type *argType = arg->getType();
            while (argType->isPointerTy() && argType != targetType && argType->getPointerElementType()->isArrayTy())
            {
                arg = builder_p->CreateInBoundsGEP(argType->getPointerElementType(),
                                                   arg,
                                                   {llvm::ConstantInt::get(*context_p, llvm::APInt(32, 0)),
                                                    llvm::ConstantInt::get(*context_p, llvm::APInt(32, 0))});
                argType = arg->getType();
            }

            while (argType->isPointerTy() && argType != targetType && argType->getPointerElementType()->isArrayTy())
            {
                arg = builder_p->CreateLoad(argType->getPointerElementType(), arg);
                argType = arg->getType();
            }

            if (argType != targetType)
            {
                arg = builder_p->CreateZExt(arg, targetType);
            }
            args.push_back(arg);
        }
        value_stack.push_back(builder_p->CreateCall(funcVal, args));
        return 1;
    }

    int visit(ParenExpr *p)
    {
        p->exp->accept(this);
        if (p->isSingleConst)
            castV(&p->V, &p->exp->V, p->type.base_type, p->exp->type.base_type);
        return 1;
    }

    int visit(BinaryOperator *p)
    {
        bool isSingleConst = false;
        bool LHS_is_fd = false;
        long double GLHSV, GRHSV;
        llvm::Value *LHS, *RHS;
        // literal(const val的isSingleConst为 true)直接计算值
        if (p->isSingleConst)
        {
            p->lExp->accept(this);
            GLHSV = getLDV(&p->lExp->V, p->lExp->type.base_type);
            p->rExp->accept(this);
            GRHSV = getLDV(&p->rExp->V, p->rExp->type.base_type);
        }

        // C语言要求进行短路处理
        if (p->opcode == "&&" || p->opcode == "||")
        {
            if (p->isSingleConst)
            {
                if (p->opcode == "&&")
                    assignV(GLHSV && GRHSV, &p->V, p->type.base_type);
                else
                    assignV(GLHSV || GRHSV, &p->V, p->type.base_type);
            }
            else
            {
                llvm::BasicBlock *curBB = builder_p->GetInsertBlock();
                std::vector<std::string> lhsTrueSucBBName, lhsFalseSucBBName;
                std::string RHSBBName;
                if (p->opcode == "&&")
                {
                    RHSBBName = "land.lhs.true";
                    lhsTrueSucBBName = {"land.lhs.true"};
                    lhsFalseSucBBName = {"else", "end", "lor.lhs.false"};
                }
                else
                {
                    RHSBBName = "lor.lhs.false";
                    lhsTrueSucBBName = {"then", "land.lhs.true"};
                    lhsFalseSucBBName = {"lor.lhs.false"};
                }

                llvm::BasicBlock *RHSBB = llvm::BasicBlock::Create(*context_p, RHSBBName, curBB->getParent());
                RHSBB->moveAfter(curBB);

                // 执行左手边
                p->lExp->accept(this);
                LHS = pop();
                // 在C语言的语义上int可以作为LHS，但是在IR的语义上需要转换。因此此处转换不能在IMplicitcast进行。
                if (LHS->getType() != llvm::Type::getInt1Ty(*context_p))
                    LHS = builder_p->CreateICmpNE(LHS, llvm::ConstantInt::get(*context_p, llvm::APInt(32, "0", 10)));

                builder_p->CreateCondBr(LHS, getNamedSucBB(lhsTrueSucBBName), getNamedSucBB(lhsFalseSucBBName));
                builder_p->SetInsertPoint(RHSBB);
                p->rExp->accept(this);
                RHS = pop();
                value_stack.push_back(RHS);
            }
        }
        else
        {
            if (!p->isSingleConst)
            {
                p->lExp->accept(this);
                LHS = pop();
                p->rExp->accept(this);
                RHS = pop();
                LHS_is_fd = LHS->getType()->isFloatTy() || LHS->getType()->isDoubleTy();
            }

            if (p->opcode == "=")
                value_stack.push_back(builder_p->CreateStore(RHS, LHS));
            else if (p->opcode == "+")
            {
                if (p->isSingleConst)
                {
                    assignV(GLHSV + GRHSV, &p->V, p->type.base_type);
                }
                else
                {
                    if (LHS_is_fd)
                        value_stack.push_back(builder_p->CreateFAdd(LHS, RHS));
                    else
                        value_stack.push_back(builder_p->CreateAdd(LHS, RHS));
                }
            }
            else if (p->opcode == "-")
            {
                if (p->isSingleConst)
                {
                    assignV(GLHSV - GRHSV, &p->V, p->type.base_type);
                }
                else
                {
                    if (LHS_is_fd)
                        value_stack.push_back(builder_p->CreateFSub(LHS, RHS));
                    else
                        value_stack.push_back(builder_p->CreateSub(LHS, RHS));
                }
            }
            else if (p->opcode == "*")
            {
                if (p->isSingleConst)
                {
                    assignV(GLHSV * GRHSV, &p->V, p->type.base_type);
                }
                else
                {
                    if (LHS_is_fd)
                        value_stack.push_back(builder_p->CreateFMul(LHS, RHS));
                    else
                        value_stack.push_back(builder_p->CreateMul(LHS, RHS));
                }
            }
            else if (p->opcode == "/")
            {
                if (p->isSingleConst)
                    assignV(GLHSV / GRHSV, &p->V, p->type.base_type);
                else
                {
                    if (LHS_is_fd)
                        value_stack.push_back(builder_p->CreateFDiv(LHS, RHS));
                    else
                        value_stack.push_back(builder_p->CreateSDiv(LHS, RHS));
                }
            }
            else if (p->opcode == "%")
            {
                if (p->isSingleConst)
                    assignV(int(GLHSV) % int(GRHSV), &p->V, p->type.base_type);
                else
                    value_stack.push_back(builder_p->CreateSRem(LHS, RHS));
            }
            else
            {
                if (!p->isSingleConst && (RHS->getType() != LHS->getType()))
                {
                    if (LHS->getType() == llvm::Type::getInt1Ty(*context_p))
                        RHS = builder_p->CreateICmpNE(RHS, llvm::ConstantInt::get(RHS->getType(), 0));
                    else if (RHS->getType() == llvm::Type::getInt1Ty(*context_p))
                        LHS = builder_p->CreateICmpNE(LHS, llvm::ConstantInt::get(LHS->getType(), 0));
                }

                if (p->opcode == ">")
                {
                    if (p->isSingleConst)
                        assignV(GLHSV > GRHSV, &p->V, p->type.base_type);
                    else
                    {
                        if (LHS_is_fd)
                            value_stack.push_back(builder_p->CreateFCmpUGT(LHS, RHS));
                        else
                            value_stack.push_back(builder_p->CreateICmpSGT(LHS, RHS));
                    }
                }
                else if (p->opcode == "<")
                {
                    if (p->isSingleConst)
                        assignV(GLHSV < GRHSV, &p->V, p->type.base_type);
                    else
                    {
                        if (LHS_is_fd)
                            value_stack.push_back(builder_p->CreateFCmpULT(LHS, RHS));
                        else
                            value_stack.push_back(builder_p->CreateICmpSLT(LHS, RHS));
                    }
                }
                else if (p->opcode == ">=")
                {
                    if (p->isSingleConst)
                        assignV(GLHSV >= GRHSV, &p->V, p->type.base_type);
                    else
                    {
                        if (LHS_is_fd)
                            value_stack.push_back(builder_p->CreateFCmpUGE(LHS, RHS));
                        else
                            value_stack.push_back(builder_p->CreateICmpSGE(LHS, RHS));
                    }
                }
                else if (p->opcode == "<=")
                {
                    if (p->isSingleConst)
                        assignV(GLHSV <= GRHSV, &p->V, p->type.base_type);
                    else
                    {
                        if (LHS_is_fd)
                            value_stack.push_back(builder_p->CreateFCmpULE(LHS, RHS));
                        else
                            value_stack.push_back(builder_p->CreateICmpSLE(LHS, RHS));
                    }
                }
                else if (p->opcode == "==")
                {
                    if (p->isSingleConst)
                        assignV(GLHSV == GRHSV, &p->V, p->type.base_type);
                    else
                    {
                        if (LHS_is_fd)
                            value_stack.push_back(builder_p->CreateFCmpUEQ(LHS, RHS));
                        else
                            value_stack.push_back(builder_p->CreateICmpEQ(LHS, RHS));
                    }
                }
                else if (p->opcode == "!=")
                {
                    if (p->isSingleConst)
                        assignV(GLHSV != GRHSV, &p->V, p->type.base_type);
                    else
                    {
                        if (LHS_is_fd)
                            value_stack.push_back(builder_p->CreateFCmpUNE(LHS, RHS));
                        else
                            value_stack.push_back(builder_p->CreateICmpNE(LHS, RHS));
                    }
                }
            }
        }

        if (p->isSingleConst)
            value_stack.push_back(valueToLLVMValue(&p->V, p->type.base_type));
        return 1;
    }

    int visit(UnaryOperator *p)
    {
        p->exp->accept(this);
        llvm::Value *HS;
        long double HSV;
        if (p->isSingleConst)
            HSV = getLDV(&p->exp->V, p->type.base_type);
        else
            HS = pop();

        if (p->opcode == "!")
        {
            if (p->isSingleConst)
                assignV(!(int)HSV, &p->V, p->type.base_type);
            else
            {
                if (HS->getType() != llvm::Type::getInt1Ty(*context_p))
                    HS = builder_p->CreateICmpNE(HS, llvm::ConstantInt::get(HS->getType(), 0));
                value_stack.push_back(builder_p->CreateXor(HS, llvm::ConstantInt::get(HS->getType(), 1)));
            }
        }
        else if (p->opcode == "-")
        {
            if (p->isSingleConst)
                assignV(-HSV, &p->V, p->type.base_type);
            else
            {
                if (HS->getType()->isFloatTy() || HS->getType()->isDoubleTy())
                    value_stack.push_back(builder_p->CreateFNeg(HS, "fneg"));
                else
                    value_stack.push_back(builder_p->CreateSub(llvm::ConstantInt::get(HS->getType(), 0), HS));
            }
        }
        else if (p->opcode == "+")
        {
            if (p->isSingleConst)
                assignV(HSV, &p->V, p->type.base_type);
            else
                value_stack.push_back(HS);
        }

        if (p->isSingleConst)
            value_stack.push_back(valueToLLVMValue(&p->V, p->type.base_type));
        return 1;
    }

    int visit(IntegerLiteral *p)
    {
        value_stack.push_back(valueToLLVMValue(&p->V, p->type.base_type));
        return State::LITERAL;
    }

    int visit(StringLiteral *p)
    {
        llvm::Constant *strVal = llvm::ConstantDataArray::getString(*context_p,
                                                                    p->strVal);
        llvm::Value *globStrVal = new llvm::GlobalVariable(*module_p,
                                                           strVal->getType(),
                                                           true,
                                                           llvm::GlobalValue::PrivateLinkage,
                                                           /*initializer*/ strVal,
                                                           "str");
        value_stack.push_back(globStrVal);
        return State::LITERAL;
    }

    int visit(FloatingLiteral *p)
    {
        value_stack.push_back(valueToLLVMValue(&p->V, p->type.base_type));
        return State::LITERAL;
    }

    void print()
    {
        module_p->print(llvm::outs(), nullptr);
    }

    CodegenVisitor()
    {
        context_p = std::make_unique<llvm::LLVMContext>();
        builder_p = std::make_unique<llvm::IRBuilder<>>(*context_p);
        module_p = std::make_unique<llvm::Module>("-", *context_p);
    }

private:
    std::unique_ptr<llvm::LLVMContext> context_p;
    std::unique_ptr<llvm::IRBuilder<>> builder_p;
    std::unique_ptr<llvm::Module> module_p;

    std::vector<llvm::Value *> value_stack;
    std::unordered_map<std::string, llvm::Value *> sym_table;

    llvm::Type *typeToLLVMType(Type *_type)
    {
        llvm::Type *llvm_base_type;
        switch (_type->base_type)
        {
        case (0):
            llvm_base_type = llvm::Type::getVoidTy(*context_p);
            break;
        case (1):
            llvm_base_type = llvm::Type::getInt8Ty(*context_p);
            break;
        case (2):
            llvm_base_type = llvm::Type::getInt32Ty(*context_p);
            break;
        case (3):
            llvm_base_type = llvm::Type::getInt32Ty(*context_p);
            break;
        case (4):
            llvm_base_type = llvm::Type::getInt64Ty(*context_p);
            break;
        case (5):
            llvm_base_type = llvm::Type::getInt64Ty(*context_p);
            break;
        case (6):
            llvm_base_type = llvm::Type::getInt128Ty(*context_p);
            break;
        case (7):
            llvm_base_type = llvm::Type::getFloatTy(*context_p);
            break;
        case (8):
            llvm_base_type = llvm::Type::getDoubleTy(*context_p);
            break;
        }

        llvm::Type *llvm_ret_type;

        if (_type->is_func)
        {
            std::vector<llvm::Type *> parms;
            for (auto p : _type->parm_vars)
            {
                parms.push_back(typeToLLVMType(&p->type));
            }

            llvm_ret_type = llvm::FunctionType::get(llvm_base_type, parms, false);
        }
        else
        {
            llvm_ret_type = llvm_base_type;
            if (_type->dims.size() > 0)
            {
                long dim = _type->dims.back();
                llvm::ArrayType *arr_type = llvm::ArrayType::get(llvm_base_type, dim);
                for (int i = _type->dims.size() - 2; i >= 0; i--)
                    arr_type = llvm::ArrayType::get(arr_type, _type->dims[i]);
                llvm_ret_type = arr_type;
            }

            if (_type->is_ptr)
            {
                llvm_ret_type = llvm::PointerType::get(llvm_ret_type, 0);
            }
        }
        return llvm_ret_type;
    }

    llvm::Value *valueToLLVMValue(Expr::value *V, Type::BaseType &bType)
    {
        switch (bType)
        {
        case 1:
            return llvm::ConstantInt::get(*context_p, llvm::APInt(8, V->charV));
        case 2:
            return llvm::ConstantInt::get(*context_p, llvm::APInt(32, V->intV));
        case 3:
            return llvm::ConstantInt::get(*context_p, llvm::APInt(32, V->unsignedIntV, true));
        case 4:
            return llvm::ConstantInt::get(*context_p, llvm::APInt(64, V->longV));
        case 5:
            return llvm::ConstantInt::get(*context_p, llvm::APInt(64, V->unsignedLongV, true));
        case 6:
            return llvm::ConstantInt::get(*context_p, llvm::APInt(128, V->longLongV));
        case 7:
            return llvm::ConstantFP::get(*context_p, llvm::APFloat(V->floatV));
        case 8:
            return llvm::ConstantFP::get(*context_p, llvm::APFloat(V->doubleV));
        default:
            return nullptr;
        }
    }

    void buildArray(Expr *p, llvm::Value *base)
    {
        if (auto initListExp = p->dcast<InitListExpr>())
        {
            int idx = 0;
            for (auto sub_expr : initListExp->initExps)
            {
                llvm::Value *idxVal = llvm::ConstantInt::get(*context_p, llvm::APInt(32, idx++));
                llvm::Value *nextBaseVal = builder_p->CreateInBoundsGEP(typeToLLVMType(&p->type),
                                                                        base,
                                                                        {llvm::ConstantInt::get(*context_p, llvm::APInt(32, 0)), idxVal});
                buildArray(sub_expr, nextBaseVal);
            }
        }
        else
        {
            p->accept(this);
            builder_p->CreateStore(pop(), base);
        }
    }

    // 函数变量初始化
    void buildVar(FunctionDecl *p, llvm::Function *funcValalue)
    {
        // 为返回值分配空间
        if (p->type.base_type != Type::VOID)
        {
            llvm::Value *retval = builder_p->CreateAlloca(llvm::cast<llvm::FunctionType>(typeToLLVMType(&p->type))->getReturnType(),
                                                          nullptr,
                                                          "retval");
            getOrInsertLocal("retval", retval);
            builder_p->CreateStore(llvm::Constant::getNullValue(retval->getType()->getPointerElementType()),
                                   retval);
        }

        // 初始化参数
        auto arg_iter = funcValalue->arg_begin();
        for (int parmidx = 0; parmidx < p->parmVars.size(); parmidx++)
        {
            ParmVarDecl *parm = p->parmVars[parmidx];
            llvm::AllocaInst *allo = builder_p->CreateAlloca(typeToLLVMType(&parm->type),
                                                             0,
                                                             parm->name);
            getOrInsertLocal(parm->id, allo);
            builder_p->CreateStore(&(*arg_iter), allo);
            arg_iter = std::next(arg_iter);
        }

        // 为函数内所有位置的局部变量分配空间
        for (auto localvar : p->localVars)
        {
            if (localvar->isUsed)
            {
                llvm::AllocaInst *allo = builder_p->CreateAlloca(typeToLLVMType(&localvar->type), nullptr, localvar->name);
                getOrInsertLocal(localvar->id, allo);
            }
        }
    }

    bool hasBr(llvm::BasicBlock *BB)
    {
        if (auto termIns = BB->getTerminator())
            if (strcmp(termIns->getOpcodeName(), "br") == 0)
                return true;
        return false;
    }

    llvm::BasicBlock *getNamedBB(const std::vector<std::string> substrs, int is_suc, bool includeCurBB)
    {
        llvm::Function *funcVal = builder_p->GetInsertBlock()->getParent();
        llvm::StringRef curBBName = builder_p->GetInsertBlock()->getName();
        bool curBBVisited = false;
        for (llvm::BasicBlock &BB : *funcVal)
        {
            if (BB.getName() == curBBName)
            {
                curBBVisited = true;
                if (!includeCurBB)
                    continue;
            }

            if (is_suc && curBBVisited || !is_suc && !curBBVisited)
            {
                for (auto substr : substrs)
                    if (BB.getName().str().find(substr) != std::string::npos)
                        return &BB;
            }
            else
            {
                if (!is_suc)
                    break;
            }
        }
        return builder_p->GetInsertBlock();
    }

    llvm::BasicBlock *getNamedPreBB(const std::vector<std::string> substrs)
    {
        return getNamedBB(substrs, 0, true);
    }

    llvm::BasicBlock *getNamedSucBB(const std::vector<std::string> substrs)
    {
        return getNamedBB(substrs, 1, false);
    }

    llvm::Value *getOrInsertLocal(std::string id, llvm::Value *val = nullptr)
    {
        if (val != nullptr)
        {
            auto iter = sym_table.find(id);
            if (iter != sym_table.end())
                sym_table.erase(iter);
            sym_table.insert({id, val});
        }

        if (sym_table.find(id) != sym_table.end())
        {
            return sym_table[id];
        }
        return nullptr;
    }

    llvm::Value *pop()
    {
        llvm::Value *tmp = value_stack.back();
        value_stack.pop_back();
        return tmp;
    }
};
