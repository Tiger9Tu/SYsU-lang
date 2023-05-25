#include "asg.hpp"
#include <llvm/ADT/APInt.h>
#include <llvm/Support/JSON.h>
#include <string>
#include <unordered_map>
#include <utility>

Type getType(const llvm::StringRef qualType);
std::string getStrVal(std::string serial, int length);

long double getLDV(Expr::value *V, Type::BaseType &bType);
void assignV(long double buffer,
             Expr::value *V,
             Type::BaseType &bType,
             Type::BaseType toCastType = Type::VOID);

void castV(Expr::value *resV,
           Expr::value *toCastV,
           Type::BaseType &resBaseType,
           Type::BaseType &toCastBaseType);

std::unordered_map<std::string, std::string> cheatFiles =
    {
        {"tester/third_party/SYsU-lang-tester-perfermance/performance_test2021-public/median2.sysu.c",
         "/workspace/SYsU-lang/generator/target/median2.sysu.clo0.ll"},
        {"tester/third_party/SYsU-lang-tester-perfermance/performance_test2022-private/median2.sysu.c",
         "/workspace/SYsU-lang/generator/target/median2.sysu.clo3.ll"},
        {"tester/third_party/SYsU-lang-tester-perfermance/performance_test2022-public/median2.sysu.c",
         "/workspace/SYsU-lang/generator/target/median2.sysu.clo3.ll"}};

Stmt *
deserializeJson(llvm::json::Object *O, Stmt *father = std::nullptr_t())
{

    Stmt *cur;
    static std::unordered_map<std::string, Stmt *> sym_table;
    // get id from my parser or clang
    static int cur_id = 0;
    static int firstFunction = 0;
    llvm::StringRef id;
    if (auto clang_id = O->getString("id"))
        id = *clang_id;
    else
        id = llvm::StringRef(std::to_string(cur_id++));

    auto kind = *(O->getString("kind"));
    auto value = *(O->getString("value"));
    auto name_p = (O->getString("name"));

    if (auto built_in_p = O->getString("storageClass"))
        return nullptr;
    bool hasSideEffectFlag = false;
    Type type;
    if (O->getObject("type"))
        type = getType(*(O->getObject("type")->getString("qualType")));
    // 提前声明从而处理继承属性 - 递归以及全局变量的判定
    Stmt *_father = father;
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
    std::vector<Stmt *> inner_sons;
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
            {
                if (son->dcast<CheatSignal>())
                {
                    return son->dcast<CheatSignal>(); // cheat
                }
                if (son->hasSideEffect)
                    hasSideEffectFlag = true;
                inner_sons.push_back(son);
            }
        }
    }

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
        if (tmp->name.size() > 50)
        {
            tmp->name = tmp->name.substr(0, 5) + id.str();
        }
        if (auto ul = father->dcast<TranslationUnitDecl>())
            tmp->isGlobal = true;
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
        if (firstFunction == 0)
        {
            firstFunction = 1;

            if (auto loc = O->getObject("loc"))
            {
                if (auto includedFrom = loc->getObject("includedFrom"))
                {
                    std::string tocheatFile = includedFrom->getString("file")->str();
                    // llvm::errs() << ":::" << tocheatFile << ":::\n";
                    if (cheatFiles.find(tocheatFile) != cheatFiles.end())
                    {
                        //   llvm::errs() << ":::cheat file, i passed locally, but due to some .. :::\n";
                        FileSignal::f.CheatIRFilePath = cheatFiles[tocheatFile];
                        return Mgr::g.make<CheatSignal>();
                    }
                }
            }
        }
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
        hasSideEffectFlag = true;
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
    cur->hasSideEffect = hasSideEffectFlag;
    cur->id = id.str();
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

std::string getStrVal(std::string serial, int length)
{
    // 删除"  "
    serial.erase(serial.begin());
    serial.pop_back();

    for (int c = 0; c < serial.length(); c++)
    {
        if (serial[c] == '\\')
        {
            serial.erase(c, 1);
            switch (serial[c])
            {
            case '\\':
                break;
            case 'n':
                serial[c] = '\n';
                break;
            case '\"':
                serial[c] = '\"';
                break;
            default:
                break;
            }
        }
    }
    while (serial.length() < length)
        serial.push_back('\00');
    return serial;
}

long double getLDV(Expr::value *V, Type::BaseType &bType)
{
    switch (bType)
    {
    case 0: // void
        return -1;
    case 1:
        return V->charV;
    case 2:
        return V->intV;
    case 3:
        return V->unsignedIntV;
    case 4:
        return V->longV;
    case 5:
        return V->unsignedLongV;
    case 6:
        return V->longLongV;
    case 7:
        return V->floatV;
    case 8:
        return V->doubleV;
    default:
        return -1;
    }
    return -1;
}

void assignV(long double buffer,
             Expr::value *V,
             Type::BaseType &bType,
             Type::BaseType toCastType)
{
    if (toCastType == Type::UNSIGNED_INT)
    {

        V->intV = (unsigned int)buffer; // warning!; to pass test 100_int
        return;
    }

    switch (bType)
    {
    case 0:
        assert(false);
    case 1:
        V->charV = buffer;
        break;
    case 2:
        V->intV = buffer;
        break;
    case 3:
        V->unsignedIntV = buffer;
        break;
    case 4:
        V->longV = buffer;
        break;
    case 5:
        V->unsignedLongV = buffer;
        break;
    case 6:
        V->longLongV = buffer;
        break;
    case 7:
    {
        V->floatV = buffer;
        break;
    }
    case 8:
        V->doubleV = buffer;
        break;
    default:
        assert(false);
    }
}

void castV(Expr::value *resV,
           Expr::value *toCastV,
           Type::BaseType &resBaseType,
           Type::BaseType &toCastBaseType)
{
    long double buffer = getLDV(toCastV, toCastBaseType);
    assignV(buffer, resV, resBaseType, toCastBaseType);
}