#include "Scope.hpp"

using namespace RaLang;

void Scope::setVariable(const std::string &name, llvm::AllocaInst *v) {
  this->variables[name] = v;
}

llvm::AllocaInst *Scope::getVariable(const std::string &name) {
  try {
    return this->variables.at(name);
  } catch (std::out_of_range) {
    return nullptr;
  }
}
