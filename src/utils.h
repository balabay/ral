#pragma once

#include "ralconsts.h"

#include <algorithm>
#include <cctype>
#include <locale>

namespace RaLang {

// trim from start (in place)
inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

inline void trim(std::string &s) {
  rtrim(s);
  ltrim(s);
}

void unquote(std::string &s);

std::string typeKindToString(TypeKind type);

} // namespace RaLang
