#pragma once

#include <stdexcept>
#include <string>

namespace RaLang {
class VariableNotFoundException : public std::runtime_error {
public:
  VariableNotFoundException(const std::string &name) : std::runtime_error("Variable not found: " + name + ".") {}
};

class FileNotFoundException : public std::runtime_error {
public:
  FileNotFoundException(const std::string &name) : std::runtime_error("File not found: " + name + ".") {}
};

class AlgArgMismatchException : public std::runtime_error {
public:
  AlgArgMismatchException(const std::string &text) : std::runtime_error(text) {}
};

class NotImplementedException : public std::logic_error {
public:
  NotImplementedException(const std::string &message = "")
      : std::logic_error("Functionnality not yet implemented. " + message) {}
};

class ScopeException : public std::logic_error {
public:
  ScopeException(const std::string &message) : std::logic_error(message) {}
};

class InternalException : public std::logic_error {
public:
  InternalException(const std::string &message) : std::logic_error(message) {}
};

class TypePromotionException : public std::logic_error {
public:
  TypePromotionException(const std::string &message) : std::logic_error(message) {}
};

} // namespace RaLang
