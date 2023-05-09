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
        int count;
        for (auto i : *p)
        {
            count += i->accept(this);
        }

        return count;
    }

    int visit(ExternalDecl *p)
    {
        return 1;
    }

    int visit(InitDeclList *p)
    {
        return 1;
    }

    int visit(FunctionDecltor *p)
    {
        return 1;
    }

    int visit(VarDecl *p)
    {
        if (p->is_global)
        {
            module_p->getOrInsertGlobal(p->name, typeToLLVMType(&p->type));
            llvm::GlobalVariable *globalVar = module_p->getNamedGlobal(p->name); // global value is not in symbol table
            if (p->flag)
            {
                p->expr_p->accept(this);
                if (p->type.dims.size() > 0)
                    globalVar->setInitializer(llvm::cast<llvm::ConstantArray>(pop()));
                else
                    globalVar->setInitializer(llvm::cast<llvm::Constant>(pop()));
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
                    // 默认字符串数组必须初始化
                    p->expr_p->accept(this);
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

                    if (p->flag)
                    {
                        buildArray(p->expr_p, allo); // 数组初始化的赋值方式需要特殊处理
                    }
                }
            }
            else
            {
                if (p->flag)
                {
                    p->expr_p->accept(this);
                    builder_p->CreateStore(pop(), allo);
                }
            }
        }
        return 1;
    }

    int visit(FunctionDecl *p)
    {
        llvm::Function *function_value = llvm::Function::Create(reinterpret_cast<llvm::FunctionType *>(typeToLLVMType(&p->type)),
                                                                llvm::Function::ExternalLinkage,
                                                                p->name,
                                                                *module_p);
        if (p->flag) // 有函数体
        {
            llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(*context_p, "entry", function_value);
            llvm::BasicBlock *returnBB = llvm::BasicBlock::Create(*context_p, "return", function_value);
            returnBB->moveAfter(entryBB);

            builder_p->SetInsertPoint(entryBB); // 将指令创建器定位到当前块
            buildVar(p, function_value);

            p->compound_stmt_p->accept(this);

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

    int visit(ParmVarDeclList *)
    {
        return 1;
    }
    int visit(ParmVarDecl *p)
    {
        value_stack.push_back(getOrInsertLocal(p->id));
        return 1;
    }
    int visit(CompoundStmt *p)
    {
        int count = 0;
        for (auto it : *p)
        {
            count += it->accept(this);
        }
        return count;
    }

    int visit(StmtList *p)
    {
        return 1;
    }

    int visit(ReturnStmt *p)
    {
        if (p->flag) // 有返回值
        {
            p->expr_p->accept(this);
            builder_p->CreateStore(pop(), getOrInsertLocal("retval"));
        }
        builder_p->CreateBr(getNamedSucBB({"return"}));
        return 1;
    }

    int visit(DeclStmt *p)
    {
        for (auto decl : *p)
        {
            decl->accept(this);
        }
        return 1;
    }

    int visit(ExprStmt *p)
    {
        p->expr_p->accept(this);
        return 1;
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
        if (p->flag)
        {
            ElseBB = llvm::BasicBlock::Create(*context_p, "if.else", TheFunction);
            ElseBB->moveAfter(ThenBB);
            preBB = ElseBB;
        }
        llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*context_p, "if.end", TheFunction);
        MergeBB->moveAfter(preBB);

        p->expr_p->accept(this);
        llvm::Value *cond_v = pop();
        if (!cond_v)
            return -1;
        if (cond_v->getType()->isFloatTy() && p->expr_p->isLiteral)
        {
            int literalVal = (int)llvmConstToDouble(llvm::cast<llvm::Constant>(cond_v));
            cond_v = llvm::ConstantInt::get(*context_p, llvm::APInt(32, literalVal));
        }
        // Convert condition to a bool by comparing non-equal to 0.0.
        if (cond_v->getType() != llvm::Type::getInt1Ty(*context_p))
            cond_v = builder_p->CreateICmpNE(cond_v, llvm::ConstantInt::get(cond_v->getType(), 0), "tobool");

        if (p->flag)
            builder_p->CreateCondBr(cond_v, ThenBB, ElseBB);
        else
            builder_p->CreateCondBr(cond_v, ThenBB, MergeBB);

        // Emit then value.
        builder_p->SetInsertPoint(ThenBB);
        p->stmt_p_u->accept(this);
        int brCount = 0; // 当if和else都以br结尾时，删除ifend

        if (!hasBr(builder_p->GetInsertBlock()))
            builder_p->CreateBr(MergeBB);
        else
            brCount++;

        // Emit else block.
        if (p->flag)
        {
            builder_p->SetInsertPoint(ElseBB);
            p->stmt_p_d->accept(this);
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
        llvm::Function *func_v = builder_p->GetInsertBlock()->getParent();

        // LoopEnd作为名字的字串，从而在break stmt处可以识别
        llvm::BasicBlock *whileCond = llvm::BasicBlock::Create(*context_p, "while.cond", func_v);
        whileCond->moveAfter(builder_p->GetInsertBlock());
        llvm::BasicBlock *whileBody = llvm::BasicBlock::Create(*context_p, "while.body", func_v);
        whileBody->moveAfter(whileCond);
        llvm::BasicBlock *whileEnd = llvm::BasicBlock::Create(*context_p, "while.end", func_v);
        whileEnd->moveAfter(whileBody);

        builder_p->CreateBr(whileCond);

        // whilecond
        builder_p->SetInsertPoint(whileCond);
        p->expr_p->accept(this);
        llvm::Value *cond_v = pop();
        if (cond_v->getType()->isFloatTy() && p->expr_p->isLiteral)
        {
            int literalVal = (int)llvmConstToDouble(llvm::cast<llvm::Constant>(cond_v));
            cond_v = llvm::ConstantInt::get(*context_p, llvm::APInt(32, literalVal));
        }
        if (cond_v->getType() != llvm::Type::getInt1Ty(*context_p))
            cond_v = builder_p->CreateICmpNE(cond_v, llvm::ConstantInt::get(cond_v->getType(), 0), "tobool");

        builder_p->CreateCondBr(cond_v, whileBody, whileEnd);

        // whileBody
        builder_p->SetInsertPoint(whileBody);
        p->stmt_p->accept(this);

        if (builder_p->GetInsertBlock()->getTerminator() == nullptr)
            builder_p->CreateBr(whileCond);

        builder_p->SetInsertPoint(whileEnd);

        return 1;
    }

    int visit(DoStmt *p)
    {
        llvm::Function *func_v = builder_p->GetInsertBlock()->getParent();

        llvm::BasicBlock *doBodyBB = llvm::BasicBlock::Create(*context_p, "do.body", func_v);
        doBodyBB->moveAfter(builder_p->GetInsertBlock());
        llvm::BasicBlock *doCondBB = llvm::BasicBlock::Create(*context_p, "do.cond", func_v);
        doCondBB->moveAfter(doBodyBB);
        llvm::BasicBlock *doEndBB = llvm::BasicBlock::Create(*context_p, "do.end", func_v);
        doEndBB->moveAfter(doCondBB);

        builder_p->CreateBr(doBodyBB);
        builder_p->SetInsertPoint(doBodyBB);
        p->stmt_p->accept(this);

        if (builder_p->GetInsertBlock()->getTerminator() == nullptr)
            builder_p->CreateBr(doCondBB);

        builder_p->SetInsertPoint(doCondBB);
        p->expr_p->accept(this);
        llvm::Value *cond_v = pop();
        if (cond_v->getType()->isFloatTy() && p->expr_p->isLiteral)
        {
            int literalVal = (int)llvmConstToDouble(llvm::cast<llvm::Constant>(cond_v));
            cond_v = llvm::ConstantInt::get(*context_p, llvm::APInt(32, literalVal));
        }
        // Convert condition to a bool by comparing non-equal to 0.0.
        if (cond_v->getType() != llvm::Type::getInt1Ty(*context_p))
            cond_v = builder_p->CreateICmpNE(cond_v, llvm::ConstantInt::get(cond_v->getType(), 0), "tobool");

        builder_p->CreateCondBr(cond_v, doBodyBB, doEndBB);
        builder_p->SetInsertPoint(doEndBB);

        return 1;
    }

    int
    visit(BreakStmt *)
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
        llvm::Value *decl_ref_value;
        if (auto local_var = getOrInsertLocal(p->decl_p->id))
        {
            decl_ref_value = local_var;
        }
        else if (auto global_value = module_p->getNamedGlobal(p->decl_p->name))
        {
            decl_ref_value = global_value;
        }
        else if (auto func = module_p->getFunction(p->decl_p->name))
            decl_ref_value = func;
        else
            return -1;

        value_stack.push_back(decl_ref_value);
        return 1;
    }

    int visit(ExprList *)
    {
        return 1;
    }

    int visit(InitListExpr *p)
    {
        llvm::ArrayType *arrType = llvm::cast<llvm::ArrayType>(typeToLLVMType(&p->type));
        std::vector<llvm::Constant *> const_array_elems;
        for (auto iexpr : *p)
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
        p->expr_p->accept(this);
        bool is_global = false;
        if (!builder_p->GetInsertBlock())
            is_global = true;
        if (p->castKind == "LValueToRValue" && !is_global)
        {
            llvm::Value *tocast = pop();
            std::string name = "cast";
            if (auto dre = p->expr_p->dcast<DeclRefExpr>())
            {
                if (auto pvd = dre->decl_p->dcast<ParmVarDecl>())
                {
                    name = "parm.arr.cast";
                }
            }

            value_stack.push_back(builder_p->CreateLoad(typeToLLVMType(&p->type), tocast, name));
        }
        else if (p->castKind == "FloatingToIntegral")
        {
            if (is_global)
            {
                llvm::ConstantFP *tocast = llvm::cast<llvm::ConstantFP>(pop());
                float orgVal = tocast->getValue().convertToFloat();
                int castVal = int(orgVal);
                value_stack.push_back(llvm::ConstantInt::get(*context_p,
                                                             llvm::APInt(32, castVal, 10)));
            }
            else
            {
                value_stack.push_back(builder_p->CreateFPToSI(pop(), typeToLLVMType(&p->type)));
            }
        }
        else if (p->castKind == "IntegralToFloating")
        {
            if (is_global)
            {
                llvm::ConstantInt *tocast = llvm::cast<llvm::ConstantInt>(pop());
                int orgVal = tocast->getZExtValue();
                float castVal = float(orgVal);
                value_stack.push_back(llvm::ConstantFP::get(*context_p,
                                                            llvm::APFloat(castVal)));
            }
            else
            {
                value_stack.push_back(builder_p->CreateSIToFP(pop(), typeToLLVMType(&p->type)));
            }
        }
        else if (p->castKind == "FloatingCast")
        {
            llvm::ConstantFP *tocast = llvm::cast<llvm::ConstantFP>(pop());
            float castVal = tocast->getValue().convertToFloat();
            value_stack.push_back(llvm::ConstantFP::get(*context_p,
                                                        llvm::APFloat(castVal)));
        }
        else if (p->castKind == "IntegralCast")
        {
            if (p->expr_p->isLiteral)
            {
                llvm::ConstantInt *tocast = llvm::cast<llvm::ConstantInt>(pop());
                long long castVal = tocast->getZExtValue();
                int numBits = 32;
                if (p->type.base_type == Type::LONG)
                {
                    numBits = 64;
                }
                value_stack.push_back(llvm::ConstantInt::get(*context_p,
                                                             llvm::APInt(numBits, castVal)));
            }
            else
            {
                if (p->expr_p->type.base_type == Type::CHAR)
                    value_stack.push_back(builder_p->CreateSExt(pop(), typeToLLVMType(&p->type)));
                else
                    value_stack.push_back(builder_p->CreateTrunc(pop(), typeToLLVMType(&p->type)));
            }
        }
        return 1;
    }

    int visit(ArraySubscriptExpr *p)
    {
        p->expr_p_r->accept(this);
        llvm::Value *idx_value = pop();
        p->expr_p_l->accept(this);
        llvm::Value *arr_sub = pop();

        llvm::SmallVector<llvm::Value *, 3> indexes{llvm::ConstantInt::get(*context_p, llvm::APInt(32, 0))};
        llvm::Type *etype = arr_sub->getType()->getPointerElementType();
        if (arr_sub->getName().str().find("parm.arr.cast") != std::string::npos)
            indexes.pop_back();
        indexes.push_back(idx_value);
        llvm::Value *base = builder_p->CreateInBoundsGEP(etype, arr_sub, indexes);

        value_stack.push_back(base);
        return 1;
    }

    int visit(CallExpr *p)
    {
        p->expr_p->accept(this);
        llvm::Function *func = llvm::cast<llvm::Function>(pop());

        std::vector<llvm::Value *> args;
        for (auto argp : *p)
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
        value_stack.push_back(builder_p->CreateCall(func, args));
        return 1;
    }

    int visit(ParenExpr *p)
    {
        p->expr_p->accept(this);
        return 1;
    }

    int visit(BinaryOperator *p)
    {
        bool is_const = false;
        bool LHS_is_fd = false;
        double GLHSV, GRHSV, GHSV;
        llvm::Value *LHS, *RHS;
        // 全局变量直接计算值
        if (!builder_p->GetInsertBlock() || p->isLiteral)
        {
            p->expr_p_l->accept(this);
            GLHSV = llvmConstToDouble(llvm::cast<llvm::Constant>(pop()));
            p->expr_p_r->accept(this);
            GRHSV = llvmConstToDouble(llvm::cast<llvm::Constant>(pop()));
            is_const = true;
        }

        // C语言要求进行短路处理
        if (p->opcode == "&&" || p->opcode == "||")
        {
            if (is_const)
            {
                if (p->opcode == "&&")
                    GHSV = GLHSV && GRHSV;
                else
                    GHSV = GLHSV || GRHSV;
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
                p->expr_p_l->accept(this);
                LHS = pop();
                // 在C语言的语义上int可以作为LHS，但是在IR的语义上需要转换。因此此处转换不能在IMplicitcast进行。
                if (LHS->getType() != llvm::Type::getInt1Ty(*context_p))
                    LHS = builder_p->CreateICmpNE(LHS, llvm::ConstantInt::get(*context_p, llvm::APInt(32, "0", 10)));

                builder_p->CreateCondBr(LHS, getNamedSucBB(lhsTrueSucBBName), getNamedSucBB(lhsFalseSucBBName));
                builder_p->SetInsertPoint(RHSBB);
                p->expr_p_r->accept(this);
                RHS = pop();
                value_stack.push_back(RHS);
            }
        }
        else
        {
            if (!is_const)
            {
                p->expr_p_l->accept(this);
                LHS = pop();
                p->expr_p_r->accept(this);
                RHS = pop();
                LHS_is_fd = LHS->getType()->isFloatTy() || LHS->getType()->isDoubleTy();
            }

            if (p->opcode == "=")
                value_stack.push_back(builder_p->CreateStore(RHS, LHS));
            else if (p->opcode == "+")
            {
                if (is_const)
                {
                    if (p->type.base_type == Type::INT || p->type.base_type == Type::UNSIGNED_INT)
                        GHSV = (int)GLHSV + (int)GRHSV;
                    else
                    {
                        GHSV = GLHSV + GRHSV;
                    }
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
                if (is_const)
                {
                    GHSV = GLHSV - GRHSV;
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
                if (is_const)
                    GHSV = GLHSV * GRHSV;
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
                if (is_const)
                {
                    GHSV = GLHSV / GRHSV;
                }
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
                if (is_const)
                    GHSV = (int)GLHSV % (int)GRHSV;
                else
                    value_stack.push_back(builder_p->CreateSRem(LHS, RHS));
            }
            else if (p->opcode == ">")
            {
                if (is_const)
                    GHSV = GLHSV > GRHSV;
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
                if (is_const)
                    GHSV = GLHSV < GRHSV;
                else
                {
                    if (LHS_is_fd)
                        value_stack.push_back(builder_p->CreateFCmpULT(LHS, RHS));
                    else
                        value_stack.push_back(builder_p->CreateICmpSLT(LHS, RHS));
                }
            }
            else
            {
                if (RHS->getType() != LHS->getType())
                {
                    if (LHS->getType() == llvm::Type::getInt1Ty(*context_p))
                        RHS = builder_p->CreateICmpNE(RHS, llvm::ConstantInt::get(RHS->getType(), 0));
                    else if (RHS->getType() == llvm::Type::getInt1Ty(*context_p))
                        LHS = builder_p->CreateICmpNE(LHS, llvm::ConstantInt::get(LHS->getType(), 0));
                }

                if (p->opcode == ">=")
                {
                    if (is_const)
                        GHSV = GLHSV >= GRHSV;
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
                    if (is_const)
                        GHSV = GLHSV <= GRHSV;
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
                    if (is_const)
                        GHSV = GLHSV == GRHSV;
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
                    if (is_const)
                        GHSV = GLHSV != GRHSV;
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

        if (is_const)
        {
            if (p->type.base_type == Type::FLOAT)
            {
                value_stack.push_back(llvm::ConstantFP::get(*context_p,
                                                            llvm::APFloat(float(GHSV))));
            }
            else if (p->type.base_type == Type::DOUBLE)
            {
                value_stack.push_back(llvm::ConstantFP::get(*context_p,
                                                            llvm::APFloat(double(GHSV))));
            }
            else
            {
                value_stack.push_back(llvm::ConstantInt::get(*context_p,
                                                             llvm::APInt(32, int(GHSV))));
            }
        }
        return 1;
    }

    int
    visit(UnaryOperator *p)
    {
        p->expr_p->accept(this);
        llvm::Value *HS;
        double GHSV;
        bool is_const = false;
        if (!builder_p->GetInsertBlock() || p->isLiteral)
        {
            is_const = true;
            GHSV = llvmConstToDouble(llvm::cast<llvm::Constant>(pop()));
        }
        else
            HS = pop();

        if (p->opcode == "!")
        {
            if (is_const)
            {
                GHSV = !(int)GHSV;
            }
            else
            {
                if (HS->getType() != llvm::Type::getInt1Ty(*context_p))
                    HS = builder_p->CreateICmpNE(HS, llvm::ConstantInt::get(HS->getType(), 0));
                value_stack.push_back(builder_p->CreateXor(HS, llvm::ConstantInt::get(HS->getType(), 1)));
            }
        }
        else if (p->opcode == "-")
        {
            if (is_const)
            {

                GHSV = -GHSV;
            }
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
            if (is_const)
            {
                GHSV = +GHSV;
            }
            else
                value_stack.push_back(HS);
        }

        if (is_const)
        {
            if (p->type.base_type == Type::FLOAT)
            {
                GHSV = float(GHSV);
                value_stack.push_back(llvm::ConstantFP::get(*context_p,
                                                            llvm::APFloat(GHSV)));
            }
            else if (p->type.base_type == Type::DOUBLE)
            {
                value_stack.push_back(llvm::ConstantFP::get(*context_p,
                                                            llvm::APFloat(GHSV)));
            }
            else
            {
                value_stack.push_back(llvm::ConstantInt::get(*context_p,
                                                             llvm::APInt(32, int(GHSV))));
            }
        }
        return 1;
    }

    int visit(IntegerLiteral *p)
    {
        value_stack.push_back(llvm::ConstantInt::get(*context_p,
                                                     llvm::APInt(32, p->str_value, 10)));
        return State::LITERAL;
    }

    int visit(StringLiteral *p)
    {
        llvm::Constant *strVal = llvm::ConstantDataArray::getString(*context_p,
                                                                    p->serial);
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
        value_stack.push_back(llvm::ConstantFP::get(*context_p,
                                                    llvm::APFloat(float(p->value))));
        return State::LITERAL;
    }

    int visit(array_filler *)
    {
        return 1;
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

    double llvmConstToDouble(llvm::Constant *Val)
    {

        if (auto glob = module_p->getGlobalVariable(Val->getName()))
            Val = glob->getInitializer();

        if (Val->getType()->isFloatTy() || Val->getType()->isDoubleTy())
        {
            return llvm::cast<llvm::ConstantFP>(Val)->getValue().convertToDouble();
        }
        else if (auto constInt = llvm::cast<llvm::ConstantInt>(Val))
        {
            return double(constInt->getZExtValue());
        }
        return 0;
    }

    void buildArray(Expr *p, llvm::Value *base)
    {
        if (auto init_list_expr_p = p->dcast<InitListExpr>())
        {
            int idx = 0;
            for (auto sub_expr : *init_list_expr_p)
            {
                llvm::Value *idx_value = llvm::ConstantInt::get(*context_p, llvm::APInt(32, idx++));
                llvm::Value *next_base = builder_p->CreateInBoundsGEP(typeToLLVMType(&p->type),
                                                                      base,
                                                                      {llvm::ConstantInt::get(*context_p, llvm::APInt(32, 0)), idx_value});
                buildArray(sub_expr, next_base);
            }
        }
        else
        {
            p->accept(this);
            builder_p->CreateStore(pop(), base);
        }
    }

    // 函数变量初始化
    void buildVar(FunctionDecl *p, llvm::Function *func_value)
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
        auto arg_iter = func_value->arg_begin();
        for (int parmidx = 0; parmidx < p->size(); parmidx++)
        {
            ParmVarDecl *parm = p->at(parmidx);
            llvm::AllocaInst *allo = builder_p->CreateAlloca(typeToLLVMType(&parm->type),
                                                             0,
                                                             parm->name);
            getOrInsertLocal(parm->id, allo);
            builder_p->CreateStore(&(*arg_iter), allo);
            arg_iter = std::next(arg_iter);
        }

        // 为函数内所有位置的局部变量分配空间
        for (auto localvar : p->inside_vars)
        {
            llvm::AllocaInst *allo = builder_p->CreateAlloca(typeToLLVMType(&localvar->type), nullptr, localvar->name);
            getOrInsertLocal(localvar->id, allo);
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
        llvm::Function *func_v = builder_p->GetInsertBlock()->getParent();
        llvm::StringRef curBBName = builder_p->GetInsertBlock()->getName();
        bool curBBVisited = false;
        for (llvm::BasicBlock &BB : *func_v)
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
};
