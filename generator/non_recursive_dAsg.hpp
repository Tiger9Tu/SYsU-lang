#include "asg.hpp"
#include <llvm/Support/JSON.h>
#include <string>
#include <unordered_map>
#include "deserializeAsg.hpp"

Stmt *makeStmt(llvm::json::Object *obj);

Stmt *nonRecursiveDeserializeJson(llvm::json::Object *O)
{
    Stmt *transEntry = Mgr::g.make<TranslationUnitDecl>();
    std::vector<Stmt *> SStack = {transEntry};
    std::vector<llvm::json::Object *> OStack = {O};

    std::unordered_map<std::string, Stmt *> sym_table;
    FunctionDecl *curFunc = nullptr;

    while (!OStack.empty())
    {
        auto curObj = OStack.back();
        OStack.pop_back();
        Stmt *curStmt = SStack.back();
        SStack.pop_back();

        auto value = *(curObj->getString("value"));
        auto name_p = (curObj->getString("name"));
        if (auto built_in_p = curObj->getString("storageClass"))
            continue;

        Type type;
        if (curObj->getObject("type"))
            type = getType(*(curObj->getObject("type")->getString("qualType")));

        curStmt->id = (curObj->getString("id"))->str();

        auto kind = curObj->getString("kind")->str();
        llvm::errs() << kind << "\n";
        if (kind == "TranslationUnitDecl")
        {
            TranslationUnitDecl *curNode = curStmt->dcast<TranslationUnitDecl>();
            if (auto inner = curObj->getArray("inner"))
            {
                for (int i = inner->size() - 1; i >= 0; i--)
                {
                    if ((*inner)[i].getAsObject()->getString("kind")->str() == "TypedefDecl")
                    {
                        break;
                    }
                    Stmt *sonStmt = makeStmt((*inner)[i].getAsObject());
                    Decl *externalDecl = sonStmt->dcast<Decl>();
                    externalDecl->isGlobal = true;
                    curNode->externalDecls.push_back(externalDecl);

                    OStack.push_back((*inner)[i].getAsObject());
                    SStack.push_back(sonStmt);
                }
            }
        }
        else if (kind == "VarDecl")
        {
            VarDecl *curNode = curStmt->dcast<VarDecl>();
            curNode->name = name_p->str();
            curNode->type = type;
            curNode->isSingleConst = type.is_const && (type.dims.size() == 0);

            sym_table.insert({curNode->id, curStmt});

            if (auto inner = curObj->getArray("inner"))
            {
                Stmt *sonStmt = makeStmt((*inner)[0].getAsObject());
                curNode->initExp = sonStmt->dcast<Expr>();

                SStack.push_back(sonStmt);
                OStack.push_back((*inner)[0].getAsObject());
            }

            if (curFunc)
                curFunc->localVars.push_back(curNode); // 将变量存在FunctionDecl内
        }
        else if (kind == "FunctionDecl")
        {
            FunctionDecl *curNode = curStmt->dcast<FunctionDecl>();
            curFunc = curNode;
            curNode->name = name_p->str();
            curNode->type = type;

            if (auto inner = curObj->getArray("inner"))
            {

                for (int i = inner->size() - 1; i >= 0; i--)
                {
                    Stmt *sonStmt = makeStmt((*inner)[i].getAsObject());
                    if (auto comStmt = sonStmt->dcast<CompoundStmt>())
                        curNode->compoundStmt = comStmt;
                    else if (auto parmDecl = sonStmt->dcast<ParmVarDecl>())
                        curNode->parmVars.push_back(parmDecl);
                    else
                        break;

                    SStack.push_back(sonStmt);
                    OStack.push_back((*inner)[i].getAsObject());
                }
            }

            for (auto pa : curNode->parmVars) // 将参数地址放入type
                curNode->type.parm_vars.push_back(pa);
        }
        else if (kind == "ParmVarDecl")
        {
            ParmVarDecl *curNode = curStmt->dcast<ParmVarDecl>();
            curNode->name = name_p->str();
            curNode->type = type;
            sym_table.insert({curNode->id, curStmt});
        }
        else if (kind == "CompoundStmt")
        {
            CompoundStmt *curNode = curStmt->dcast<CompoundStmt>();
            if (auto inner = curObj->getArray("inner"))
            {
                for (int i = inner->size() - 1; i >= 0; i--)
                {
                    Stmt *sonStmt = makeStmt((*inner)[i].getAsObject());
                    curNode->stmts.push_back(sonStmt);

                    SStack.push_back(sonStmt);
                    OStack.push_back((*inner)[i].getAsObject());
                }
            }
        }
        else if (kind == "ReturnStmt")
        {
            ReturnStmt *curNode = curStmt->dcast<ReturnStmt>();
            if (auto inner = curObj->getArray("inner"))
            {
                Stmt *sonStmt = makeStmt((*inner)[0].getAsObject());
                curNode->returnExp = sonStmt->dcast<Expr>();

                SStack.push_back(sonStmt);
                OStack.push_back((*inner)[0].getAsObject());
            }
        }
        else if (kind == "DeclStmt")
        {
            DeclStmt *curNode = curStmt->dcast<DeclStmt>();
            if (auto inner = curObj->getArray("inner"))
            {
                for (int i = inner->size() - 1; i >= 0; i--)
                {
                    Stmt *sonStmt = makeStmt((*inner)[i].getAsObject());

                    curNode->decls.push_back(sonStmt->dcast<Decl>());
                    SStack.push_back(sonStmt);
                    OStack.push_back((*inner)[i].getAsObject());
                }
            }
        }
        else if (kind == "ExprStmt")
        {
            ExprStmt *curNode = curStmt->dcast<ExprStmt>();
            if (auto inner = curObj->getArray("inner"))
            {
                Stmt *sonStmt = makeStmt((*inner)[0].getAsObject());
                curNode->exp = sonStmt->dcast<Expr>();

                SStack.push_back(sonStmt);
                OStack.push_back((*inner)[0].getAsObject());
            }
        }
        else if (kind == "IfStmt")
        {
            IfStmt *curNode = curStmt->dcast<IfStmt>();
            if (auto inner = curObj->getArray("inner"))
            {
                for (int i = inner->size() - 1; i >= 0; i--)
                {
                    Stmt *sonStmt = makeStmt((*inner)[i].getAsObject());
                    if (i == 2)
                        curNode->elseStmt = sonStmt;
                    else if (i == 1)
                        curNode->thenStmt = sonStmt;
                    else
                        curNode->condExp = sonStmt->dcast<Expr>();

                    SStack.push_back(sonStmt);
                    OStack.push_back((*inner)[i].getAsObject());
                }
            }
        }
        else if (kind == "WhileStmt")
        {
            WhileStmt *curNode = curStmt->dcast<WhileStmt>();
            if (auto inner = curObj->getArray("inner"))
            {
                for (int i = inner->size() - 1; i >= 0; i--)
                {
                    Stmt *sonStmt = makeStmt((*inner)[i].getAsObject());
                    if (i == 1)
                        curNode->whileBodyStmt = sonStmt;
                    else
                        curNode->condExp = sonStmt->dcast<Expr>();

                    OStack.push_back((*inner)[i].getAsObject());
                    SStack.push_back(sonStmt);
                }
            }
        }
        else if (kind == "DoStmt")
        {
            DoStmt *curNode = curStmt->dcast<DoStmt>();
            if (auto inner = curObj->getArray("inner"))
            {
                for (int i = inner->size() - 1; i >= 0; i--)
                {
                    Stmt *sonStmt = makeStmt((*inner)[i].getAsObject());
                    if (i == 1)
                        curNode->condExp = sonStmt->dcast<Expr>();
                    else
                        curNode->doBodyStmt = sonStmt;

                    OStack.push_back((*inner)[i].getAsObject());
                    SStack.push_back(sonStmt);
                }
            }
        }
        else if (kind == "DeclRefExpr")
        {
            DeclRefExpr *curNode = curStmt->dcast<DeclRefExpr>();
            std::string id_key = curObj->getObject("referencedDecl")->getString("id")->str();
            curNode->referencedDecl = sym_table[id_key]->dcast<Decl>();
            curNode->referencedDecl->isUsed = true; // both function ref and var ref
            curNode->isSingleConst = curNode->referencedDecl->isSingleConst;
            curNode->type = type;
        }
        else if (kind == "InitListExpr")
        {
            InitListExpr *curNode = curStmt->dcast<InitListExpr>();
            curNode->type = type;
            curNode->isSingleConst = false;

            if (auto inner = curObj->getArray("inner"))
            {
                for (int i = inner->size() - 1; i >= 0; i--)
                {
                    Stmt *sonStmt = makeStmt((*inner)[i].getAsObject());
                    curNode->initExps.push_back(sonStmt->dcast<Expr>());

                    OStack.push_back((*inner)[i].getAsObject());
                    SStack.push_back(sonStmt);
                }
            }

            if (auto array_filler = curObj->getArray("array_filler"))
            {
                for (int i = array_filler->size() - 1; i >= 0; i--)
                {
                    Stmt *sonStmt = makeStmt((*array_filler)[i].getAsObject());
                    curNode->initExps.push_back(sonStmt->dcast<Expr>());

                    OStack.push_back((*array_filler)[i].getAsObject());
                    SStack.push_back(sonStmt);
                }
            }
        }
        else if (kind == "ImplicitCastExpr")
        {
            ImplicitCastExpr *curNode = curStmt->dcast<ImplicitCastExpr>();
            curNode->type = type;
            curNode->castKind = curObj->getString("castKind")->str();

            if (auto inner = curObj->getArray("inner"))
            {
                Stmt *sonStmt = makeStmt((*inner)[0].getAsObject());
                curNode->toCastExp = sonStmt->dcast<Expr>();

                SStack.push_back(sonStmt);
                OStack.push_back((*inner)[0].getAsObject());
            }
            curNode->isSingleConst = curNode->toCastExp->isSingleConst;
        }
        else if (kind == "ArraySubscriptExpr")
        {
            ArraySubscriptExpr *curNode = curStmt->dcast<ArraySubscriptExpr>();
            curNode->type = type;

            if (auto inner = curObj->getArray("inner"))
            {
                for (int i = inner->size() - 1; i >= 0; i--)
                {
                    Stmt *sonStmt = makeStmt((*inner)[i].getAsObject());
                    if (i == 1)
                        curNode->indexExp = sonStmt->dcast<Expr>();
                    else
                        curNode->arrayPointerExp = sonStmt->dcast<Expr>();

                    OStack.push_back((*inner)[i].getAsObject());
                    SStack.push_back(sonStmt);
                }
            }
        }
        else if (kind == "CallExpr")
        {
            CallExpr *curNode = curStmt->dcast<CallExpr>();
            curNode->type = type;

            auto inner = curObj->getArray("inner");
            for (int i = inner->size() - 1; i >= 0; i--)
            {
                Stmt *sonStmt = makeStmt((*inner)[i].getAsObject());
                if (i == 0)
                    curNode->functionPointerExp = sonStmt->dcast<Expr>();
                else
                    curNode->parmExps.push_back(sonStmt->dcast<Expr>());

                OStack.push_back((*inner)[i].getAsObject());
                SStack.push_back(sonStmt);
            }
        }
        else if (kind == "ParenExpr")
        {
            ParenExpr *curNode = curStmt->dcast<ParenExpr>();
            curNode->isSingleConst = curNode->exp->isSingleConst;
            curNode->type = type;
            if (auto inner = curObj->getArray("inner"))
            {
                Stmt *sonStmt = makeStmt((*inner)[0].getAsObject());

                curNode->exp = sonStmt->dcast<Expr>();
                OStack.push_back((*inner)[0].getAsObject());
                SStack.push_back(sonStmt);
            }
        }
        else if (kind == "BinaryOperator")
        {
            BinaryOperator *curNode = curStmt->dcast<BinaryOperator>();
            curNode->opcode = curObj->getString("opcode")->str();
            curNode->type = type;

            if (auto inner = curObj->getArray("inner"))
            {
                for (int i = inner->size() - 1; i >= 0; i--)
                {
                    Stmt *sonStmt = makeStmt((*inner)[i].getAsObject());
                    if (i == 1)
                        curNode->rExp = sonStmt->dcast<Expr>();
                    else
                        curNode->lExp = sonStmt->dcast<Expr>();

                    OStack.push_back((*inner)[i].getAsObject());
                    SStack.push_back(sonStmt);
                }
            }

            // curNode->isSingleConst = curNode->lExp->isSingleConst && curNode->rExp->isSingleConst;
        }
        else if (kind == "UnaryOperator")
        {
            UnaryOperator *curNode = curStmt->dcast<UnaryOperator>();
            curNode->opcode = curObj->getString("opcode")->str();
            curNode->type = type;

            if (auto inner = curObj->getArray("inner"))
            {
                Stmt *sonStmt = makeStmt((*inner)[0].getAsObject());
                curNode->exp = sonStmt->dcast<Expr>();
                OStack.push_back((*inner)[0].getAsObject());
                SStack.push_back(sonStmt);
            }
            // curNode->isSingleConst = curNode->exp->isSingleConst;
        }
        else if (kind == "StringLiteral")
        {
            StringLiteral *curNode = curStmt->dcast<StringLiteral>();
            curNode->type = type;
            curNode->isSingleConst = false;
            curNode->strVal = getStrVal(value.str(), (type.dims[0] - 1));
        }
        else if (kind == "FloatingLiteral")
        {
            FloatingLiteral *curNode = curStmt->dcast<FloatingLiteral>();
            curNode->isSingleConst = true;
            value.getAsDouble(curNode->V.doubleV);
            curNode->type = type;
        }
        else if (kind == "IntegerLiteral")
        {
            IntegerLiteral *curNode = curStmt->dcast<IntegerLiteral>();
            curNode->type = type;
            curNode->isSingleConst = true;

            llvm::APInt tmpV(128, value, 10);
            double buffer = tmpV.getSExtValue();
            assignV(buffer, &curNode->V, curNode->type.base_type);
        }
    }

    return transEntry;
}

Stmt *makeStmt(llvm::json::Object *obj)
{
    std::string kind = obj->getString("kind")->str();
    if (kind == "VarDecl")
        return Mgr::g.make<VarDecl>();
    else if (kind == "FunctionDecl")
        return Mgr::g.make<FunctionDecl>();
    else if (kind == "ParmVarDecl")
        return Mgr::g.make<ParmVarDecl>();
    else if (kind == "CompoundStmt")
        return Mgr::g.make<CompoundStmt>();
    else if (kind == "ReturnStmt")
        return Mgr::g.make<ReturnStmt>();
    else if (kind == "DeclStmt")
        return Mgr::g.make<DeclStmt>();
    else if (kind == "ExprStmt")
        return Mgr::g.make<ExprStmt>();
    else if (kind == "IfStmt")
        return Mgr::g.make<IfStmt>();
    else if (kind == "WhileStmt")
        return Mgr::g.make<WhileStmt>();
    else if (kind == "DoStmt")
        return Mgr::g.make<DoStmt>();
    else if (kind == "BreakStmt")
        return Mgr::g.make<BreakStmt>();
    else if (kind == "ContinueStmt")
        return Mgr::g.make<ContinueStmt>();
    else if (kind == "NullStmt")
        return Mgr::g.make<NullStmt>();
    else if (kind == "DeclRefExpr")
        return Mgr::g.make<DeclRefExpr>();
    else if (kind == "InitListExpr")
        return Mgr::g.make<InitListExpr>();
    else if (kind == "ImplicitCastExpr")
        return Mgr::g.make<ImplicitCastExpr>();
    else if (kind == "ArraySubscriptExpr")
        return Mgr::g.make<ArraySubscriptExpr>();
    else if (kind == "CallExpr")
        return Mgr::g.make<CallExpr>();
    else if (kind == "ParenExpr")
        return Mgr::g.make<ParenExpr>();
    else if (kind == "BinaryOperator")
        return Mgr::g.make<BinaryOperator>();
    else if (kind == "UnaryOperator")
        return Mgr::g.make<UnaryOperator>();
    else if (kind == "StringLiteral")
        return Mgr::g.make<StringLiteral>();
    else if (kind == "FloatingLiteral")
        return Mgr::g.make<FloatingLiteral>();
    else if (kind == "IntegerLiteral")
        return Mgr::g.make<IntegerLiteral>();
    else
        return nullptr; //"ImplicitValueInitExpr"
}
