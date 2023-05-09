#include "asg.hpp"
#include <llvm/Support/JSON.h>
#include <string>
#include <unordered_map>
// extern Scope , translationUnitDecl
extern Mgr g;

std::unordered_map<std::string, Obj *> sym_table;
ScopeManager *scope_manager_p; // stupid global variabel
Type getType(const llvm::StringRef qualType);

Obj *deserializeJson(const llvm::json::Object *O, Obj *father = std::nullptr_t())
{
    Obj *cur;
    auto kind = *(O->getString("kind"));
    auto value = *(O->getString("value"));
    auto id = *(O->getString("id")); // for symbol table key
    auto name_p = (O->getString("name"));
    if (auto built_in_p = O->getString("storageClass"))
        return nullptr;

    Type type;
    if (O->getObject("type"))
        type = getType(*(O->getObject("type")->getString("qualType")));
    // 提前声明从而处理继承属性 - 递归以及全局变量的判定
    Obj *_father = father;
    FunctionDecl *tmp_func;
    TranslationUnitDecl *tmp_trans;
    if (kind == "FunctionDecl")
    {
        tmp_func = Mgr::g.make<FunctionDecl>();
        sym_table.insert({id.str(), tmp_func});
        _father = tmp_func;
    }
    else if (kind == "TranslationUnitDecl")
    {
        tmp_trans = Mgr::g.make<TranslationUnitDecl>();
        _father = tmp_trans;
    }

    // 处理子节点
    std::vector<Obj *> inner_sons;
    if (auto inner = O->getArray("inner"))
    {
        int i = 0;
        if (kind == "TranslationUnitDecl" &&
            (*inner)[0].getAsObject()->getString("kind")->str() == "TypedefDecl")
        {
            i += 5; // json内置类型去除
        }

        for (; i < inner->size(); i++)
        {
            if (auto son = deserializeJson((*inner)[i].getAsObject(), _father))
                inner_sons.push_back(son);
        }
    }

    //
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
        {
            tmp_trans->push_back(it->dcast<Decl>());
        }
        cur = tmp_trans;
    }
    else if (kind == "VarDecl")
    {
        VarDecl *tmp = Mgr::g.make<VarDecl>();
        tmp->name = name_p->str();
        tmp->id = id.str();
        if (auto ul = father->dcast<TranslationUnitDecl>())
        {
            tmp->is_global = true;
        }
        else if (auto func = father->dcast<FunctionDecl>())
        {
            func->inside_vars.push_back(tmp); // 将变量存在FunctionDecl内
        }
        sym_table.insert({id.str(), tmp});
        if (inner_sons.size() > 0)
        {
            tmp->flag = 1;
            tmp->expr_p = inner_sons[0]->dcast<Expr>();
        }
        tmp->type = type;
        cur = tmp;
    }
    else if (kind == "FunctionDecl")
    {
        tmp_func->id = id.str();
        tmp_func->name = name_p->str();
        int flag = inner_sons.size() > 0;

        for (auto it : inner_sons)
        {
            if (auto son = it->dcast<CompoundStmt>())
            {
                tmp_func->compound_stmt_p = son;
                tmp_func->flag = 1; // 有函数体
            }
            else if (auto son = it->dcast<ParmVarDecl>())
            {
                tmp_func->push_back(son);
            }
            else
                break;
        }
        tmp_func->type = type;
        cur = tmp_func;

        for (auto pa : *tmp_func) // 将参数地址放入type
            tmp_func->type.parm_vars.push_back(pa);
    }
    else if (kind == "ParmVarDecl")
    {
        ParmVarDecl *tmp = Mgr::g.make<ParmVarDecl>();
        tmp->name = name_p->str();
        tmp->type = type;
        tmp->id = id.str();
        sym_table.insert({id.str(), tmp});
        cur = tmp;
    }
    else if (kind == "CompoundStmt")
    {
        CompoundStmt *tmp = Mgr::g.make<CompoundStmt>();
        for (auto it : inner_sons)
        {
            tmp->push_back(it);
        }
        cur = tmp;
    }
    else if (kind == "ReturnStmt")
    {
        ReturnStmt *tmp = Mgr::g.make<ReturnStmt>();
        int flag = inner_sons.size() > 0;
        tmp->flag = flag;
        if (flag)
        {
            tmp->expr_p = inner_sons[0]->dcast<Expr>();
        }
        cur = tmp;
    }
    else if (kind == "DeclStmt")
    {
        DeclStmt *tmp = Mgr::g.make<DeclStmt>();
        for (auto it : inner_sons)
        {
            tmp->push_back(it->dcast<Decl>());
        }
        cur = tmp;
    }
    else if (kind == "ExprStmt")
    {
        ExprStmt *tmp = Mgr::g.make<ExprStmt>();
        tmp->expr_p = inner_sons[0]->dcast<Expr>();
        cur = tmp;
    }
    else if (kind == "IfStmt")
    {
        IfStmt *tmp = Mgr::g.make<IfStmt>();
        tmp->expr_p = inner_sons[0]->dcast<Expr>();
        tmp->stmt_p_u = inner_sons[1]->dcast<Stmt>();
        if (inner_sons.size() > 2)
        {
            tmp->flag = 1;
            tmp->stmt_p_d = inner_sons[2]->dcast<Stmt>();
        }
        cur = tmp;
    }
    else if (kind == "WhileStmt")
    {
        WhileStmt *tmp = Mgr::g.make<WhileStmt>();
        tmp->expr_p = inner_sons[0]->dcast<Expr>();
        tmp->stmt_p = inner_sons[1]->dcast<Stmt>();
        cur = tmp;
    }
    else if (kind == "DoStmt")
    {
        DoStmt *tmp = Mgr::g.make<DoStmt>();
        tmp->stmt_p = inner_sons[0]->dcast<Stmt>();
        tmp->expr_p = inner_sons[1]->dcast<Expr>();
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
        tmp->decl_p = sym_table[id_key]->dcast<Decl>();
        cur = tmp;
    }
    else if (kind == "InitListExpr")
    {
        InitListExpr *tmp = Mgr::g.make<InitListExpr>();
        tmp->type = type;
        for (auto it : inner_sons)
        {
            if (it) // 忽略ImplicitValueInitExpr
                tmp->push_back(it->dcast<Expr>());
        }
        cur = tmp;
    }
    else if (kind == "array_filler") // 不存在？
    {
        array_filler *tmp = Mgr::g.make<array_filler>();
        cur = tmp;
    }
    else if (kind == "ImplicitCastExpr")
    {
        ImplicitCastExpr *tmp = Mgr::g.make<ImplicitCastExpr>();
        tmp->type = type;
        tmp->castKind = O->getString("castKind")->str();
        tmp->expr_p = inner_sons[0]->dcast<Expr>();
        tmp->isLiteral = tmp->expr_p->isLiteral;
        cur = tmp;
    }
    else if (kind == "ArraySubscriptExpr")
    {
        ArraySubscriptExpr *tmp = Mgr::g.make<ArraySubscriptExpr>();
        tmp->type = type;
        tmp->expr_p_l = inner_sons[0]->dcast<Expr>();
        tmp->expr_p_r = inner_sons[1]->dcast<Expr>();
        cur = tmp;
    }
    else if (kind == "CallExpr")
    {
        CallExpr *tmp = Mgr::g.make<CallExpr>();
        tmp->type = type;
        tmp->expr_p = inner_sons[0]->dcast<Expr>();
        inner_sons.erase(inner_sons.begin());
        for (auto it : inner_sons)
        {
            tmp->push_back(it->dcast<Expr>());
        }
        cur = tmp;
    }
    else if (kind == "ParenExpr")
    {
        ParenExpr *tmp = Mgr::g.make<ParenExpr>();
        tmp->expr_p = inner_sons[0]->dcast<Expr>();
        cur = tmp;
    }
    else if (kind == "BinaryOperator")
    {
        BinaryOperator *tmp = Mgr::g.make<BinaryOperator>();
        tmp->expr_p_l = inner_sons[0]->dcast<Expr>();
        tmp->expr_p_r = inner_sons[1]->dcast<Expr>();
        tmp->opcode = O->getString("opcode")->str();
        tmp->isLiteral = tmp->expr_p_l->isLiteral && tmp->expr_p_r->isLiteral;
        tmp->type = type;
        cur = tmp;
    }
    else if (kind == "UnaryOperator")
    {
        UnaryOperator *tmp = Mgr::g.make<UnaryOperator>();
        tmp->expr_p = inner_sons[0]->dcast<Expr>();
        tmp->opcode = O->getString("opcode")->str();
        tmp->type = type;
        tmp->isLiteral = tmp->expr_p->isLiteral;
        // tmp->getValue();
        cur = tmp;
    }
    else if (kind == "StringLiteral")
    {
        StringLiteral *tmp = Mgr::g.make<StringLiteral>();
        tmp->str_value = value.str();
        tmp->serial = value.str();

        tmp->id = id.str();
        tmp->type = type;
        tmp->convert();
        tmp->isLiteral = true;
        cur = tmp;
    }
    else if (kind == "FloatingLiteral")
    {
        FloatingLiteral *tmp = Mgr::g.make<FloatingLiteral>();
        tmp->str_value = value.str();
        tmp->isLiteral = true;
        double tmpd;
        value.getAsDouble(tmpd);

        tmp->value = tmpd;
        tmp->type = type;
        cur = tmp;
    }
    else if (kind == "IntegerLiteral")
    {
        IntegerLiteral *tmp = Mgr::g.make<IntegerLiteral>();
        tmp->type = type;
        tmp->isLiteral = true;
        tmp->str_value = value.str();
        cur = tmp;
    }
    else
    {
        return nullptr; //"ImplicitValueInitExpr"
    }
    return cur;
}

Type getType(const llvm::StringRef qualType)
{
    Type tmp;

    // 判断是否是const
    auto qualType_rm_qualifier = qualType;
    if (qualType.split(' ').first == "const")
    {
        qualType_rm_qualifier = qualType.split(' ').second;
        tmp.is_const = true;
    }

    auto b_type = qualType_rm_qualifier.split(' ').first;
    b_type = b_type.split('[').first;

    // 判断基本类型
    if (b_type == "void")
        tmp.base_type = Type::VOID;
    else if (b_type == "char")
        tmp.base_type = Type::CHAR;
    else if (b_type == "int")
        tmp.base_type = Type::INT;
    else if (b_type == "long")
        tmp.base_type = Type::LONG;
    else if (b_type == "long long")
        tmp.base_type = Type::LONG_LONG;
    else if (b_type == "unsigned" || b_type == "unsigned int")
        tmp.base_type = Type::UNSIGNED_INT;
    else if (b_type == "float")
        tmp.base_type = Type::FLOAT;
    else
        tmp.base_type = Type::DOUBLE; // incomplete

    // 判断是否是一个函数类型 "Type ()(...)"
    auto func_type = qualType_rm_qualifier;
    if (func_type.back() == ')' && func_type[func_type.size() - 3] != '(')
    {
        tmp.is_func = true;

        long l = func_type.find('('), r = func_type.find(')');
        auto parms_type = func_type.substr(l + 1, r - l - 1);
        // 函数类型的参数在后面的parmdecl再插入
    }

    // 判断是否是数组
    auto arr_type = qualType_rm_qualifier;
    long size;
    int l, r;
    while (true)
    {
        l = arr_type.find('['), r = arr_type.find(']');
        if (l == std::string::npos)
            break;
        if (arr_type.substr(l + 1, r - l - 1).getAsInteger(0, size))
        {
            size = 100000; // warning
        }
        tmp.dims.push_back(size);
        arr_type = arr_type.substr(r + 1);
    }

    // 判断是否为指针
    if (qualType_rm_qualifier.find('*') != std::string::npos && !tmp.is_func)
    {
        tmp.is_ptr = true;
    }
    return tmp;
}
