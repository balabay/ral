#pragma once

#include <stdexcept>
#include <string>

namespace RaLang {
class VariableNotFoundException : public std::runtime_error {
public:
  VariableNotFoundException(const std::string &name)
      : std::runtime_error("Variable not found: " + name + ".") {}
};

class NotImplementedException : public std::logic_error {
public:
  NotImplementedException()
      : std::logic_error("Functionnality not yet implemented.") {}
};

class ScopeException : public std::logic_error {
public:
  ScopeException(const std::string &message) : std::logic_error(message) {}
};

} // namespace RaLang