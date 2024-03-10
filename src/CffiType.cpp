#include "CffiType.hpp"

namespace cffi {

CffiType::CffiType() : ffi_type(ffi_type_void) {}
CffiType::CffiType(const String& name, _ffi_type ffi_type) : name(name), ffi_type(ffi_type) {}

ffi_type& CffiType::get_ffi_type() {
    return ffi_type;
}

const ffi_type& CffiType::get_ffi_type() const {
    return ffi_type;
}

const String& CffiType::get_name() const {
    return name;
}

void CffiType::_bind_methods() {}

String CffiType::_to_string() const {
    return String("[%s:%s]") % Array::make(get_class(), name);
}

}