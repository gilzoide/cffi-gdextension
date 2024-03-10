#include "CffiType.hpp"

namespace cffi {

CffiType::CffiType() : ffi_type(ffi_type_void) {}
CffiType::CffiType(_ffi_type ffi_type) : ffi_type(ffi_type) {}

ffi_type& CffiType::get_ffi_type() {
    return ffi_type;
}

const ffi_type& CffiType::get_ffi_type() const {
    return ffi_type;
}

void CffiType::_bind_methods() {}

}