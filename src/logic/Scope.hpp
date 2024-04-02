#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <string>
#include <unordered_map>

namespace RaLang {
class Scope {
public:
  std::unordered_map<std::string, llvm::AllocaInst *> variables;

  llvm::Function *currentFunction = nullptr;

  Scope(llvm::Function *_currentFunction = nullptr)
      : currentFunction(_currentFunction) {}

  void setVariable(const std::string &name, llvm::AllocaInst * v);

  llvm::AllocaInst * getVariable(const std::string &name);
};
} // namespace RaLang
