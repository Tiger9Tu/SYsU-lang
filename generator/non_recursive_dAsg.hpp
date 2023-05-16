#include "asg.hpp"
#include <llvm/Support/JSON.h>
#include <string>
#include <unordered_map>
#include "deserializeAsg.hpp"

Stmt *nonRecursiveDeserializeJson(llvm::json::Object *O)
{
    Stmt *transEntry = Mgr::g.make<TranslationUnitDecl>();
    std::vector<Stmt *> stmtStack = {transEntry};
    std::vector<llvm::json::Object *> OStack = {O};

    while (!OStack.empty())
    {
        auto curStmt = stmtStack.back();
        stmtStack.pop_back();
        auto curO = OStack.back();
        OStack.pop_back();

        llvm::json::Array *inner = curO->getArray("inner");
        llvm::json::Array *array_filler = curO->getArray("array_filler");
        if (inner || inner)
        {
            for (int i = inner->size() - 1; i >= 0; i--)
            {
                auto son = (*inner)[i];
                if (son.getAsObject()->getString("kind")->str() == "TypedefDecl") // solve builtin
                    continue;
                auto nextStmt = makeObjAndConnect(son.getAsObject(), curStmt);
                stmtStack.push_back(nextStmt);
                OStack.push_back(son.getAsObject());
            }
        }
    }

    return transEntry;
}

Stmt *makeObjAndConnect(const llvm::json::Object *O, Stmt *father)
{
    // get id from my parser or clang
    static int cur_id;
    llvm::StringRef id;
    if (auto clang_id = O->getString("id"))
        id = *clang_id;
    else
        id = llvm::StringRef(std::to_string(cur_id++));

    auto kind = *(O->getString("kind"));
    auto value = *(O->getString("value"));
    auto name_p = (O->getString("name"));
    if (auto built_in_p = O->getString("storageClass"))
        return;

    Type type;
    if (O->getObject("type"))
        type = getType(*(O->getObject("type")->getString("qualType")));

    if (auto array_filler = O->getArray("array_filler"))
    {

        for (int i = 0; i < array_filler->size(); i++)
        {
            // array_filler和inner同样处理
            inner_sons.push_back(deserializeJson((*array_filler)[i].getAsObject(), _father));
        }
    }

    if (kind == "TranslationUnitDecl")
    {
        for (auto it : inner_sons)
            tmp_trans->externalDecls.push_back(it->dcast<Decl>());
        cur = tmp_trans;
    }
    else if (kind == "VarDecl")
    {
        VarDecl *tmp = Mgr::g.make<VarDecl>();
        tmp->name = name_p->str();
        if (auto ul = father->dcast<TranslationUnitDecl>())
            tmp->is_global = true;
        else if (auto func = father->dcast<FunctionDecl>())
            func->localVars.push_back(tmp); // 将变量存在FunctionDecl内

        sym_table.insert({id.str(), tmp});
        if (inner_sons.size() > 0)
            tmp->initExp = inner_sons[0]->dcast<Expr>();
        tmp->type = type;
        tmp->isSingleConst = type.is_const && (type.dims.size() == 0);
        cur = tmp;
    }
    else if (kind == "FunctionDecl")
    {
        tmp_func->name = name_p->str();
        for (auto it : inner_sons)
        {
            if (auto son = it->dcast<CompoundStmt>())
                tmp_func->compoundStmt = son;
            else if (auto son = it->dcast<ParmVarDecl>())
                tmp_func->parmVars.push_back(son);
            else
                break;
        }
        tmp_func->type = type;
        cur = tmp_func;

        for (auto pa : tmp_func->parmVars) // 将参数地址放入type
            tmp_func->type.parm_vars.push_back(pa);
    }
    else if (kind == "ParmVarDecl")
    {
        ParmVarDecl *tmp = Mgr::g.make<ParmVarDecl>();
        tmp->name = name_p->str();
        tmp->type = type;
        sym_table.insert({id.str(), tmp});
        cur = tmp;
    }
    else if (kind == "CompoundStmt")
    {
        CompoundStmt *tmp = Mgr::g.make<CompoundStmt>();
        tmp->stmts = inner_sons;
        cur = tmp;
    }
    else if (kind == "ReturnStmt")
    {
        ReturnStmt *tmp = Mgr::g.make<ReturnStmt>();
        if (inner_sons.size() > 0)
            tmp->returnExp = inner_sons[0]->dcast<Expr>();
        cur = tmp;
    }
    else if (kind == "DeclStmt")
    {
        DeclStmt *tmp = Mgr::g.make<DeclStmt>();
        for (auto p : inner_sons)
            tmp->decls.push_back(p->dcast<Decl>());
        cur = tmp;
    }
    else if (kind == "ExprStmt")
    {
        ExprStmt *tmp = Mgr::g.make<ExprStmt>();
        tmp->exp = inner_sons[0]->dcast<Expr>();
        cur = tmp;
    }
    else if (kind == "IfStmt")
    {
        IfStmt *tmp = Mgr::g.make<IfStmt>();
        tmp->condExp = inner_sons[0]->dcast<Expr>();
        tmp->thenStmt = inner_sons[1];
        if (inner_sons.size() > 2)
            tmp->elseStmt = inner_sons[2];
        cur = tmp;
    }
    else if (kind == "WhileStmt")
    {
        WhileStmt *tmp = Mgr::g.make<WhileStmt>();
        tmp->condExp = inner_sons[0]->dcast<Expr>();
        tmp->whileBodyStmt = inner_sons[1];
        cur = tmp;
    }
    else if (kind == "DoStmt")
    {
        DoStmt *tmp = Mgr::g.make<DoStmt>();
        tmp->doBodyStmt = inner_sons[0]->dcast<Stmt>();
        tmp->condExp = inner_sons[1]->dcast<Expr>();
        cur = tmp;
    }
    else if (kind == "BreakStmt")
    {
        BreakStmt *tmp = Mgr::g.make<BreakStmt>();
        cur = tmp;
    }
    else if (kind == "ContinueStmt")
    {
        ContinueStmt *tmp = Mgr::g.make<ContinueStmt>();
        cur = tmp;
    }
    else if (kind == "NullStmt")
    {
        NullStmt *tmp = Mgr::g.make<NullStmt>();
        cur = tmp;
    }
    else if (kind == "DeclRefExpr")
    {
        DeclRefExpr *tmp = Mgr::g.make<DeclRefExpr>();
        std::string id_key = O->getObject("referencedDecl")->getString("id")->str();
        tmp->referencedDecl = sym_table[id_key]->dcast<Decl>();
        tmp->referencedDecl->isUsed = true; // both function ref and var ref
        tmp->isSingleConst = tmp->referencedDecl->isSingleConst;
        tmp->type = type;
        cur = tmp;
    }
    else if (kind == "InitListExpr")
    {
        InitListExpr *tmp = Mgr::g.make<InitListExpr>();
        tmp->type = type;
        for (auto it : inner_sons)
        {
            if (it) // 忽略ImplicitValueInitExpr
                tmp->initExps.push_back(it->dcast<Expr>());
        }
        tmp->isSingleConst = false;
        cur = tmp;
    }
    else if (kind == "ImplicitCastExpr")
    {
        ImplicitCastExpr *tmp = Mgr::g.make<ImplicitCastExpr>();
        tmp->type = type;
        tmp->castKind = O->getString("castKind")->str();
        tmp->toCastExp = inner_sons[0]->dcast<Expr>();
        tmp->isSingleConst = tmp->toCastExp->isSingleConst;
        cur = tmp;
    }
    else if (kind == "ArraySubscriptExpr")
    {
        ArraySubscriptExpr *tmp = Mgr::g.make<ArraySubscriptExpr>();
        tmp->type = type;
        tmp->arrayPointerExp = inner_sons[0]->dcast<Expr>();
        tmp->indexExp = inner_sons[1]->dcast<Expr>();
        cur = tmp;
    }
    else if (kind == "CallExpr")
    {
        CallExpr *tmp = Mgr::g.make<CallExpr>();
        tmp->type = type;
        tmp->functionPointerExp = inner_sons[0]->dcast<Expr>();
        inner_sons.erase(inner_sons.begin());
        for (auto it : inner_sons)
            tmp->parmExps.push_back(it->dcast<Expr>());
        cur = tmp;
    }
    else if (kind == "ParenExpr")
    {
        ParenExpr *tmp = Mgr::g.make<ParenExpr>();
        tmp->exp = inner_sons[0]->dcast<Expr>();
        tmp->isSingleConst = tmp->exp->isSingleConst;
        tmp->type = type;
        cur = tmp;
    }
    else if (kind == "BinaryOperator")
    {
        BinaryOperator *tmp = Mgr::g.make<BinaryOperator>();
        tmp->lExp = inner_sons[0]->dcast<Expr>();
        tmp->rExp = inner_sons[1]->dcast<Expr>();
        tmp->opcode = O->getString("opcode")->str();
        tmp->isSingleConst = tmp->lExp->isSingleConst && tmp->rExp->isSingleConst;
        tmp->type = type;
        cur = tmp;
    }
    else if (kind == "UnaryOperator")
    {
        UnaryOperator *tmp = Mgr::g.make<UnaryOperator>();
        tmp->exp = inner_sons[0]->dcast<Expr>();
        tmp->opcode = O->getString("opcode")->str();
        tmp->type = type;
        tmp->isSingleConst = tmp->exp->isSingleConst;
        cur = tmp;
    }
    else if (kind == "StringLiteral")
    {
        StringLiteral *tmp = Mgr::g.make<StringLiteral>();
        tmp->type = type;
        tmp->isSingleConst = false;
        tmp->strVal = getStrVal(value.str(), (type.dims[0] - 1));
        cur = tmp;
    }
    else if (kind == "FloatingLiteral")
    {
        FloatingLiteral *tmp = Mgr::g.make<FloatingLiteral>();
        tmp->isSingleConst = true;
        value.getAsDouble(tmp->V.doubleV);
        tmp->type = type;
        cur = tmp;
    }
    else if (kind == "IntegerLiteral")
    {
        IntegerLiteral *tmp = Mgr::g.make<IntegerLiteral>();
        tmp->type = type;
        tmp->isSingleConst = true;

        llvm::APInt tmpV(128, value, 10);
        double buffer = tmpV.getSExtValue();
        assignV(buffer, &tmp->V, tmp->type.base_type);
        cur = tmp;
    }
    else
    {
        return nullptr; //"ImplicitValueInitExpr"
    }

    cur->id = id.str();
    return cur;
}
