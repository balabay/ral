#pragma once

// Entry point name
#define RAL_MAIN "main"
#define RAL_SCOPE_GLOBAL "global"
#define RAL_SCOPE_LOCAL "local"

#define RAL_INT "int"
#define RAL_REAL "real"
#define RAL_VOID "void"

namespace RaLang {
enum class TypeKind : uint8_t { Struct = 0, Array, Boolean, Char, Int, Real, Void };
}

#define RAL_RET_VALUE "_return"
#define RAL_PRINT_CALL "_print"
#define RAL_INPUT_CALL "_input"
