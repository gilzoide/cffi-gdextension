#include "cffi_type.hpp"
#include "cffi.hpp"
#include "cffi_value.hpp"

namespace cffi {

CFFIType::CFFIType() : ffi_handle(ffi_type_void) {}
CFFIType::CFFIType(const String& name, ffi_type ffi_type) : name(name), ffi_handle(ffi_type) {}

ffi_type& CFFIType::get_ffi_type() {
	return ffi_handle;
}

const ffi_type& CFFIType::get_ffi_type() const {
	return ffi_handle;
}

uint64_t CFFIType::get_size() const {
	return ffi_handle.size;
}

int CFFIType::get_alignment() const {
	return ffi_handle.alignment;
}

const String& CFFIType::get_name() const {
	return name;
}

bool CFFIType::get_return_value(const PackedByteArray& data, Variant& r_variant) const {
	if (data.size() < ffi_handle.size) {
		ERR_PRINT_ED(String("Expected at least %d bytes for %s, got %d") % Array::make((uint64_t) ffi_handle.size, name, data.size()));
		return false;
	}

	return get_return_value(data.ptr(), r_variant);
}

bool CFFIType::get_return_value(const uint8_t *ptr, Variant& r_variant) const {
	switch (ffi_handle.type) {
		case FFI_TYPE_VOID:
			r_variant = Variant();
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

bool CFFIType::serialize_value_into(const Variant& value, PackedByteArray& buffer) const {
	int64_t previous_size = buffer.size();
	buffer.resize(previous_size + ffi_handle.size);
	return serialize_value_into(value, buffer.ptrw() + previous_size);
}

bool CFFIType::serialize_value_into(const Variant& value, uint8_t *buffer) const {
	switch (ffi_handle.type) {
		case FFI_TYPE_VOID:
			break;

		case FFI_TYPE_INT:
			*(int *) buffer = value;
			break;

		case FFI_TYPE_FLOAT:
			*(float *) buffer = value;
			break;

		case FFI_TYPE_DOUBLE:
			*(double *) buffer = value;
			break;

#if FFI_TYPE_LONGDOUBLE != FFI_TYPE_DOUBLE
		case FFI_TYPE_LONGDOUBLE:
			*(long double *) buffer = (double) value;
			break;
#endif

		case FFI_TYPE_UINT8:
			*(uint8_t *) buffer = (uint64_t) value;
			break;

		case FFI_TYPE_SINT8:
			*(int8_t *) buffer = (int64_t) value;
			break;

		case FFI_TYPE_UINT16:
			*(uint16_t *) buffer = (uint64_t) value;
			break;

		case FFI_TYPE_SINT16:
			*(int16_t *) buffer = (int64_t) value;
			break;

		case FFI_TYPE_UINT32:
			*(uint32_t *) buffer = (uint64_t) value;
			break;

		case FFI_TYPE_SINT32:
			*(int32_t *) buffer = (int64_t) value;
			break;

		case FFI_TYPE_UINT64:
			*(uint64_t *) buffer = (uint64_t) value;
			break;

		case FFI_TYPE_SINT64:
			*(int64_t *) buffer = (int64_t) value;
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

Ref<CFFIValue> CFFIType::alloc(bool initialize_with_zeros) {
	return memnew(CFFIValue(this, initialize_with_zeros));
}

Ref<CFFIType> CFFIType::from_variant(const Variant& var) {
	CFFIType *type = Object::cast_to<CFFIType>(var);
	return type ? type : CFFI::get_singleton()->get_type(var);
}

void CFFIType::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_name"), &CFFIType::get_name);
	ClassDB::bind_method(D_METHOD("get_size"), &CFFIType::get_size);
	ClassDB::bind_method(D_METHOD("get_alignment"), &CFFIType::get_alignment);
	ClassDB::bind_method(D_METHOD("alloc", "initialize_with_zeros"), &CFFIType::alloc, DEFVAL(true));
}

String CFFIType::_to_string() const {
	return String("[%s:%s]") % Array::make(get_class(), name);
}

}