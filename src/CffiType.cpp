#include "CffiType.hpp"

namespace cffi {

CffiType::CffiType() : ffi_type(ffi_type_void) {}
CffiType::CffiType(_ffi_type ffi_type) : ffi_type(ffi_type) {}

void CffiType::_bind_methods() {}

}