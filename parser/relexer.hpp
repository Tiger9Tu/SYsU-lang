#include "parser.hh"
#include <llvm/Support/JSON.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>
#include <string.h>

namespace {
auto llvmin = llvm::MemoryBuffer::getFileOrSTDIN("-");
auto input = llvmin.get() -> getBuffer();
auto end = input.end(), it = input.begin();
auto wk_getline(char endline = "\n"[0]) {
    auto beg = it;
    while (it != end && *it != endline)
        ++it;
    auto len = it - beg;
    if (it != end && *it == endline)
        ++it;
    return llvm::StringRef(beg, len);
}

llvm::json::Array stak;
} // namespace

int yylex(void) {
    auto tk = wk_getline();
    auto b = tk.find("'") + 1, e = tk.rfind("'");
    auto s = tk.substr(b, e - b).str(), t = tk.substr(0, tk.find(" ")).str();

    if (t == "numeric_constant") {
        yylval.Str = new Str(s);
        return NUMERIC_CONSTANT; // warning
    }
    if (t == "string_literal") {
        yylval.Str = new Str(s);

        return STRING_LITERAL; // warning
    }
    if (t == "identifier") {
        yylval.Str = new Str(s);
        return IDENTIFIER;
    }
    if (t == "int") return INT;
    if (t == "void") return VOID;
    if (t == "float") return FLOAT;
    if (t == "char") {
        return CHAR;
    }
    if (t == "long") return LONG;

    if (t == "return") return RETURN;
    if (t == "semi") {
        // scope_manager_p->state = ScopeManager::CONTINUE_ST;
        return SEMI;
    }
    if (t == "comma") return COMMA;
    if (t == "l_paren") {
        return L_PAREN;
    }
    if (t == "r_paren") {
        return R_PAREN;
    }
    if (t == "l_brace") {
        return L_BRACE;
    }
    if (t == "r_brace") {
        return R_BRACE;
    }
    if (t == "l_square") return L_SQUARE;
    if (t == "r_square") return R_SQUARE;
    if (t == "equal") return EQUAL;

    if (t == "star") return STAR;       // *
    if (t == "slash") return SLASH;     // / DIV
    if (t == "percent") return PERCENT; // %
    if (t == "plus") return PLUS;
    if (t == "minus") return MINUS;
    if (t == "exclaim") return EXCLAIM;   // !
    if (t == "tilde") return TILDE;       //~
    if (t == "amp") return AMP;           //&
    if (t == "ellipsis") return ELLIPSIS; //...
    if (t == "greater") return GREATER;
    if (t == "less") return LESS;
    if (t == "greaterequal") return GREATEREQUAL;
    if (t == "lessequal") return LESSEQUAL;

    if (t == "equalequal") return EQUALEQUAL;
    if (t == "exclaimequal") return EXCLAIMEQUAL;
    if (t == "ampamp") return AMPAMP; //&&
    if (t == "pipepipe") return PIPEPIPE;

    if (t == "const") return CONST;

    if (t == "if") return IF;
    if (t == "else") return ELSE;
    if (t == "while") return WHILE;
    if (t == "do") return DO;
    if (t == "break") return BREAK;
    if (t == "continue") return CONTINUE;

    // printf("unrecognized token %s\n",t.c_str());
    return YYEOF;
}

void yyerror(char const *s) {
    fprintf(stderr, "%s\n", s);
}