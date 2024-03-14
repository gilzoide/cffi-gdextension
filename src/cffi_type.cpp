#include "cffi_type.hpp"
#include "cffi.hpp"

namespace cffi {

FFIType::FFIType() : ffi_handle(ffi_type_void) {}
FFIType::FFIType(const String& name, ffi_type ffi_type) : name(name), ffi_handle(ffi_type) {}

ffi_type& FFIType::get_ffi_type() {
	return ffi_handle;
}

const ffi_type& FFIType::get_ffi_type() const {
	return ffi_handle;
}

const String& FFIType::get_name() const {
	return name;
}

bool FFIType::get_return_value(const PackedByteArray& data, Variant& r_variant) const {
	if (data.size() < ffi_handle.size) {
		ERR_PRINT_ED(String("Expected at least %d bytes for %s, got %d") % Array::make((uint64_t) ffi_handle.size, name, data.size()));
		return false;
	}

	const void *ptr = data.ptr();
	switch (ffi_handle.type) {
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

bool FFIType::serialize_value_into(const Variant& value, Ref<StreamPeerBuffer> buffer) const {
	switch (ffi_handle.type) {
		case FFI_TYPE_VOID:
			break;

		case FFI_TYPE_INT:
			buffer->put_32(value);
			break;

		case FFI_TYPE_FLOAT:
			buffer->put_float(value);
			break;

		case FFI_TYPE_DOUBLE:
			buffer->put_double(value);
			break;

#if FFI_TYPE_LONGDOUBLE != FFI_TYPE_DOUBLE
		case FFI_TYPE_LONGDOUBLE:
			buffer->put_double((double) value);
			break;
#endif

		case FFI_TYPE_UINT8:
			buffer->put_u8((uint64_t) value);
			break;

		case FFI_TYPE_SINT8:
			buffer->put_8((int64_t) value);
			break;

		case FFI_TYPE_UINT16:
			buffer->put_u16((uint64_t) value);
			break;

		case FFI_TYPE_SINT16:
			buffer->put_16((int64_t) value);
			break;

		case FFI_TYPE_UINT32:
			buffer->put_u32((uint64_t) value);
			break;

		case FFI_TYPE_SINT32:
			buffer->put_32((int64_t) value);
			break;

		case FFI_TYPE_UINT64:
			buffer->put_u64((uint64_t) value);
			break;

		case FFI_TYPE_SINT64:
			buffer->put_64((int64_t) value);
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

Ref<FFIType> FFIType::from_variant(const Variant& var) {
	FFIType *type = Object::cast_to<FFIType>(var);
	return type ? type : FFI::get_singleton()->get_type(var);
}

void FFIType::_bind_methods() {}

String FFIType::_to_string() const {
	return String("[%s:%s]") % Array::make(get_class(), name);
}

}