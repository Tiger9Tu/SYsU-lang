#pragma once
#include <memory>
#include <string>
#include <vector>

struct TranslationUnitDecl;
struct Expr;
struct Stmt; // abstruct one
struct Decl; // abstract base
struct VarDecl;
struct FunctionDecl;
struct ParmVarDecl;
struct CompoundStmt;
struct ReturnStmt; // public stmt
struct ExprStmt;
struct DeclStmt;
struct IfStmt;
struct WhileStmt;
struct DoStmt;
struct BreakStmt;
struct ContinueStmt;
struct NullStmt;
struct DeclRefExpr;
struct InitListExpr;       // {1,2,3} / {{1,2},{a[0],0}} /..
struct ImplicitCastExpr;   //
struct ArraySubscriptExpr; // a[2]
struct CallExpr;           //  func()
struct ParenExpr;
struct BinaryOperator; // a = 1 / a * 1 ...
struct UnaryOperator;  // +
struct IntegerLiteral;
struct FloatingLiteral;
struct StringLiteral;

struct Visitor
{
public:
    // virtual int visit(FileSignal *) = 0;
    virtual int visit(TranslationUnitDecl *) = 0;
    virtual int visit(VarDecl *) = 0;      // int main(){ ... }
    virtual int visit(FunctionDecl *) = 0; // int main(){ ... }
    virtual int visit(ParmVarDecl *) = 0;
    virtual int visit(CompoundStmt *) = 0;
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
};

class Obj
{
public:
    virtual ~Obj() = default;

public:
    virtual int accept(Visitor *)
    {
        return -1;
    };

    template <typename T>
    T *dcast() { return dynamic_cast<T *>(this); }
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

struct FileSignal
{
public:
    static FileSignal f;
    std::string CheatIRFilePath;
    // int accept(Visitor *pv) final { return pv->visit(this); }
};

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

    bool is_ptr = false;
    bool is_func = false; // int ()
    std::vector<ParmVarDecl *> parm_vars;
    bool is_ellipsis = false;
    bool is_const = false;
    bool is_desugared = false;
    std::vector<int> dims;
};

struct Stmt
    : public Obj
{
    std::string id;
    bool hasSideEffect = false;
};

struct Expr
    : public Stmt
{
    union value
    {
        char charV;
        int intV;
        unsigned int unsignedIntV;
        long longV;
        unsigned long unsignedLongV;
        long long longLongV;
        float floatV;
        double doubleV;
    } V;

    std::string serial;
    bool isSingleConst = false;

    enum ValueCatagory
    {
        LVALUE,
        PRVALUE,
        XVALUE
    } valueCatagory;
    Type type;
};

struct Decl
    : public Expr
{
    bool isGlobal = false;
    bool isUsed = false;
    std::string name;
};

struct CheatSignal : public Stmt
{
};

struct ImplicitCastExpr
    : public Expr
{
    Expr *toCastExp;
    std::string castKind;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct InitListExpr
    : public Expr
{
    std::vector<Expr *> initExps;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct DeclRefExpr
    : public Expr
{
    Decl *referencedDecl;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct ArraySubscriptExpr
    : public Expr
{
    int flag; // []?
    Expr *arrayPointerExp, *indexExp;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct BinaryOperator
    : public Expr
{
    std::string opcode; // '+' / '-' / '=' ...
    Expr *lExp, *rExp;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct UnaryOperator
    : public Expr
{
    std::string opcode; // '+' / '-' / '&' ...
    Expr *exp;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct ParenExpr
    : public Expr
{
    Expr *exp;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct StringLiteral
    : public Expr
{
    std::string strVal;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct FloatingLiteral
    : public Expr
{
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct IntegerLiteral
    : public Expr
{
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct TypedefDecl
    : public Decl
{
};

struct ParmVarDecl
    : public Decl
{
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct FunctionDecl
    : public Decl
{
    CompoundStmt *compoundStmt; // 1
    std::vector<VarDecl *> localVars;
    std::vector<ParmVarDecl *> parmVars;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct VarDecl
    : public Decl
{
    Expr *initExp; // 1 init = expr
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct CallExpr // func(a,1)
    : public Expr
{
    Expr *functionPointerExp; // func
    std::vector<Expr *> parmExps;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct ReturnStmt
    : public Stmt
{
    Expr *returnExp;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct BreakStmt
    : public Stmt
{
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct ContinueStmt
    : public Stmt
{
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct ExprStmt
    : public Stmt
{
    Expr *exp;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct DeclStmt
    : public Stmt
{
    std::vector<Decl *> decls;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct IfStmt
    : public Stmt
{
    Expr *condExp;
    Stmt *thenStmt, *elseStmt;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct WhileStmt
    : public Stmt
{
    Expr *condExp;
    Stmt *whileBodyStmt;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct DoStmt
    : public Stmt
{
    Stmt *doBodyStmt;
    Expr *condExp;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct CompoundStmt
    : public Stmt
{
    std::vector<Stmt *> stmts;
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct NullStmt
    : public Stmt
{
    int accept(Visitor *pv) final { return pv->visit(this); }
};

struct TranslationUnitDecl
    : public Decl
{
    std::vector<Decl *> externalDecls;
    int accept(Visitor *pv) final { return pv->visit(this); }
};
