#include "Scope.hpp"

using namespace RaLang;

ValueInst Scope::setVariable(const std::string &name, ValueInst vi) {
  this->variables[name] = vi;
  return vi;
}

ValueInst Scope::getVariable(const std::string &name) {
  try {
    return this->variables.at(name);
  } catch (std::out_of_range) {
    return {nullptr, nullptr};
  }
}
