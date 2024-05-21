#include "utils.h"

#include <cassert>
#include <iterator>
#include <regex>
#include <string>

namespace RaLang {

void unquote(std::string &s) {
  assert(s.size() >= 2);
  if (s.size() < 2)
    return;
  if ((s[0] == '\'') && (s[s.size() - 1] == '\'')) {
    s.erase(0, 1);
    s.erase(s.size() - 1, 1);
    s = std::regex_replace(s, std::regex("''"), "'");
  } else if ((s[0] == '"') && (s[s.size() - 1] == '"')) {
    s.erase(0, 1);
    s.erase(s.size() - 1, 1);
    s = std::regex_replace(s, std::regex("\"\""), "\"");
  } else {
    assert(false);
  }
}

static const char *TYPE_KIND_STRINGS[] = {"Void", "Array", "Boolean", "Char", "Int", "Real", "Struct"};

static_assert(std::size(TYPE_KIND_STRINGS) == static_cast<size_t>(TypeKind::_COUNT),
              "TYPE_KIND_STRINGS must match TypeKind");

const char *typeKindToString(TypeKind type) {
  size_t l = static_cast<size_t>(type);
  return l >= 0 && l < static_cast<size_t>(TypeKind::_COUNT) ? TYPE_KIND_STRINGS[l] : nullptr;
}

} // namespace RaLang
