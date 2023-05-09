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

////////////////////////////////////midtype///////////////////////////////////

struct Stmt
    : public Obj
{
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
    bool isLiteral = false;
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
    std::string name;
};

/////////////////////////////////////////////expr//////////////////////////////////////

struct ImplicitCastExpr
    : public Expr
{
    Expr *expToCast;
    std::string castKind;
};

struct DeclRefExpr
    : public Expr
{
    Decl *referencedDecl;
};

struct ArraySubscriptExpr
    : public Expr
{
    int flag; // []?
    Expr *arrayPointer, *index;
};

struct BinaryOperator
    : public Expr
{
    std::string opcode; // '+' / '-' / '=' ...
    Expr *expL, *expR;
};

struct UnaryOperator
    : public Expr
{
    std::string opcode; // '+' / '-' / '&' ...
    Expr *exp;
};

struct ParenExpr
    : public Expr
{
    Expr *exp;
};

struct StringLiteral
    : public Expr
{
    std::string strVal;
};

struct FloatingLiteral
    : public Expr
{
};

struct IntegerLiteral
    : public Expr
{
};

struct ParmVarDecl
    : public Decl
{
};

struct FunctionDecl
    : public Decl,
{
    CompoundStmt *compoundStmt; // 1
    std::vector<VarDecl *> localVars;
    std::vector<ParmVarDecl *> parmVars;
};

struct VarDecl
    : public Decl
{
    Expr *exp; // 1 init = expr
    bool is_global;
};

struct CallExpr // func(a,1)
    : public Expr,
{
    Expr *functionPointer; // func
    std::vector<Expr *> parmExps;
};

/////////////////////////////////////////////stmt//////////////////////////////////////

struct ReturnStmt
    : public Stmt
{
    Expr *exp;
};

struct BreakStmt
    : public Stmt
{
};

struct ContinueStmt
    : public Stmt
{
};

struct ExprStmt
    : public Stmt
{
    Expr *exp;
};

struct DeclStmt
    : public Stmt
{
    std::vector<Decl *> decls;
};

struct IfStmt
    : public Stmt
{
    Expr *condExp;
    Stmt *ifThen, *ifElse;
};

struct WhileStmt
    : public Stmt
{
    Expr *condExp;
    Stmt *whileBody;
};

struct DoStmt
    : public Stmt
{
    Stmt *doBody;
    Expr *cond;
};

struct CompoundStmt
    : public Stmt
{
    std::vector<Stmt *> stmts;
};

struct NullStmt
    : public Stmt
{
};

struct TranslationUnitDecl
    : public Obj,
{
    std::vector<Decl *> externalDecls;
};
