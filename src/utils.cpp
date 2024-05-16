#include "utils.h"

#include <cassert>
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
  } else {
    assert(false);
  }
  s = std::regex_replace(s, std::regex("''"), "'");
}

} // namespace RaLang
