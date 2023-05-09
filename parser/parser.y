%{
#include "relexer.hpp"

%}
%code requires {
#include "asg.hpp"
}

%union {
  TranslationUnitDecl* TranslationUnitDecl;
  ExternalDecl* ExternalDecl;
  InitDeclList* InitDeclList;
  InitDecltorList* InitDecltorList;
  InitDecltor* InitDecltor;
  Decltor* Decltor;
  VarDecltor* VarDecltor;
  FunctionDecltor* FunctionDecltor;
  VarDecl* VarDecl;
  FunctionDecl* FunctionDecl;
  ParmVarDeclList* ParmVarDeclList;
  ParmVarDecl* ParmVarDecl;
  CompoundStmt* CompoundStmt;
  StmtList* StmtList;
  Stmt* Stmt;
  ReturnStmt* ReturnStmt;
  DeclStmt* DeclStmt;
  ExprStmt* ExprStmt;
  IfStmt* IfStmt;
  WhileStmt* WhileStmt;
  DoStmt* DoStmt;
  BreakStmt* BreakStmt;
  ContinueStmt* ContinueStmt;
  NullStmt* NullStmt;
  Expr* Expr;
  DeclRefExpr* DeclRefExpr;
  CallExpr* CallExpr;
  ExprList* ExprList;
  InitListExpr* InitListExpr;
  StringLiteral* StringLiteral;
  TypeSpec* TypeSpec;
  Type::BaseType BaseType;
  Str* Str;
}

%nterm <TranslationUnitDecl>  translation_unit_decl
%nterm <ExternalDecl>         external_decl
%nterm <InitDeclList>         init_decl_list
%nterm <InitDecltorList>      init_decltor_list
%nterm <InitDecltor>          init_decltor
%nterm <Decltor>              decltor
%nterm <VarDecltor>           var_decltor
%nterm <FunctionDecltor>      function_decltor
%nterm <FunctionDecl>         function_decl function_head
%nterm <ParmVarDeclList>      parm_var_decl_list
%nterm <ParmVarDecl>          parm_var_decl
%nterm <TypeSpec>             type_spec
%nterm <BaseType>             base_type
%nterm <CompoundStmt>         compound_stmt
%nterm <StmtList>             stmt_list
%nterm <Stmt>                 stmt control_stmt      
%nterm <ReturnStmt>           return_stmt
%nterm <DeclStmt>             decl_stmt
%nterm <ExprStmt>             expr_stmt
%nterm <IfStmt>               if_stmt
%nterm <WhileStmt>            while_stmt
%nterm <DoStmt>               do_stmt
%nterm <BreakStmt>            break_stmt
%nterm <ContinueStmt>         continue_stmt
%nterm <NullStmt>             null_stmt
%nterm <Expr>                 expr primary_expr postfix_expr unary_expr mul_div_expr
%nterm <Expr>                 add_sub_expr relational_expr equality_expr logical_and_expr
%nterm <Expr>                 logical_or_expr assign_expr literal
%nterm <DeclRefExpr>          decl_ref_expr
%nterm <CallExpr>             call_expr
%nterm <ExprList>             expr_list
%nterm <InitListExpr>         init_list_expr
%nterm <Str>                  relational_op  unary_op  mul_div_op
%nterm <StringLiteral>        string_literal;


%token L_PAREN R_PAREN L_BRACE R_BRACE L_SQUARE R_SQUARE COMMA SEMI ELLIPSIS
%token PLUS MINUS SLASH STAR PERCENT // + - * / %
%token EQUAL LESS GREATER LESSEQUAL GREATEREQUAL EQUALEQUAL EXCLAIMEQUAL
%token EXCLAIM TILDE AMP AMPAMP PIPEPIPE// ! ~ & && ||
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token <Str> NUMERIC_CONSTANT IDENTIFIER STRING_LITERAL
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%start translation_unit_decl
%%

//  to manage scope layer
l_paren
  : L_PAREN {
    scope_manager_p->_state_stack.push_back(ScopeManager::PARM_READY_ST);
  }
r_paren
  : R_PAREN {
    if (!(scope_manager_p->_state_stack.back() == ScopeManager::PARM_CONTINUE_ST)) {
      scope_manager_p->_state_stack.pop_back();
    }
  }
l_brace
  : L_BRACE {
    if (scope_manager_p->_state_stack.back() == ScopeManager::PARM_CONTINUE_ST) {
        scope_manager_p->_state_stack.back() = ScopeManager::CONTINUE_ST;
    } else {
        scope_manager_p->_state_stack.push_back(ScopeManager::READY_ST);
    }
  }
r_brace 
  : R_BRACE {
    if (scope_manager_p->_state_stack.back() == ScopeManager::CONTINUE_ST) {
        scope_manager_p->finalizeScope();
    }
    scope_manager_p->_state_stack.pop_back();
  }

semi
  : SEMI {
    if (scope_manager_p->_state_stack.back() == ScopeManager::PARM_CONTINUE_ST){
      scope_manager_p->finalizeScope();
      scope_manager_p->_state_stack.pop_back();
    }
    else if(scope_manager_p->_state_stack.back() == ScopeManager::PARM_READY_ST){
        scope_manager_p->_state_stack.pop_back();
    }
  }

translation_unit_decl
	: external_decl {
    trans_unit_decl_p->push_back($1);
  }
	| translation_unit_decl external_decl {
    trans_unit_decl_p->push_back($2);
  }
	;

external_decl
  : init_decl_list {
    $$ = Mgr::g.make<ExternalDecl>(0,$1,std::nullptr_t());
  }
  | function_decl {
    $$ = Mgr::g.make<ExternalDecl>(1,std::nullptr_t(),$1);

  }



init_decl_list // c's tricky grammer
  : type_spec init_decltor_list semi {
    $$ = Mgr::g.make<InitDeclList>($1->type,$2);
    
  }

init_decltor_list
  : init_decltor {
    $$ = Mgr::g.make<InitDecltorList>();
    $$->push_back($1);
  }
  | init_decltor_list COMMA init_decltor {
    $1->push_back($3);
    $$ = $1;
  }

init_decltor
  : decltor {
    $$ = Mgr::g.make<InitDecltor>(0,$1,std::nullptr_t()); 
  }
  | decltor EQUAL expr {
    $$ = Mgr::g.make<InitDecltor>(1,$1,$3);   
  }


decltor
  : var_decltor {
    $$ = $1;
    $$->flag = 0;
  }
  | function_decltor {
    $$ = $1;
    $$->flag = 1;
  }

var_decltor
  : IDENTIFIER{
    $$ = Mgr::g.make<VarDecltor>($1->s);
    $$->is_ptr = false;
    delete $1;
  }
  | var_decltor L_SQUARE R_SQUARE{
    $$ = $1;
    //$$->is_ptr = true;
    $$->dims.push_back(0);// in parmdecl, it becomnse *
  }
  | var_decltor L_SQUARE expr R_SQUARE{
    $$ = $1;
    $$->dims.push_back($3->value);
  }

function_decltor
  : IDENTIFIER l_paren parm_var_decl_list r_paren {
    $$ = Mgr::g.make<FunctionDecltor>(1,$1->s,$3);
    delete $1;
  }
  | IDENTIFIER l_paren parm_var_decl_list COMMA ELLIPSIS r_paren {
    $$ = Mgr::g.make<FunctionDecltor>(1,$1->s,$3);
    $$->type.is_ellipsis = true;
    delete $1;
  }
  | IDENTIFIER l_paren r_paren {
    $$ = Mgr::g.make<FunctionDecltor>(0,$1->s,std::nullptr_t());
    delete $1;
  }


parm_var_decl_list
  : parm_var_decl {
    $$ = Mgr::g.make<ParmVarDeclList>();
    $$->push_back($1);
  }
  | parm_var_decl_list COMMA parm_var_decl {
    $1->push_back($3);
    $$ = $1;
  }

parm_var_decl
  : type_spec var_decltor {
    $$ = Mgr::g.make<ParmVarDecl>( $1->type
                                      ,$2);
  }

function_head
  : type_spec function_decltor {
    $$ = Mgr::g.make<FunctionDecl>(0,std::nullptr_t(),$2,$1->type.base_type);
  }

function_decl
  : function_head compound_stmt {
    $$ = $1;
    $$->flag = 1;
    $$->compound_stmt_p = $2;
  }

compound_stmt
  : l_brace r_brace {
    $$ = Mgr::g.make<CompoundStmt>(0,std::nullptr_t());
  }
  | l_brace stmt_list r_brace {
    $$ = Mgr::g.make<CompoundStmt>(1,$2);
  }

stmt_list
  : stmt {
    $$ = Mgr::g.make<StmtList>();
    $$->push_back($1);
  }
  | stmt_list stmt{
    $1->push_back($2);
    $$ = $1;
  }

stmt 
  : return_stmt {
    $$ = $1; 
  }
  | expr_stmt{
    $$ = $1;
  }
  | decl_stmt{
    $$ = $1;
  }
  | compound_stmt {
    $$ = $1;
  }
  | control_stmt {
    $$ = $1;
  }
  | null_stmt {
    $$ = $1;
  }
  

return_stmt
  : RETURN expr semi 
  {
    $$ = Mgr::g.make<ReturnStmt>(1,$2);
  }
  | RETURN semi {
    $$ = Mgr::g.make<ReturnStmt>(0,std::nullptr_t());
  }




expr_stmt
  : expr semi {
    $$ = Mgr::g.make<ExprStmt>($1);

  }

decl_stmt
  : init_decl_list {
    $$ = Mgr::g.make<DeclStmt>($1);  
  }

control_stmt
  : if_stmt{
    $$ = $1;
  }
  | while_stmt{
    $$ = $1;
  }
  | break_stmt{
    $$ = $1;
  }
  | continue_stmt{
    $$ = $1;
  }
  | do_stmt {
    $$ = $1;
  }


if_stmt
  : IF l_paren expr r_paren stmt {
    $$ = Mgr::g.make<IfStmt>(0,$3,$5,std::nullptr_t());
  } 
  | IF l_paren expr r_paren stmt ELSE stmt {
    $$ = Mgr::g.make<IfStmt>(1,$3,$5,$7);
  }

while_stmt
  : WHILE l_paren expr r_paren stmt{
    $$ = Mgr::g.make<WhileStmt>($3,$5);
  } 

do_stmt
  : DO stmt WHILE l_paren expr r_paren SEMI{
    $$ = Mgr::g.make<DoStmt>($5,$2);
  } 

break_stmt
  : BREAK semi{
  $$ = Mgr::g.make<BreakStmt>();
} 

continue_stmt
  : CONTINUE semi{
  $$ = Mgr::g.make<ContinueStmt>();
}  

null_stmt
  : semi{
  $$ = Mgr::g.make<NullStmt>();
}

///////////////////////////////////////////EXPR///////////////////////////////////


primary_expr
	: decl_ref_expr { $$ = $1;}
  | literal { $$ = $1;}
	| l_paren expr r_paren { 
    $$ = Mgr::g.make<ParenExpr>($2);
  }
  | init_list_expr { $$ = $1;}
  | call_expr { $$ = $1; }
	;

postfix_expr
	: primary_expr { $$ = $1;}
	| postfix_expr L_SQUARE expr R_SQUARE {
    $$ = Mgr::g.make<ArraySubscriptExpr>(1,$1,$3);
  }
  | postfix_expr L_SQUARE R_SQUARE {
    $$ = Mgr::g.make<ArraySubscriptExpr>(0,$1,std::nullptr_t());
  }
  /*
	| postfix_expr l_paren r_paren
  */
	;

unary_expr
	: postfix_expr {$$ = $1;}
  | unary_op unary_expr{
    $$ = Mgr::g.make<UnaryOperator>( $1->s,
                                      $2);
    delete $1;
  }
	;

unary_op
	: AMP {$$ = new Str("&");}
  | STAR {$$ = new Str("*");}
	| PLUS {$$ = new Str("+");}
	| MINUS {$$ = new Str("-");}
	| TILDE {$$ = new Str("~");}
	| EXCLAIM {$$ = new Str("!");}
	;




mul_div_expr
	: unary_expr { $$ = $1;}
	| mul_div_expr mul_div_op unary_expr {
    $$ = Mgr::g.make<BinaryOperator>( $2->s,
                                          $1,$3);
    delete $2;
  }
	;

mul_div_op
  : STAR{ $$ = new Str("*");}
  | SLASH{ $$ = new Str("/");}
  | PERCENT { $$ = new Str("%");}


add_sub_expr
	: mul_div_expr {$$ = $1;}
	| add_sub_expr PLUS mul_div_expr {
    $$ = Mgr::g.make<BinaryOperator>( std::string("+"),
                                          $1,$3);
  }
	| add_sub_expr MINUS mul_div_expr{
    $$ = Mgr::g.make<BinaryOperator>( std::string("-"),
                                          $1,$3);
  }
	;


relational_expr
	: add_sub_expr {$$ = $1;}
	| relational_expr relational_op add_sub_expr {
    $$ = Mgr::g.make<BinaryOperator>( $2->s,
                                          $1,$3);
    delete $2;
  }
	;

relational_op
  : GREATER {$$ = new Str(">");}
  | LESS    {$$ = new Str("<");} 
  | GREATEREQUAL {$$ = new Str(">=");}
  | LESSEQUAL    {$$ = new Str("<=");}


equality_expr
	: relational_expr{$$ = $1;}
  | equality_expr EQUALEQUAL relational_expr{
    $$ = Mgr::g.make<BinaryOperator>( std::string("=="),
                                      $1,$3);
  }
	| equality_expr EXCLAIMEQUAL relational_expr{
    $$ = Mgr::g.make<BinaryOperator>( std::string("!="),
                                      $1,$3);
  }
	;



logical_and_expr
	: equality_expr{$$ = $1;}
	| logical_and_expr AMPAMP equality_expr{
    $$ = Mgr::g.make<BinaryOperator>( std::string("&&"),
                                  $1,$3);
  }
  
	;

logical_or_expr
	: logical_and_expr{$$ = $1;}
	| logical_or_expr PIPEPIPE logical_and_expr {
    $$ = Mgr::g.make<BinaryOperator>( std::string("||"),
                                  $1,$3);
  }
  

assign_expr
	: logical_or_expr {$$ = $1;}
	| unary_expr assign_operator logical_or_expr{
    $$ = Mgr::g.make<BinaryOperator>( std::string("="),
                                          $1,
                                          $3);
  }
	;

assign_operator
	: EQUAL // '='
  /*
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
  */
	;

expr
  : assign_expr {$$ = $1; }
  


decl_ref_expr
  : IDENTIFIER {
    $$ = Mgr::g.make<DeclRefExpr>($1->s);
    delete $1;
  }

call_expr
  : decl_ref_expr l_paren r_paren {
    $$ = Mgr::g.make<CallExpr>(0,$1,std::nullptr_t());
  }
  | decl_ref_expr l_paren expr_list r_paren {
    $$ = Mgr::g.make<CallExpr>(1,$1,$3);
  }

expr_list
  : expr {
    $$ = Mgr::g.make<ExprList>();
    $$->push_back($1);
  }
  | expr_list COMMA expr {
    $1->push_back($3);
    $$ = $1;
  }


init_list_expr
  : l_brace expr_list r_brace {
    $$ = Mgr::g.make<InitListExpr>($2);
  }
  | l_brace r_brace {
    $$ = Mgr::g.make<InitListExpr>(Mgr::g.make<ExprList>());
  }

literal
  : NUMERIC_CONSTANT {
    std::string serial = $1->s;
    if (serial.find('.') != std::string::npos || 
        serial.find('p') != std::string::npos ||
        serial.find('e') != std::string::npos) {
        $$ = Mgr::g.make<FloatingLiteral>(serial);
    }
    else{
      $$ = Mgr::g.make<IntegerLiteral>(serial);
    }
    delete $1;
  }
  | string_literal {
    $1->get_count();
    $$ = $1;
  }

string_literal 
  : STRING_LITERAL {
    $$ = Mgr::g.make<StringLiteral>();
    $$->serial = $1->s;
  }
  | string_literal STRING_LITERAL {
    $1->serial = $1->serial.substr(0, $1->serial.size() - 1) + $2->s.substr(1, $2->s.size() - 1);
    $$ = $1;
    delete $2;
  }

type_spec
  : base_type {
    $$ = Mgr::g.make<TypeSpec>($1,false);
  }
  | CONST type_spec {
    $$ = $2;
    $$->type.is_const = true;
  }

base_type
  : INT {
    $$ = Type::INT;
  }
  | VOID {
    $$ = Type::VOID;
  }
  | FLOAT {
    $$ = Type::FLOAT;
  }
  | CHAR {
    $$ = Type::CHAR;
  }
  | LONG {
    $$ = Type::LONG;
  }
  | LONG LONG {
    $$ = Type::LONG_LONG;
  }

%%


