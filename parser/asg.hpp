#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <llvm/Support/JSON.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>

struct TranslationUnitDecl;
class ScopeManager;
struct ExternalDecl;

struct InitDeclList;    // int a,b,fun();
struct InitDecltorList; // a =1,b,c[2]={1,2}
struct InitDecltor;     // a / a=1

struct Decltor; // a, c[2],func() // abstract base
struct VarDecltor;
struct FunctionDecltor;

struct Decl; // abstract base
struct VarDecl;
struct FunctionDecl;
struct ParmVarDeclList; // int a, int b, int c
struct ParmVarDecl;

struct CompoundStmt;
struct StmtList;
struct Stmt;       // abstruct one
struct ReturnStmt; // public stmt
struct ExprStmt;
struct DeclStmt;
struct IfStmt;
struct WhileStmt;
struct DoStmt;
struct BreakStmt;
struct ContinueStmt;
struct NullStmt;

// struct InitDeclListStmt;      // Decl inside a function

struct Expr; // An expression consists of at least one
             // operand and zero or more operators.
             // Operands are typed objects such as constants, variables,
             // and function calls that return values.
struct DeclRefExpr;
struct ExprList; // 1,2,a[1],..

struct InitListExpr; // {1,2,3} / {{1,2},{a[0],0}} /..

struct ImplicitCastExpr;   //
struct ArraySubscriptExpr; // a[2]
struct CallExpr;           //  func()
struct ParenExpr;
/*
//struct Operator;

struct UnaryOperator; // + / - / !
*/
struct array_filler;
struct BinaryOperator; // a = 1 / a * 1 ...
struct UnaryOperator;  // +
struct IntegerLiteral;
struct FloatingLiteral;
struct StringLiteral;

struct Visitor
{
public:
    virtual int visit(TranslationUnitDecl *) = 0;
    virtual int visit(ExternalDecl *) = 0;
    virtual int visit(InitDeclList *) = 0;
    virtual int visit(FunctionDecltor *) = 0; // main()     // mid
    virtual int visit(VarDecl *) = 0;         // int main(){ ... }
    virtual int visit(FunctionDecl *) = 0;    // int main(){ ... }
    virtual int visit(ParmVarDeclList *) = 0;
    virtual int visit(ParmVarDecl *) = 0;
    virtual int visit(CompoundStmt *) = 0;
    virtual int visit(StmtList *) = 0;
    virtual int visit(ReturnStmt *) = 0;
    virtual int visit(DeclStmt *) = 0;
    virtual int visit(ExprStmt *) = 0;
    virtual int visit(IfStmt *) = 0;
    virtual int visit(WhileStmt *) = 0;
    virtual int visit(DoStmt *) = 0;
    virtual int visit(BreakStmt *) = 0;
    virtual int visit(ContinueStmt *) = 0;
    virtual int visit(NullStmt *) = 0;
    virtual int visit(DeclRefExpr *) = 0;
    virtual int visit(ExprList *) = 0;
    virtual int visit(InitListExpr *) = 0;
    virtual int visit(ImplicitCastExpr *) = 0;
    virtual int visit(ArraySubscriptExpr *) = 0;
    virtual int visit(CallExpr *) = 0;
    virtual int visit(ParenExpr *) = 0;
    virtual int visit(BinaryOperator *) = 0;
    virtual int visit(UnaryOperator *) = 0;
    virtual int visit(IntegerLiteral *) = 0;
    virtual int visit(StringLiteral *) = 0;
    virtual int visit(FloatingLiteral *) = 0;
    virtual int visit(array_filler *) = 0;
};

class Obj
{
public:
    std::string id;
    virtual ~Obj() = default;

public:
    virtual int accept(Visitor *)
    {
        assert(false);
        return -1;
    };

    template <typename T>
    T *dcast()
    {
        return dynamic_cast<T *>(this);
    }
};

class Mgr : public std::vector<std::unique_ptr<Obj>>
{
public:
    static Mgr g;

public:
    template <typename T, typename... Args>
    T *make(Args... args)
    {
        auto ptr = std::make_unique<T>(args...);
        auto obj = ptr.get();
        emplace_back(std::move(ptr));
        return obj;
    }
};

struct Scope
{
public:
    Scope *next; // or spell as parent, enclosing, ...
    std::unordered_map<std::string, Decl *> scope_table;

    Decl *lookupLocal(std::string key)
    {
        auto iter = scope_table.find(key);
        if (iter == scope_table.end())
        {
            return std::nullptr_t();
        }
        else
        {
            return iter->second;
        }
    }

    Decl *lookup(std::string key)
    {
        Decl *e = lookupLocal(key);
        if (e == std::nullptr_t() && next == std::nullptr_t())
        {
            return std::nullptr_t();
        }
        if (e == std::nullptr_t() && next != std::nullptr_t())
        {
            return next->lookup(key); // recursive lookup
        }
        return e;
    }

    void insert(std::string key, Decl *value)
    {
        this->scope_table.insert({key, value});
    }
};

class ScopeManager
{
public:
    enum State
    {
        PARM_CONTINUE_ST, // insert into current scope,
        PARM_READY_ST,    // create new scope, enter continuest,initial state
        CONTINUE_ST,
        READY_ST
    };
    std::vector<State> _state_stack;

    bool prev_token_is_r_paren;
    Scope *top_scope_p;
    Decl *last_global_value;

    void initializeScope()
    {
        Scope *new_top_scope_p = new Scope();
        new_top_scope_p->next = top_scope_p;
        this->top_scope_p = new_top_scope_p;
    }

    void finalizeScope()
    {
        Scope *inner_scope_p = top_scope_p->next;
        delete top_scope_p;
        top_scope_p = inner_scope_p;
    }

    Decl *lookup(std::string key)
    {
        return top_scope_p->lookup(key);
    }

    ScopeManager()
    {
        this->initializeScope();
        this->_state_stack.push_back(CONTINUE_ST);
    }

    ~ScopeManager()
    {
        this->finalizeScope();
    }

    void insert(std::string key, Decl *value)
    {
        if (_state_stack.back() == READY_ST || _state_stack.back() == PARM_READY_ST)
        {
            initializeScope();
        }
        switch (_state_stack.back())
        {
        case 1:
        {
            _state_stack.back() = PARM_CONTINUE_ST;
            break;
        }
        case 3:
        {
            _state_stack.back() = CONTINUE_ST;
            break;
        }
        default:
            break;
        }
        top_scope_p->insert(key, value);
    }

    void global_insert(std::string key, Decl *value)
    {
        if (this->top_scope_p == std::nullptr_t())
        {
            this->initializeScope();
            this->_state_stack.back() = CONTINUE_ST;
        }
        Scope *global_scope_p = this->top_scope_p;
        while (global_scope_p->next != std::nullptr_t())
        {
            global_scope_p = global_scope_p->next;
        }
        global_scope_p->insert(key, value);
        this->last_global_value = value;
    }
};

extern ScopeManager *scope_manager_p;

struct Type
{
public:
    enum BaseType
    {
        VOID,
        CHAR,
        INT,
        UNSIGNED_INT,
        LONG,
        UNSIGNED_LONG,
        LONG_LONG,
        FLOAT,
        DOUBLE,
    } base_type;

    bool is_ptr;
    bool is_func; // int ()
    std::vector<ParmVarDecl *> parm_vars;
    bool is_ellipsis;
    bool is_const;
    bool is_desugared;
    std::vector<int> dims;
    Type(const Type &_type)
    {
        this->base_type = _type.base_type;
        this->is_ptr = _type.is_ptr;
        this->is_func = _type.is_func;
        this->is_const = _type.is_const;
        this->is_desugared = _type.is_desugared; // only parm
        this->is_ellipsis = _type.is_ellipsis;
        this->dims = _type.dims;
        this->parm_vars = _type.parm_vars;
    }
    void operator=(const Type &_type)
    {
        this->base_type = _type.base_type;
        this->is_ptr = _type.is_ptr;
        this->is_func = _type.is_func;
        this->is_const = _type.is_const;
        this->is_desugared = _type.is_desugared; // only parm
        this->is_ellipsis = _type.is_ellipsis;
        this->dims = _type.dims;
        this->parm_vars = _type.parm_vars;
    }

    Type(BaseType _bt, bool _is_ptr, bool _is_func, bool _is_const = 0) : base_type(_bt),
                                                                          is_ptr(_is_ptr),
                                                                          is_func(_is_func),
                                                                          is_const(_is_const),
                                                                          is_desugared(false),
                                                                          is_ellipsis(false)
    {
    }
    Type() : is_ptr(false),
             is_func(false),
             is_const(false),
             is_desugared(false),
             is_ellipsis(false)
    {
    }
    bool operator==(const Type &type2)
    {
        return this->base_type == type2.base_type && this->is_ptr == type2.is_ptr && this->is_func == type2.is_func && this->dims == type2.dims;
        // const no need to compare?
    }
};

////////////////////////////////////midtype///////////////////////////////////

struct Stmt
    : public Obj
{
};

struct Expr
    : public Stmt
{
    long double value;
    std::string str_value;
    bool isLiteral = false;
    enum ValueCatagory
    {
        LVALUE,
        PRVALUE,
        XVALUE
    } valueCatagory;
    Type type;

    Expr()
    {
        this->type.base_type = Type::INT;
    }
    virtual void resolveInitListExpr(std::vector<int> _dims, Type _type)
    {
    }
    virtual Expr *resolveImplicitCast(Type _type, ValueCatagory _vc, int _cheat = 0)
    {
        return this;
    }
    virtual void reference()
    {
    }
};

struct Decl
    : public Obj
{
    Type type;
    std::string name;
    long double value;
    Decl()
    {
    }
    virtual void resolveImplicitCast()
    {
    }
};

/////////////////////////////////////////////expr//////////////////////////////////////

struct ImplicitCastExpr
    : public Expr
{
    Expr *expr_p;
    std::string castKind;
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    ImplicitCastExpr() {}
    ImplicitCastExpr(Expr *_son, Type _type) : expr_p(_son)
    {
        this->type = _type;
        // this->type.is_const = false;
    }
};

struct DeclRefExpr
    : public Expr
{
    std::string name;
    Decl *decl_p;
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    DeclRefExpr() {}
    DeclRefExpr(std::string _name)
    {
        this->name = _name;
        this->reference();
    }

    void reference() final
    {
        this->decl_p = scope_manager_p->lookup(this->name);
        if (this->decl_p == std::nullptr_t())
        {
            this->value = 0;
        }
        else
        {
            this->type = decl_p->type;
            this->value = this->decl_p->value;
            if (this->decl_p->type.is_func)
            {
                this->valueCatagory = PRVALUE;
            }
            else
            {
                this->valueCatagory = LVALUE;
            }
        }
    }

    Expr *resolveImplicitCast(Type _type, ValueCatagory _vc, int _cheat = 0) final
    {
        _type.is_const = this->type.is_const;
        Expr *p = this;
        Type cast;
        if (!(this->type.dims == _type.dims))
        { // arrayToPointerDecay
            cast = _type;
            cast.is_ptr = true;
            p = Mgr::g.make<ImplicitCastExpr>(this, cast);
        }
        else if (!(this->type.is_func == _type.is_func) || !(this->type.is_ptr == _type.is_ptr))
        {
            cast = this->type;
            cast.is_ptr = true; // function to pointer decay
            p = Mgr::g.make<ImplicitCastExpr>(this, cast);
        }
        else if (!(this->valueCatagory == _vc))
        {
            cast = this->type;
            cast.is_const = false; // rvalue not const
            p = Mgr::g.make<ImplicitCastExpr>(this, cast);
        }

        if (!(this->type.base_type == _type.base_type))
        {
            cast = p->type;
            cast.base_type = _type.base_type;
            p = Mgr::g.make<ImplicitCastExpr>(p, cast);
        }

        return p;
    }
};

struct ExprList
    : public Obj,
      public std::vector<Expr *>
{
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }

    void resolveImplicitCastList(std::vector<Type> _types, std::vector<Expr::ValueCatagory> _vcs,
                                 std::vector<int> _cheats = std::vector<int>())
    { // this is called when there is no father expr

        for (int i = 0; i < this->size(); i++)
        { // for example: func(a) ,where a is casted to prvalue
            if (_cheats.size() > 0)
                this->at(i) = this->at(i)->resolveImplicitCast(_types[i], _vcs[i], _cheats[i]);
            else
                this->at(i) = this->at(i)->resolveImplicitCast(_types[i], _vcs[i]);
        }
    }
};

struct array_filler
    : public Expr
{
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
};

// {1,2,3} / {{1,2},{a[0],0}} /..
struct InitListExpr
    : public Expr,
      public std::vector<Expr *>
{
    ExprList *expr_list_p;
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    InitListExpr() {}
    InitListExpr(ExprList *_expr_list_p)
    {
        this->expr_list_p = _expr_list_p;
        this->type = Type(Type::INT, false, false, false);
        // printf("initlist construct isdesuger = %d\n", this->type.is_desugared);
    }

    std::vector<int> getDimSize(std::vector<int> dims)
    {
        std::vector<int> dim_sizes;
        dim_sizes.resize(dims.size());
        int multiplier = 1;
        for (int cur_dim = dims.size() - 1; cur_dim >= 0; cur_dim--)
        {
            dim_sizes[cur_dim] = multiplier;
            multiplier *= dims[cur_dim];
        }
        return dim_sizes;
    }

    void resolveInitListExpr(std::vector<int> _dims, Type _type) final
    {
        std::vector<int> dim_sizes = getDimSize(_dims);
        this->recursiveResolver(_dims, dim_sizes, _type);
    }

    void recursiveResolver(std::vector<int> dims, std::vector<int> dim_sizes, Type _type)
    {
        this->type = _type;
        this->type.dims = dims; // for type
        if (dims.size() == 0)
            return;
        int cur_dim = dims[0];
        int cur_dim_size = dim_sizes[0];

        if (dims.size() == 1)
        {
            if (this->expr_list_p->size() < cur_dim)
            {
                this->expr_list_p->push_back(Mgr::g.make<array_filler>());
            }
            return;
        }
        dims.erase(dims.begin());
        dim_sizes.erase(dim_sizes.begin());

        int counter = 0;
        int frontier_size = this->expr_list_p->size();
        for (int i = 0; i < cur_dim && counter < frontier_size; i++)
        {
            Expr *item = this->expr_list_p->front(); // w
            if (auto p = item->dcast<InitListExpr>())
            {
                p->recursiveResolver(dims, dim_sizes, this->type);
                this->expr_list_p->push_back(p);
                expr_list_p->erase(expr_list_p->begin());
                counter++;
            }
            else
            {
                ExprList *sub_tree = Mgr::g.make<ExprList>();
                InitListExpr *sub_node = Mgr::g.make<InitListExpr>(sub_tree);
                sub_node->type.base_type = this->type.base_type; // int?
                for (int j = 0; j < cur_dim_size && counter < frontier_size; j++)
                {
                    sub_tree->push_back(expr_list_p->front());
                    expr_list_p->erase(expr_list_p->begin());
                    counter++;
                }
                sub_node->recursiveResolver(dims, dim_sizes, this->type);
                this->expr_list_p->push_back(sub_node);
            }
        }
        if (this->expr_list_p->size() < cur_dim)
        {
            this->expr_list_p->push_back(Mgr::g.make<array_filler>());
        }
    }

    Expr *resolveImplicitCast(Type _type, ValueCatagory _vc, int _cheat = 0) final
    {
        std::vector<Type> types;
        std::vector<ValueCatagory> vcs;
        _type.dims.clear();
        for (size_t i = 0; i < expr_list_p->size(); i++)
        {
            types.push_back(_type);
            vcs.push_back(_vc);
        }
        expr_list_p->resolveImplicitCastList(types, vcs);
        return this;
    }
};

struct ArraySubscriptExpr
    : public Expr
{
    int flag; // []?
    Expr *expr_p_l, *expr_p_r;
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    ArraySubscriptExpr() {}
    ArraySubscriptExpr(int _flag, Expr *_expr_p_l, Expr *_expr_p_r) : flag(_flag), expr_p_l(_expr_p_l), expr_p_r(_expr_p_r)
    {
        this->type = _expr_p_l->type;
        // this->type.dims.pop_back();
        // printf("this->type.dims = %zu\nthis->expr_p_l->decl*=%p\n",this->type.dims.size(),this->expr_p_l->dcast<DeclRefExpr>()->decl_p);
        if (this->type.is_ptr)
        {
            this->type.is_ptr = false;
        }
        else
        {
            this->type.dims.erase(this->type.dims.begin());
        }
        this->type.is_desugared = false;
        this->valueCatagory = LVALUE; // ?
    }

    Expr *resolveImplicitCast(Type _type, ValueCatagory _vc, int _cheat = 0) final
    {
        _type.is_const = this->type.is_const;
        Type array_index_type;
        if (this->expr_p_r->type.base_type > 4)
            array_index_type = Type(this->expr_p_r->type.base_type, false, false);
        else
        {
            array_index_type = Type(Type::INT, false, false);
        }
        this->expr_p_l =
            this->expr_p_l->resolveImplicitCast(this->type, this->valueCatagory);
        this->expr_p_r =
            this->expr_p_r->resolveImplicitCast(array_index_type, Expr::PRVALUE);

        Expr *p = this;
        Type cast;
        if (!(this->type.dims == _type.dims))
        { // arrayToPointerDecay
            cast = _type;
            cast.is_ptr = true;
            p = Mgr::g.make<ImplicitCastExpr>(this, cast);
        }
        else if (!(this->type.is_func == _type.is_func) || !(this->type.is_ptr == _type.is_ptr))
        {
            cast = this->type;
            cast.is_ptr = true; // function to pointer decay
            p = Mgr::g.make<ImplicitCastExpr>(this, cast);
        }
        else if (!(this->valueCatagory == _vc))
        {
            cast = this->type;
            cast.is_const = false;
            p = Mgr::g.make<ImplicitCastExpr>(this, cast);
        }

        if (!(this->type.base_type == _type.base_type))
        {
            cast = p->type;
            cast.base_type = _type.base_type;
            p = Mgr::g.make<ImplicitCastExpr>(p, cast);
        }

        return p;
    }
};

struct BinaryOperator
    : public Expr
{
    std::string opcode; // '+' / '-' / '=' ...
    Expr *expr_p_l, *expr_p_r;
    Type::BaseType pro_base_type;
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }

    void getValue()
    {
        double lvalue, rvalue; // int is also double
        lvalue = expr_p_l->value;
        rvalue = expr_p_r->value;
        if (opcode == "+")
        {
            this->value = lvalue + rvalue;
        }
        else if (opcode == "-")
        {
            this->value = lvalue - rvalue;
        }
        else if (opcode == "*")
        {
            this->value = lvalue * rvalue;
        }
        else if (opcode == "/")
        {
            this->value = lvalue / rvalue;
        }
        else if (opcode == "%%")
        {
            this->value = lvalue / rvalue;
        }
        else if (opcode == "&&")
        {
            this->value = lvalue && rvalue;
        }
        else if (opcode == "||")
        {
            this->value = lvalue || rvalue;
        }
        else if (opcode == "==")
        {
            this->value = lvalue == rvalue;
        }
        else if (opcode == "!=")
        {
            this->value = lvalue != rvalue;
        }
        else if (opcode == ">")
        {
            this->value = lvalue > rvalue;
        }
        else if (opcode == ">=")
        {
            this->value = lvalue >= rvalue;
        }
        else if (opcode == "<")
        {
            this->value = lvalue < rvalue;
        }
        else if (opcode == "<=")
        {
            this->value = lvalue <= rvalue;
        }
        else
        {
            this->value = lvalue;
        }
    }

    bool is_logic()
    {
        return std::string("||&&").find(this->opcode) != std::string::npos;
    }

    bool is_compare()
    {
        if (this->opcode == "=")
            return false;
        return std::string("<>==>=<=!=").find(this->opcode) != std::string::npos;
    }
    BinaryOperator() {}
    BinaryOperator(std::string _opcode,
                   Expr *_expr_p_l,
                   Expr *_expr_p_r)
    {
        this->opcode = _opcode;
        pro_base_type = (int)(_expr_p_l->type.base_type) > (int)(_expr_p_r->type.base_type) ? _expr_p_l->type.base_type : _expr_p_r->type.base_type;
        if (this->opcode == "=")
        {
            this->type = _expr_p_l->type;
        }
        else if (is_logic() || is_compare())
        {
            this->type.base_type = Type::INT;
        }
        else
        {
            this->type.base_type = pro_base_type;
        }
        this->type.is_ptr = false;
        this->type.is_func = false;
        this->type.is_const = false;
        this->type.is_desugared = false;
        this->valueCatagory = Expr::PRVALUE;
        expr_p_l = _expr_p_l;
        expr_p_r = _expr_p_r;

        getValue();
    }
    void reference() final
    {
        this->expr_p_l->reference();
        this->expr_p_r->reference();
    }

    Expr *resolveImplicitCast(Type _type, ValueCatagory _vc, int _cheat = 0) final
    {
        if (this->opcode == "=")
        {
            this->expr_p_l =
                this->expr_p_l->resolveImplicitCast(this->expr_p_l->type, this->expr_p_l->valueCatagory);
            this->expr_p_r =
                this->expr_p_r->resolveImplicitCast(this->type, this->valueCatagory);
        }
        else if (is_logic())
        {
            this->expr_p_l =
                this->expr_p_l->resolveImplicitCast(this->expr_p_l->type, this->valueCatagory);
            this->expr_p_r =
                this->expr_p_r->resolveImplicitCast(this->expr_p_r->type, this->valueCatagory);
        }
        else
        {
            Type cast = this->type;
            cast.base_type = this->pro_base_type;
            this->expr_p_l =
                this->expr_p_l->resolveImplicitCast(cast, this->valueCatagory);
            this->expr_p_r =
                this->expr_p_r->resolveImplicitCast(cast, this->valueCatagory);
        }
        // right

        if (this->type == _type && this->valueCatagory == _vc)
            return this;
        else
        {
            _type.is_const = this->type.is_const; // rvalue always
            return Mgr::g.make<ImplicitCastExpr>(this, _type);
        }
    }
};

struct UnaryOperator
    : public Expr
{
    std::string opcode; // '+' / '-' / '&' ...
    Expr *expr_p;

    void getValue()
    {
        if (is_logic())
        {
            this->type.base_type = Type::INT;
            this->value = (int)expr_p->value;
        }
        if (this->opcode == "!")
        {
            this->value = expr_p->value == 0 ? 1 : 0;
        }
        if (this->opcode == "-")
        {
            this->value = -expr_p->value;
        }
    }
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }

    bool is_logic()
    {
        return std::string("!").find(this->opcode) != std::string::npos;
    }
    UnaryOperator() {}
    UnaryOperator(std::string _opcode,
                  Expr *_expr_p)
    {
        this->type = _expr_p->type;
        this->type.is_ptr = false;
        this->type.is_func = false;
        this->type.is_const = false;
        this->valueCatagory = Expr::PRVALUE;
        opcode = _opcode;
        expr_p = _expr_p;

        getValue();
    }

    Expr *resolveImplicitCast(Type _type, ValueCatagory _vc, int _cheat = 0) final
    {
        if (is_logic())
        {
            this->expr_p =
                this->expr_p->resolveImplicitCast(this->expr_p->type, this->valueCatagory);
        }
        else
        {
            this->expr_p =
                this->expr_p->resolveImplicitCast(this->type, this->valueCatagory);
        }
        if (this->type == _type && this->valueCatagory == _vc)
            return this;
        else
        {
            _type.is_const = this->type.is_const;
            return Mgr::g.make<ImplicitCastExpr>(this, _type);
        }
    }
};

struct ParenExpr
    : public Expr
{
    Expr *expr_p;

    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    ParenExpr() {}
    ParenExpr(Expr *_expr_p)
    {
        this->type = _expr_p->type;
        // this->type.is_func = false;
        this->valueCatagory = _expr_p->valueCatagory;
        expr_p = _expr_p;
    }

    Expr *resolveImplicitCast(Type _type, ValueCatagory _vc, int _cheat = 0) final
    {
        this->expr_p =
            this->expr_p->resolveImplicitCast(this->type, this->valueCatagory);
        if (this->type == _type && this->valueCatagory == _vc)
            return this;
        else
        {
            _type.is_const = this->type.is_const;
            return Mgr::g.make<ImplicitCastExpr>(this, _type);
        }
    }
};

struct StringLiteral
    : public Expr
{
    std::string serial; // for output , tricky
    llvm::StringRef str_v;
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    StringLiteral()
    {
        this->type.base_type = Type::CHAR;
        this->type.is_ptr = false;
        this->type.is_func = false;
        this->valueCatagory = Expr::PRVALUE;
        this->type.dims.push_back(0);
    }
    // noop cast cheat

    void get_count()
    {
        int _count = 0;
        for (int i = 0; i < serial.length(); i++)
        {
            if (serial[i] == '\\')
            {
                i++;
            }
            _count++;
        }
        this->type.dims[0] = _count - 1;
    }

    void convert()
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
                    llvm::errs() << "defalte char: " << serial[c] << "\n";
                    break;
                }
            }
        }
        while (serial.length() < (type.dims[0] - 1))
            serial.push_back('\00');
    }

    Expr *
    resolveImplicitCast(Type _type, ValueCatagory _vc, int _cheat = 0) final
    {
        Expr *p = this;

        if (!(this->type.is_ptr == _type.is_ptr))
        {
            Type cast = this->type;
            cast.is_ptr = true;
            cast.dims.pop_back();                       // arraytopointer
            p = Mgr::g.make<ImplicitCastExpr>(p, cast); // array to pointer decay
        }

        if (!(this->type.base_type == _type.base_type))
        {
            Type cast = p->type;
            cast.base_type = _type.base_type;
            p = Mgr::g.make<ImplicitCastExpr>(p, cast);
        }

        if (_cheat == 1)
        {
            _type.is_desugared = true;
            p = Mgr::g.make<ImplicitCastExpr>(p, _type); // noop
        }

        return p;
    }
};

struct FloatingLiteral
    : public Expr
{
    std::string serial; // for output , tricky
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    FloatingLiteral() {}
    FloatingLiteral(std::string _serial) : serial(_serial)
    { // maby
        this->type.base_type = Type::DOUBLE;
        this->type.is_ptr = false;
        this->type.is_func = false;
        this->valueCatagory = Expr::PRVALUE;
        this->value = std::stof(serial);
    }
    Expr *resolveImplicitCast(Type _type, ValueCatagory _vc, int _cheat = 0) final
    {
        if (this->type == _type && this->valueCatagory == _vc)
        {
            return this;
        }
        _type.is_const = this->type.is_const;
        return Mgr::g.make<ImplicitCastExpr>(this, _type);
    }
};

struct IntegerLiteral
    : public Expr
{
    std::string serial;
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    IntegerLiteral()
    {
    }
    IntegerLiteral(std::string _serial) : serial(_serial)
    { // maby
        this->type = Type(Type::INT, false, false);
        this->valueCatagory = Expr::PRVALUE;
        get_value();
    }

    void get_value()
    {
        if (serial.find("0x") != std::string::npos)
        {
            this->value = std::stoll(this->serial, 0, 16);
        }
        else if (serial[0] == '0')
        {
            this->value = std::stoll(this->serial, 0, 8);
        }
        else
        {
            this->value = std::stoll(this->serial);
        }

        if (this->value <= 2147483647 && this->value >= -21474836478)
        {
            this->type.base_type = Type::INT;
        }
        else if (this->serial.find("0x") != std::string::npos)
        { // weried llvm
            this->type.base_type = Type::UNSIGNED_INT;
        }
        else
        {
            this->type.base_type = Type::LONG;
        } /*else {
            this->type.base_type = Type::UNSIGNED_LONG;
        }*/
    }

    Expr *resolveImplicitCast(Type _type, ValueCatagory _vc, int _cheat = 0) final
    {
        if (this->type == _type && this->valueCatagory == _vc)
        {
            return this;
        }
        _type.is_const = this->type.is_const;
        return Mgr::g.make<ImplicitCastExpr>(this, _type);
    }
};

/////////////////////////////////////////////decl//////////////////////////////

struct TypeSpec : public Obj
{
    Type type;
    TypeSpec(Type::BaseType _bt, bool _is_const)
    {
        this->type = Type(_bt, false, false, _is_const);
    }
};

struct Decltor
    : public Decl
{
    int flag; // 0 for var 1 for func
    // std::string name;
};

struct InitDecltor
    : public Obj
{
    int flag;
    Decltor *decltor_p; // 0 1
    Expr *expr_p;       // 1 a[2] = {1,2} / a = 1

    // remember to call resolve
    int get_decltor_catagory()
    {
        return decltor_p->flag;
    }

    InitDecltor(int _flag,
                Decltor *_decltor_p,
                Expr *_expr_p)
    {
        this->flag = _flag;
        this->decltor_p = _decltor_p;
        this->expr_p = _expr_p;
    }
};

struct InitDecltorList
    : public Obj,
      public std::vector<InitDecltor *>
{
};

struct VarDecltor
    : public Decltor
{
    bool is_ptr;
    std::vector<int> dims; // a[1][3] : dims = {1,3}
                           // a[][3] : dims = {1} is_ptr
                           // a[] : dims = {1};
    VarDecltor(std::string _name)
    {
        this->name = _name;
    }
};

struct ParmVarDecl
    : public Decl
{
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    ParmVarDecl() {}
    ParmVarDecl(Type _type, VarDecltor *_var_decltor_p)
    {
        this->name = _var_decltor_p->name;
        this->type = _type;
        this->type.is_ptr = _var_decltor_p->is_ptr;
        this->type.dims = _var_decltor_p->dims;
        if (this->type.dims.size() > 0)
        {
            this->type.is_ptr = true;
            this->type.is_desugared = true;
            this->type.dims.erase(this->type.dims.begin());
        }
        scope_manager_p->insert(this->name, this);
    }
};

struct ParmVarDeclList //(int a,int b)
    : public Obj,
      public std::vector<ParmVarDecl *>
{
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
};

struct FunctionDecltor
    : public Decltor
{
    int flag;
    ParmVarDeclList *parm_var_decl_list_p; // 1
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    FunctionDecltor(int _flag,
                    std::string _name,
                    ParmVarDeclList *_parm_var_decl_list_p)
    {
        this->flag = _flag;
        this->name = _name;
        this->parm_var_decl_list_p = _parm_var_decl_list_p;
    }
};

struct FunctionDecl
    : public Decl,
      public std::vector<ParmVarDecl *>
{
    int flag;
    CompoundStmt *compound_stmt_p; // 1
    FunctionDecltor *function_decltor_p;
    std::vector<VarDecl *> inside_vars;

    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    FunctionDecl() : flag(0)
    {
    }

    FunctionDecl(int _flag,
                 CompoundStmt *_compound_stmt_p,
                 FunctionDecltor *_function_decltor_p,
                 Type::BaseType _bt)
    {
        this->flag = _flag;
        this->compound_stmt_p = _compound_stmt_p;
        this->function_decltor_p = _function_decltor_p;
        this->name = function_decltor_p->name;
        ParmVarDeclList *tmp = getParmDeclList();
        this->type = Type(_bt, false, true);
        this->type.is_ellipsis = this->function_decltor_p->type.is_ellipsis;
        if (tmp != std::nullptr_t())
        {
            for (int i = 0; i < tmp->size(); i++)
            {
                this->type.parm_vars.push_back(tmp->at(i));
            }
        }

        scope_manager_p->global_insert(this->name, this);
    }

    ParmVarDeclList *getParmDeclList()
    {
        return function_decltor_p->parm_var_decl_list_p;
    }
};

struct VarDecl
    : public Decl
{
    int flag;
    Expr *expr_p; // 1 init = expr
    VarDecltor *var_decltor_p;
    int is_global;
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    VarDecl() : flag(0), is_global(0) {}
    VarDecl(VarDecltor *_var_decltor_p, Type _type_spec, Expr *_expr_p)
    {
        this->var_decltor_p = _var_decltor_p;
        this->name = _var_decltor_p->name;
        this->type = _type_spec;
        // printf("var decl type.isconst = %d\n", this->type.is_const);
        this->type.dims = _var_decltor_p->dims;
        if (_expr_p)
        {
            this->flag = 1;
            this->expr_p = _expr_p;
            if (auto decl_ref_expr_p = _expr_p->dcast<DeclRefExpr>())
            {
                decl_ref_expr_p->reference();
            }
            this->value = _expr_p->value; // what about {1,2,3} ?-> value = 0
        }
        else
        {
            this->flag = 0;
            this->value = 0;
        }
        if (this->type.base_type == Type::CHAR)
        {
            _expr_p->type.dims = this->type.dims;
            _expr_p->type.is_const = this->type.is_const; // ugly hard coding patch
        }
        scope_manager_p->insert(this->name, this);
    }

    void resolveImplicitCast() final
    {
        if (this->flag)
        {
            this->expr_p =
                this->expr_p->resolveImplicitCast(this->type, Expr::PRVALUE);
        }
    }
};

struct InitDeclList
    : public Obj,
      public std::vector<Decl *>
{
    InitDecltorList *init_decltor_list_p;
    Type type;

    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }

    InitDeclList(Type _type,
                 InitDecltorList *_init_decltor_list_p)
    {
        this->type = _type;
        this->init_decltor_list_p = _init_decltor_list_p;
        this->resolveDecl();
    }

    void resolveDecl()
    {
        for (int i = 0; i < init_decltor_list_p->size(); i++)
        {
            InitDecltor *_init_dt_p = init_decltor_list_p->at(i);
            int catagory = _init_dt_p->get_decltor_catagory();
            if (catagory == 0)
            { // var
                VarDecl *var_decl_p;
                VarDecltor *var_decltor_p = _init_dt_p->decltor_p->dcast<VarDecltor>();
                if (_init_dt_p->flag)
                {
                    Expr *init_expr_p = _init_dt_p->expr_p;
                    if (auto p9 = init_expr_p->dcast<InitListExpr>())
                        init_expr_p->type.base_type = type.base_type;
                    init_expr_p->reference();
                    init_expr_p->resolveInitListExpr(var_decltor_p->dims, this->type);
                    var_decl_p = Mgr::g.make<VarDecl>(var_decltor_p, this->type, init_expr_p);
                }
                else
                {
                    var_decl_p = Mgr::g.make<VarDecl>(
                        var_decltor_p, this->type, std::nullptr_t());
                }
                this->push_back(var_decl_p);
            }
            else
            {
                auto function_decl_p = Mgr::g.make<FunctionDecl>(0,
                                                                 std::nullptr_t(),
                                                                 _init_dt_p->decltor_p->dcast<FunctionDecltor>(),
                                                                 this->type.base_type);
                this->push_back(function_decl_p);
            }
        }
    }

    void resolveImplicitCastList()
    {
        for (int i = 0; i < this->size(); i++)
        {
            this->at(i)->resolveImplicitCast();
        }
    }
};

/////////////////////////////////////
struct CallExpr // func(a,1)
    : public Expr,
      public std::vector<Expr *>
{
    int flag;
    Expr *expr_p; // func
    DeclRefExpr *decl_ref_expr_p;
    ExprList *expr_list_p; // 1,a,3
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    CallExpr() : flag(0) {}
    CallExpr(int _flag, Expr *_expr_p, ExprList *_expr_list_p)
    {
        this->flag = _flag;
        this->expr_p = _expr_p;
        this->decl_ref_expr_p = _expr_p->dcast<DeclRefExpr>();
        this->expr_list_p = _expr_list_p;
        this->type = _expr_p->type;
        this->type.is_func = false; // int() -> int
        this->type.is_ptr = false;  // we dont have ptr type
        this->valueCatagory = Expr::PRVALUE;
    }

    Expr *resolveImplicitCast(Type _type, ValueCatagory _vc, int _cheat = 0) final
    {
        if (this->flag == 1)
        {
            ParmVarDeclList *tmp = getParmDeclList();

            std::vector<ValueCatagory> vcs;
            std::vector<Type> types;
            std::vector<int> cheats;
            for (int i = 0; i < tmp->size(); i++)
            {
                if (tmp->at(i)->type.is_ptr || tmp->at(i)->type.dims.size() > 0)
                {
                    vcs.push_back(PRVALUE); // ?
                }
                else
                {
                    vcs.push_back(PRVALUE);
                }
                Type cast = tmp->at(i)->type;
                cast.is_desugared = false;
                types.push_back(cast);
                if (tmp->at(i)->type.base_type == Type::CHAR)
                {
                    cheats.push_back(1);
                }
                else
                {
                    cheats.push_back(0);
                }
            }

            this->expr_list_p->resolveImplicitCastList(types, vcs, cheats);
        }

        this->expr_p =
            this->expr_p->resolveImplicitCast(this->type, this->valueCatagory);
        if (this->type == _type && this->valueCatagory == _vc)
            return this;
        else
        {
            return Mgr::g.make<ImplicitCastExpr>(this, _type);
        }
    }

    ParmVarDeclList *getParmDeclList()
    {
        return decl_ref_expr_p->decl_p->dcast<FunctionDecl>()->function_decltor_p->parm_var_decl_list_p;
    }
};

/////////////////////////////////////////////stmt//////////////////////////////////////

struct ReturnStmt
    : public Stmt
{
    int flag;
    Expr *expr_p; // 1
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    ReturnStmt() : flag(0) {}
    ReturnStmt(int _flag, Expr *_expr_p) : flag(_flag), expr_p(_expr_p)
    {
        resolveImplicitCast();
    }

    void resolveImplicitCast()
    {
        if (this->flag == 1)
        {
            Type::BaseType bt = scope_manager_p->last_global_value->type.base_type;
            Type tmp = Type(bt, false, false, false); // expr
            this->expr_p = expr_p->resolveImplicitCast(tmp, Expr::PRVALUE);
        }
    }
};

struct BreakStmt
    : public Stmt
{
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
};

struct ContinueStmt
    : public Stmt
{
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
};

struct ExprStmt
    : public Stmt
{
    Expr *expr_p;
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    ExprStmt() {}
    ExprStmt(Expr *_expr_p) : expr_p(_expr_p)
    {
        expr_p->resolveImplicitCast(expr_p->type, expr_p->valueCatagory);
    }
};

struct DeclStmt
    : public Stmt,
      public std::vector<Decl *>
{
    InitDeclList *init_decl_list_p;
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    DeclStmt() {}
    DeclStmt(InitDeclList *_init_decl_list_p) : init_decl_list_p(_init_decl_list_p)
    {
        init_decl_list_p->resolveImplicitCastList();
    }
};

struct IfStmt
    : public Stmt
{
    int flag;
    Expr *expr_p;
    Stmt *stmt_p_u, *stmt_p_d;
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    IfStmt() : flag(0) {}
    IfStmt(int _flag,
           Expr *_expr_p,
           Stmt *_stmt_p_u,
           Stmt *_stmt_p_d)
    {
        this->expr_p =
            _expr_p->resolveImplicitCast(_expr_p->type, Expr::PRVALUE);
        this->flag = _flag;
        this->stmt_p_u = _stmt_p_u;
        this->stmt_p_d = _stmt_p_d;
    }
};

struct WhileStmt
    : public Stmt
{
    Expr *expr_p;
    Stmt *stmt_p;
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    WhileStmt() {}
    WhileStmt(Expr *_expr_p, Stmt *_stmt_p) : expr_p(_expr_p), stmt_p(_stmt_p)
    {
        this->expr_p = _expr_p->resolveImplicitCast(_expr_p->type, Expr::PRVALUE);
    }
};

struct DoStmt
    : public Stmt
{
    Expr *expr_p;
    Stmt *stmt_p;
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    DoStmt() {}
    DoStmt(Expr *_expr_p, Stmt *_stmt_p) : expr_p(_expr_p), stmt_p(_stmt_p)
    {
        this->expr_p = _expr_p->resolveImplicitCast(_expr_p->type, Expr::PRVALUE);
    }
};

struct StmtList
    : public Obj,
      public std::vector<Stmt *>
{
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
};

struct CompoundStmt
    : public Stmt,
      public std::vector<Obj *> // binaryOperator
{
    int flag;
    StmtList *stmt_list_p;
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    CompoundStmt() {}
    CompoundStmt(int _flag, StmtList *_stmt_list_p) : flag(_flag), stmt_list_p(_stmt_list_p)
    {
    }
};

struct NullStmt
    : public Stmt
{
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
};

struct TranslationUnitDecl
    : public Obj,
      public std::vector<Decl *>
{
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
};

extern TranslationUnitDecl *trans_unit_decl_p;

struct ExternalDecl
    : public Decl
{
    int flag;
    InitDeclList *init_decl_list_p;
    FunctionDecl *function_decl_p;
    int accept(Visitor *pv) final
    {
        return pv->visit(this);
    }
    ExternalDecl(int _flag,
                 InitDeclList *_init_decl_list_p,
                 FunctionDecl *_function_decl_p)
    {
        this->flag = _flag;

        this->init_decl_list_p = _init_decl_list_p;
        this->function_decl_p = _function_decl_p;

        if (_flag == 0)
        {
            this->init_decl_list_p->resolveImplicitCastList();
        }
    }
};

struct Str
{
    std::string s;
    Str(std::string _s) : s(_s)
    {
    }
};