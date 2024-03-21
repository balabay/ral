#include <stdexcept>
#include <string>

namespace RaLang {
class VariableNotFoundException : public std::runtime_error {
public:
  VariableNotFoundException(std::string name)
      : std::runtime_error("Variable not found: " + name + ".") {}
};
} // namespace RaLang
