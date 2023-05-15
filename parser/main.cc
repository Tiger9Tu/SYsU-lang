#include "parser.hh"
#include <cassert>
#include <llvm/ADT/APFloat.h>
#include <iostream>
#define DEBUG 0

struct JsonVisitor : public Visitor {
    int visit(TranslationUnitDecl *p) final {
        int count = 0;
        for (int i = 0; i < p->size(); i++) {
            count += p->at(i)->accept(this);
        }
        llvm::json::Value tmp = common("TranslationUnitDecl", std::string(), std::string(), std::nullptr_t());
        reduce(tmp, count);
        return 1;
    }

    int visit(ExternalDecl *p) final {
        switch (p->flag) {
        case 0: return p->init_decl_list_p->accept(this);
        case 1: return p->function_decl_p->accept(this);
        default: assert(false);
        }
        return -1;
    }

    int visit(InitDeclList *p) final {
        int count = 0;
        for (int i = 0; i < p->size(); i++) {
            count += p->at(i)->accept(this);
        }
        return count;
    }

    int visit(FunctionDecltor *p) final {
        int count = 0;
        if (p->flag) {
            count += p->parm_var_decl_list_p->accept(this);
        }
        return count;
    }

    int visit(VarDecl *p) final {
        llvm::json::Value tmp = common("VarDecl", p->name, std::string(), typeToJasonValue(&p->type));
        switch (p->flag) {
        case 0: {
            reduce(tmp, 0);
            return 1;
        }
        case 1: {
            p->expr_p->accept(this);
            reduce(tmp, 1);
            return 1;
        }
        default: assert(false);
        }
        return -1;
    }

    int visit(FunctionDecl *p) final {
        int count = 0;
        count += p->function_decltor_p->accept(this);
        if (p->flag) {
            count += p->compound_stmt_p->accept(this);
        }
        llvm::json::Value tmp = common("FunctionDecl", p->name, std::string(), typeToJasonValue(&p->type));
        reduce(tmp, count);
        return 1;
    }

    int visit(ParmVarDeclList *p) final {
        int count = 0;
        for (int i = 0; i < p->size(); i++) {
            count += p->at(i)->accept(this);
        }
        return count;
    }

    int visit(ParmVarDecl *p) final {
        llvm::json::Value tmp = common("ParmVarDecl", p->name, std::string(), typeToJasonValue(&p->type));
        reduce(tmp, 0);
        return 1;
    }

    int visit(CompoundStmt *p) final {
        int count = 0;
        if (p->flag == 1)
            count += p->stmt_list_p->accept(this);
        llvm::json::Value tmp = common("CompoundStmt", std::string(), std::string(), std::nullptr_t());
        reduce(tmp, count);
        return 1;
    }

    int visit(StmtList *p) final {
        int count = 0;
        for (int i = 0; i < p->size(); i++)
            count += p->at(i)->accept(this);
        return count;
    }

    int visit(ReturnStmt *p) final {
        int count = 0;
        if (p->flag == 1)
            count += p->expr_p->accept(this);
        llvm::json::Value tmp = common("ReturnStmt",
                                       std::string(),
                                       std::string(),
                                       std::nullptr_t());
        reduce(tmp, count);
        return 1;
    }

    int visit(BreakStmt *p) final {
        llvm::json::Value tmp = common("BreakStmt",
                                       std::string(),
                                       std::string(),
                                       std::nullptr_t());
        reduce(tmp, 0);
        return 1;
    }

    int visit(ContinueStmt *p) final {
        llvm::json::Value tmp = common("ContinueStmt",
                                       std::string(),
                                       std::string(),
                                       std::nullptr_t());
        reduce(tmp, 0);
        return 1;
    }

    int visit(ExprStmt *p) final {
        return p->expr_p->accept(this);
    }

    int visit(DeclStmt *p) final {
        int count = p->init_decl_list_p->accept(this);
        llvm::json::Value tmp = common("DeclStmt",
                                       std::string(),
                                       std::string(),
                                       std::nullptr_t());
        reduce(tmp, count);
        return 1;
    }

    int visit(IfStmt *p) final {
        int count = 0;
        count += p->expr_p->accept(this);
        count += p->stmt_p_u->accept(this);
        if (p->flag == 1) {
            count += p->stmt_p_d->accept(this);
        }
        llvm::json::Value tmp = common("IfStmt",
                                       std::string(),
                                       std::string(),
                                       std::nullptr_t());
        reduce(tmp, count);
        return 1;
    }

    int visit(WhileStmt *p) final {
        int count = p->expr_p->accept(this);
        count += p->stmt_p->accept(this);
        llvm::json::Value tmp = common("WhileStmt",
                                       std::string(),
                                       std::string(),
                                       std::nullptr_t());
        reduce(tmp, count);
        return 1;
    }

    int visit(DoStmt *p) final {
        int count = p->stmt_p->accept(this);
        count += p->expr_p->accept(this);
        llvm::json::Value tmp = common("DoStmt",
                                       std::string(),
                                       std::string(),
                                       std::nullptr_t());
        reduce(tmp, count);
        return 1;
    }

    int visit(NullStmt *p) final {
        llvm::json::Value tmp = common("NullStmt",
                                       std::string(),
                                       std::string(),
                                       std::nullptr_t());
        reduce(tmp, 0);
        return 1;
    }

    int visit(DeclRefExpr *p) final {
        llvm::json::Value tmp = common("DeclRefExpr",
                                       p->name,
                                       valueCatagoryToStr(p->valueCatagory),
                                       typeToJasonValue(&p->type));
        reduce(tmp, 0);

        return 1;
    }

    int visit(CallExpr *p) final {
        llvm::json::Value tmp = common("CallExpr", std::string(), valueCatagoryToStr(p->valueCatagory), typeToJasonValue(&p->type));
        int count = p->expr_p->accept(this);
        if (p->flag) {
            count += p->expr_list_p->accept(this);
        }
        reduce(tmp, count);
        return 1;
    }

    int visit(ExprList *p) final {
        int count = 0;
        for (int i = 0; i < p->size(); i++)
            count += p->at(i)->accept(this);
        return count;
    }

    int visit(InitListExpr *p) final {
        llvm::json::Value tmp = common("InitListExpr", std::string(), std::string(), typeToJasonValue(&p->type));
        int count = p->expr_list_p->accept(this);
        reduce(tmp, count);
        return 1;
    }

    int visit(ImplicitCastExpr *p) final {
        llvm::json::Value tmp = common("ImplicitCastExpr", std::string(), std::string(), typeToJasonValue(&p->type));
        int count = p->expr_p->accept(this);
        reduce(tmp, count);
        return 1;
    }

    int visit(ArraySubscriptExpr *p) final {
        llvm::json::Value tmp = common("ArraySubscriptExpr", std::string(), valueCatagoryToStr(p->valueCatagory), typeToJasonValue(&p->type));
        int count = p->expr_p_l->accept(this);
        if (p->flag == 1)
            count += p->expr_p_r->accept(this);
        reduce(tmp, count);
        return 1;
    }

    int visit(BinaryOperator *p) final {
        llvm::json::Value tmp = common("BinaryOperator", std::string(), valueCatagoryToStr(p->valueCatagory), typeToJasonValue(&p->type));
        tmp.getAsObject()->insert({"opcode", p->opcode});
        int count = 0;
        count += p->expr_p_l->accept(this);
        count += p->expr_p_r->accept(this);
        reduce(tmp, count);
        return 1;
    }

    int visit(UnaryOperator *p) final {
        llvm::json::Value tmp = common("UnaryOperator", std::string(), valueCatagoryToStr(p->valueCatagory), typeToJasonValue(&p->type));
        tmp.getAsObject()->insert({"prefix", p->opcode});
        int count = p->expr_p->accept(this);
        reduce(tmp, count);
        return 1;
    }

    int visit(ParenExpr *p) final {
        llvm::json::Value tmp = common("ParenExpr", std::string(), valueCatagoryToStr(p->valueCatagory), typeToJasonValue(&p->type));
        int count = p->expr_p->accept(this);
        reduce(tmp, count);
        return 1;
    }

    int visit(IntegerLiteral *p) final {
        llvm::json::Value tmp = common("IntegerLiteral", std::string(), valueCatagoryToStr(p->valueCatagory), typeToJasonValue(&p->type));
        tmp.getAsObject()->insert({"value", std::to_string((unsigned long long)p->value)});
        reduce(tmp, 0);
        return 1;
    }

    int visit(StringLiteral *p) final {
        llvm::json::Value tmp = common("StringLiteral", std::string(), valueCatagoryToStr(p->valueCatagory), typeToJasonValue(&p->type));

        tmp.getAsObject()->insert({"value", p->serial});
        reduce(tmp, 0);
        return 1;
    }

    int visit(FloatingLiteral *p) final {
        llvm::json::Value tmp = common("FloatingLiteral", std::string(), valueCatagoryToStr(p->valueCatagory), typeToJasonValue(&p->type));

        llvm::StringRef str(p->serial);
        llvm::APFloat apf(0.0);

        llvm::Expected<llvm::APFloatBase::opStatus> useless = apf.convertFromString(str, llvm::APFloat::rmNearestTiesToEven);
        llvm::SmallString<16> Buffer;
        apf.toString(Buffer);
        std::string value = Buffer.c_str();

        tmp.getAsObject()->insert({"value", value});
        reduce(tmp, 0);
        return 1;
    }

    int visit(array_filler *p) final {
        llvm::json::Value tmp = common("array_filler",
                                       std::string(),
                                       std::string(),
                                       std::nullptr_t());
        reduce(tmp, 0);
        return 1;
    }

    llvm::json::Array stack_; // act like a stack
private:
    llvm::json::Value common(std::string kind,
                             std::string name,
                             // double value,
                             std::string valueCatagory,
                             llvm::json::Value type,
                             llvm::json::Value inner = llvm::json::Array{}) {
#if DEBUG
        printf("visit %s\n", kind.c_str());
#endif
        llvm::json::Object tmp{
            {"kind", kind},
        };
        if (!name.empty()) tmp.insert({"name", name});
        // if (!value.empty()) tmp.insert({"value", value});
        if (!valueCatagory.empty()) tmp.insert({"valueCatagory", valueCatagory});
        if (!type.getAsNull()) tmp.insert({"type", type});
        if (inner.getAsArray()->size() > 0) tmp.insert({"inner", inner});

        return tmp;
    }

    void reduce(llvm::json::Value tmp, int count) {
        if (count > 0) {
            llvm::json::Value inner = llvm::json::Array{};
            for (int i = 1; i <= count; i++) {
                inner.getAsArray()->insert(inner.getAsArray()->begin(), stack_.back());
                stack_.pop_back();
            }

            tmp.getAsObject()->insert({"inner", inner});
        }
        stack_.push_back(tmp);
    }

    std::string typeToString(Type *_type) {
        std::string qualType;
        if (_type->is_const) {
            qualType += "const ";
        }

        switch (_type->base_type) {
        case 0: qualType += "void"; break;
        case 1: qualType += "char"; break;
        case 2: qualType += "int"; break;
        case 3: qualType += "unsigned int"; break;
        case 4: qualType += "long"; break;
        case 5: qualType += "unsigned long"; break; //  4294967295
        case 6: qualType += "long long"; break;     // 2,147,483,647
        case 7: qualType += "float"; break;         //  4,294,967,295
        case 8: qualType += "double"; break;
        default: assert(false);
        }
        if (_type->is_ptr || _type->is_func || _type->dims.size() > 0) {
            qualType += " ";
        }
        if (_type->is_ptr) {
            if (_type->dims.empty() && !_type->is_func) { // is not array
                qualType += "*";
            } else {
                qualType += "(*)";
            }
        }
        if (_type->is_func) {
            qualType += "(";
            if (_type->parm_vars.size() > 0) {
                for (int i = 0; i < _type->parm_vars.size(); i++) {
                    if (i > 0) qualType += ", ";
                    qualType += typeToString(&(_type->parm_vars[i]->type));
                }
            }
            if (_type->is_ellipsis)
                qualType += ", ...";
            qualType += ")";
        }
        int dims_size = _type->dims.size();
        for (int i = 0; i < dims_size; i++) {
            if (_type->dims[i] == 0) {
                qualType += "[]";
            } else {
                qualType += "[" + std::to_string(_type->dims[i]) + "]";
            }
        }
        return qualType;
    }

    llvm::json::Value typeToJasonValue(Type *_type) { // if dissugured
        std::string qualType = typeToString(_type);
        llvm::json::Value tmp = llvm::json::Object{};
        if (_type->is_desugared) {
            tmp.getAsObject()->insert({"desugaredQualType", qualType});
        }
        tmp.getAsObject()->insert({"qualType", qualType});

        return tmp;
    }

    std::string valueCatagoryToStr(Expr::ValueCatagory vc) {
        switch (vc) {
        case 0: return std::string("lvalue");
        case 1: return std::string("prvalue");
        case 2: return std::string("xvalue");
        default: return std::string();
        }
    }

    std::string valueToStr(double value) {
        return std::string();
    }
};

Mgr Mgr::g; // memory manager
TranslationUnitDecl trans_unit_decl;
TranslationUnitDecl *trans_unit_decl_p = &trans_unit_decl; // root
ScopeManager scope_manager;
ScopeManager *scope_manager_p = &scope_manager;

JsonVisitor jv;
#include <unistd.h>

int main(int argc, char const *argv[]) {
    if (argc > 1)
        yydebug = 1;

#if DEBUG
    int useless = sleep(20); // for attach gdb
#endif
    auto p = yyparse();

    trans_unit_decl_p->accept(&jv);
    llvm::outs() << jv.stack_.back() << "\n";
}
