#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/JSON.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>
// #include "deserializeAsg.hpp"
#include "non_recursive_dAsg.hpp"
#include "codegenVt.hpp"
#include <iostream>
#include <unistd.h>
#include <iostream>
#define DEBUG 0

Mgr Mgr::g;
FileSignal FileSignal::f;

int main(int argc, char const *argv[])
{
  if (argc > 1)
  {
    int useless = sleep(10);
  }
  auto llvmin = llvm::MemoryBuffer::getFileOrSTDIN("-");
  auto json = llvm::json::parse(llvmin.get()->getBuffer());

  // signal
  CodegenVisitor cv;
  Stmt *ParseStmt = deserializeJson(json->getAsObject())->dcast<TranslationUnitDecl>();
  if (TranslationUnitDecl *trans_unit_decl_p = ParseStmt->dcast<TranslationUnitDecl>())
  {
    trans_unit_decl_p->accept(&cv);
    cv.print();
  }
  else
  {
    std::fstream file(FileSignal::f.CheatIRFilePath);
    if (file.is_open())
    {
      std::string line;
      while (getline(file, line))
      {
        llvm::outs() << line << "\n";
      }
      file.close();
    }

    return 0;
  }
}
