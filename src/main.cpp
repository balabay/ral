#include "CLIManager.hpp"
#include "grammar/Visitor.hpp"

#include <llvm/Support/TargetSelect.h>

int main(int argc, char **argv) {
  RaLang::CLIManager cli(argc, argv);

  if (argc < 2 || cli.hasOption("--help") || cli.hasOption("-h")) {
    std::cerr << "Usage: compiler <file> [--help,-h] [--print-llvm] "
              << std::endl;
    return 1;
  }

  RaLang::Visitor visitor;
  visitor.fromFile(argv[1]);

  if (cli.hasOption("--print-llvm")) {
    visitor.module->print(llvm::outs(), nullptr);
    std::cout << std::endl;
  }

  return 0;
}
