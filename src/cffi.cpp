#include "cffi.hpp"
#include "cffi_library_handle.hpp"
#include "cffi_pointer.hpp"
#include "cffi_type.hpp"

#include <godot_cpp/classes/engine.hpp>

namespace cffi {

CFFI::CFFI() {
	defined_types["void"] = Ref<CFFIType>(memnew(CFFIType("void", ffi_type_void)));
	defined_types["uint8"] = Ref<CFFIType>(memnew(CFFIType("uint8", ffi_type_uint8)));
	defined_types["sint8"] = Ref<CFFIType>(memnew(CFFIType("sint8", ffi_type_sint8)));
	defined_types["uint16"] = Ref<CFFIType>(memnew(CFFIType("uint16", ffi_type_uint16)));
	defined_types["sint16"] = Ref<CFFIType>(memnew(CFFIType("sint16", ffi_type_sint16)));
	defined_types["uint32"] = Ref<CFFIType>(memnew(CFFIType("uint32", ffi_type_uint32)));
	defined_types["sint32"] = Ref<CFFIType>(memnew(CFFIType("sint32", ffi_type_sint32)));
	defined_types["uint64"] = Ref<CFFIType>(memnew(CFFIType("uint64", ffi_type_uint64)));
	defined_types["sint64"] = Ref<CFFIType>(memnew(CFFIType("sint64", ffi_type_sint64)));
	defined_types["float"] = Ref<CFFIType>(memnew(CFFIType("float", ffi_type_float)));
	defined_types["double"] = Ref<CFFIType>(memnew(CFFIType("double", ffi_type_double)));
	defined_types["pointer"] = Ref<CFFIType>(memnew(CFFIType("pointer", ffi_type_pointer)));
	defined_types["long double"] = Ref<CFFIType>(memnew(CFFIType("long double", ffi_type_longdouble)));
#ifdef FFI_TARGET_HAS_COMPLEX_TYPE
	defined_types["complex float"] = Ref<CFFIType>(memnew(CFFIType("complex float", ffi_type_complex_float)));
	defined_types["complex double"] = Ref<CFFIType>(memnew(CFFIType("complex double", ffi_type_complex_double)));
	defined_types["complex longdouble"] = Ref<CFFIType>(memnew(CFFIType("complex longdouble", ffi_type_complex_longdouble)));
#endif

	// libffi aliases
	defined_types["char"] = Ref<CFFIType>(memnew(CFFIType("char", ffi_type_schar)));
	defined_types["unsigned char"] = Ref<CFFIType>(memnew(CFFIType("unsigned char", ffi_type_uchar)));
	defined_types["short"] = Ref<CFFIType>(memnew(CFFIType("short", ffi_type_sshort)));
	defined_types["unsigned short"] = Ref<CFFIType>(memnew(CFFIType("unsigned short", ffi_type_ushort)));
	defined_types["int"] = Ref<CFFIType>(memnew(CFFIType("int", ffi_type_sint)));
	defined_types["unsigned int"] = Ref<CFFIType>(memnew(CFFIType("unsigned int", ffi_type_uint)));
	defined_types["long"] = Ref<CFFIType>(memnew(CFFIType("long", ffi_type_slong)));
	defined_types["unsigned long"] = Ref<CFFIType>(memnew(CFFIType("unsigned long", ffi_type_ulong)));

	// stdint.h aliases
	defined_types["int8_t"] = defined_types["sint8"];
	defined_types["uint8_t"] = defined_types["uint8"];
	defined_types["int16_t"] = defined_types["sint16"];
	defined_types["uint16_t"] = defined_types["uint16"];
	defined_types["int32_t"] = defined_types["sint32"];
	defined_types["uint32_t"] = defined_types["uint32"];
	defined_types["int64_t"] = defined_types["sint64"];
	defined_types["uint64_t"] = defined_types["uint64"];
}

Ref<CFFILibraryHandle> CFFI::open(const String& name) const {
	return CFFILibraryHandle::open(name);
}

PackedByteArray CFFI::null_terminated_ascii_buffer(const String& str) {
	PackedByteArray buffer = str.to_ascii_buffer();
	buffer.append(0);
	return buffer;
}

PackedByteArray CFFI::null_terminated_utf8_buffer(const String& str) {
	PackedByteArray buffer = str.to_utf8_buffer();
	buffer.append(0);
	return buffer;
}

PackedByteArray CFFI::null_terminated_utf16_buffer(const String& str) {
	PackedByteArray buffer = str.to_utf16_buffer();
	buffer.append(0);
	buffer.append(0);
	return buffer;
}

PackedByteArray CFFI::null_terminated_utf32_buffer(const String& str) {
	PackedByteArray buffer = str.to_utf32_buffer();
	for (int i = 0; i < sizeof(char32_t); i++) {
		buffer.append(0);
	}
	return buffer;
}

PackedByteArray CFFI::null_terminated_wchar_buffer(const String& str) {
	PackedByteArray buffer = str.to_wchar_buffer();
	for (int i = 0; i < sizeof(wchar_t); i++) {
		buffer.append(0);
	}
	return buffer;
}

Ref<CFFIPointer> CFFI::memcpy(Ref<CFFIPointer> dest, Ref<CFFIPointer> src, int64_t size_bytes) {
	ERR_FAIL_COND_V(dest.is_null(), nullptr);
	ERR_FAIL_COND_V(src.is_null(), nullptr);
	::memcpy(dest->address_offset_by(0), src->address_offset_by(0), size_bytes);
	return dest;
}

Ref<CFFIPointer> CFFI::memmove(Ref<CFFIPointer> dest, Ref<CFFIPointer> src, int64_t size_bytes) {
	ERR_FAIL_COND_V(dest.is_null(), nullptr);
	ERR_FAIL_COND_V(src.is_null(), nullptr);
	::memmove(dest->address_offset_by(0), src->address_offset_by(0), size_bytes);
	return dest;
}

Ref<CFFIPointer> CFFI::memset(Ref<CFFIPointer> dest, int byte_value, int64_t size_bytes) {
	ERR_FAIL_COND_V(dest.is_null(), nullptr);
	::memset(dest->address_offset_by(0), byte_value, size_bytes);
	return dest;
}

int CFFI::memcmp(Ref<CFFIPointer> s1, Ref<CFFIPointer> s2, int64_t size_bytes) {
	if (s1.is_null() || s2.is_null()) {
		return s1.is_null() != s2.is_null();
	}
	return ::memcmp(s1->address_offset_by(0), s2->address_offset_by(0), size_bytes);
}

bool CFFI::memequal(Ref<CFFIPointer> s1, Ref<CFFIPointer> s2, int64_t size_bytes) {
	return memcmp(s1, s2, size_bytes) == 0;
}

void CFFI::_bind_methods() {
	ClassDB::bind_method(D_METHOD("open", "name_or_path"), &CFFI::open);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("null_terminated_ascii_buffer", "str"), &CFFI::null_terminated_ascii_buffer);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("null_terminated_utf8_buffer", "str"), &CFFI::null_terminated_utf8_buffer);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("null_terminated_utf16_buffer", "str"), &CFFI::null_terminated_utf16_buffer);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("null_terminated_utf32_buffer", "str"), &CFFI::null_terminated_utf32_buffer);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("null_terminated_wchar_buffer", "str"), &CFFI::null_terminated_wchar_buffer);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("memcpy", "dest", "src", "size_bytes"), &CFFI::memcpy);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("memmove", "dest", "src", "size_bytes"), &CFFI::memmove);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("memset", "dest", "byte_value", "size_bytes"), &CFFI::memset);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("memcmp", "s1", "s2", "size_bytes"), &CFFI::memcmp);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("memequal", "s1", "s2", "size_bytes"), &CFFI::memequal);
}

CFFI *CFFI::get_singleton() {
	return instance.ptr();
}

CFFI *CFFI::get_or_create_singleton() {
	if (instance.is_null()) {
		instance.instantiate();
		Engine::get_singleton()->register_singleton(CFFI::get_class_static(), instance.ptr());
	}
	return instance.ptr();
}

void CFFI::delete_singleton() {
	if (instance.is_valid()) {
		Engine::get_singleton()->unregister_singleton(CFFI::get_class_static());
		instance.unref();
	}
}

Ref<CFFI> CFFI::instance;

}
