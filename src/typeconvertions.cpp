#include "typeconvertions.h"

#include "ralconsts.h"
#include "ralexceptions.h"

std::string
RaLang::fromSourceTypeName(const std::string &type_name_in_ral_source_code) {
  if (type_name_in_ral_source_code == "цел") {
    return RAL_INT;
  } else if (type_name_in_ral_source_code == "вещ") {
    return RAL_FLOAT;
  } else if (type_name_in_ral_source_code == "") {
    return RAL_VOID;
  } else {
    throw VariableNotFoundException(type_name_in_ral_source_code);
  }
}
