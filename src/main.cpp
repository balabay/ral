#include "logic/compiler.h"

int main(int argc, char **argv) {
  RaLang::Compiler compiler;
  compiler.addSource(argv[1]);
  compiler.compile();
}
