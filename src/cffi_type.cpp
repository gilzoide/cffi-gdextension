#include "cffi_type.hpp"
#include "cffi.hpp"

namespace cffi {

FFIType::FFIType() : ffi_type(ffi_type_void) {}
FFIType::FFIType(const String& name, _ffi_type ffi_type) : name(name), ffi_type(ffi_type) {}

ffi_type& FFIType::get_ffi_type() {
	return ffi_type;
}

const ffi_type& FFIType::get_ffi_type() const {
	return ffi_type;
}

const String& FFIType::get_name() const {
	return name;
}

FFIType *FFIType::from_variant(const Variant& var) {
	FFIType *type = Object::cast_to<FFIType>(var);
	return type ? type : FFI::get_singleton()->get_type(var);
}

void FFIType::_bind_methods() {}

String FFIType::_to_string() const {
	return String("[%s:%s]") % Array::make(get_class(), name);
}

}