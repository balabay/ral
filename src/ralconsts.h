#pragma once

#include <cstdint>

// Entry point name
#define RAL_MAIN "main"
#define RAL_SCOPE_GLOBAL "global"
#define RAL_SCOPE_LOCAL "local"

#define RAL_INT "int"
#define RAL_REAL "real"
#define RAL_VOID "void"

namespace RaLang {
enum class TypeKind : uint8_t { Void = 0, Array, Boolean, Char, Int, Real, Struct, _COUNT };
}

#define RAL_RET_VALUE "_return"
#define RAL_PRINT_CALL "_print"
#define RAL_INPUT_CALL "_input"
