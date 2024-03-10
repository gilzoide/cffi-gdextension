#include "CffiType.h"
#include "ffi.h"
#include "godot_cpp/core/memory.hpp"

namespace cffi {

CffiType::CffiType() : ffi_type(ffi_type_void) {}
CffiType::CffiType(_ffi_type ffi_type) : ffi_type(ffi_type) {}

void CffiType::_bind_methods() {

}

}