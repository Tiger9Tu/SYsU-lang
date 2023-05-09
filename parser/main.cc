#include "parser.hh"
#include "jsonVt.hpp"
#include <cassert>

#include <iostream>
#define DEBUG 0

Mgr Mgr::g; // memory manager
TranslationUnitDecl trans_unit_decl;
TranslationUnitDecl *trans_unit_decl_p = &trans_unit_decl; // root
ScopeManager scope_manager;
ScopeManager *scope_manager_p = &scope_manager;
// CodegenVisitor cgv;
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (argc > 1)
        yydebug = 1;

#if DEBUG
    int useless = sleep(20); // for attach gdb
#endif
    auto p = yyparse();

    JsonVisitor jv;
    trans_unit_decl_p->accept(&jv);
    jv.print();
}
