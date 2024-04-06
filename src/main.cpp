#include "grammar/Visitor.hpp"

int main(int argc, char **argv) {
  RaLang::Visitor visitor;
  visitor.fromFile(argv[1]);
  visitor.module->print(llvm::outs(), nullptr);
  std::cout << std::endl;
}
