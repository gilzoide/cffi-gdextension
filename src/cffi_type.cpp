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

bool FFIType::get_return_value(const PackedByteArray& data, Variant& r_variant) const {
	if (data.size() < ffi_type.size) {
		ERR_PRINT_ED(String("Expected at least %d bytes for %s, got %d") % Array::make((uint64_t) ffi_type.size, name, data.size()));
		return false;
	}

	const void *ptr = data.ptr();
	switch (ffi_type.type) {
		case FFI_TYPE_VOID:
			r_variant =  Variant();
			break;

		case FFI_TYPE_INT:
			r_variant = *(int *) ptr;
			break;

		case FFI_TYPE_FLOAT:
			r_variant = *(float *) ptr;
			break;

		case FFI_TYPE_DOUBLE:
			r_variant = *(double *) ptr;
			break;

#if FFI_TYPE_LONGDOUBLE != FFI_TYPE_DOUBLE
		case FFI_TYPE_LONGDOUBLE:
			r_variant = (double) *(long double *) ptr;
			break;
#endif

		case FFI_TYPE_UINT8:
			r_variant = *(uint8_t *) ptr;
			break;

		case FFI_TYPE_SINT8:
			r_variant = *(int8_t *) ptr;
			break;

		case FFI_TYPE_UINT16:
			r_variant = *(uint16_t *) ptr;
			break;

		case FFI_TYPE_SINT16:
			r_variant = *(int16_t *) ptr;
			break;

		case FFI_TYPE_UINT32:
			r_variant = *(uint32_t *) ptr;
			break;

		case FFI_TYPE_SINT32:
			r_variant = *(int32_t *) ptr;
			break;

		case FFI_TYPE_UINT64:
			r_variant = *(uint64_t *) ptr;
			break;

		case FFI_TYPE_SINT64:
			r_variant = *(int64_t *) ptr;
			break;

		case FFI_TYPE_STRUCT:
			ERR_FAIL_V_EDMSG(false, "Struct type is not supported yet");

		case FFI_TYPE_POINTER:
			ERR_FAIL_V_EDMSG(false, "Pointer type is not supported yet");

		case FFI_TYPE_COMPLEX:
			ERR_FAIL_V_EDMSG(false, "Complex type is not supported yet");
	}
	return true;
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