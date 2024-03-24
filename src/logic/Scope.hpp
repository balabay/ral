#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <string>
#include <unordered_map>

using ValueInst = std::pair<llvm::Value *, llvm::AllocaInst *>;
namespace RaLang {
class Scope {
public:
  std::unordered_map<std::string, ValueInst> variables;

  llvm::Function *currentFunction = nullptr;

  Scope(llvm::Function *_currentFunction = nullptr)
      : currentFunction(_currentFunction) {}

  ValueInst setVariable(const std::string &name, ValueInst vi);

  ValueInst getVariable(const std::string &name);
};
} // namespace RaLang
