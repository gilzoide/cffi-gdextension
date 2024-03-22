#include "cffi_pointer_type.hpp"
#include "cffi_struct_type.hpp"
#include "cffi_scope.hpp"
#include "cffi_type.hpp"
#include "cffi_type_parser.hpp"

namespace cffi {

Ref<CFFIType> CFFIScope::get_type(const String& name) const {
	CFFITypeParser parser;
	ERR_FAIL_COND_V_EDMSG(!parser.parse(name), nullptr, String("Invalid type \"%s\"") % name);

	// pointer types
	auto base_type = builtin_types.getptr(parser.get_base_name());
	ERR_FAIL_COND_V_EDMSG(base_type == nullptr, nullptr, String("Invalid type \"%s\"") % name);
	auto type = *base_type;
	for (int i = 0; i < parser.get_pointer_level(); i++) {
		type = Ref<CFFIType>(memnew(CFFIPointerType(type)));
	}
	return type;
}

Ref<CFFIStructType> CFFIScope::register_struct(const String& name, const Dictionary& fields) {
	return CFFIStructType::from_dictionary(name, fields);
}

bool CFFIScope::_get(const StringName& property_name, Variant& r_value) const {
	auto type = get_type(property_name);
	if (type.is_valid()) {
		r_value = type;
		return true;
	}
	else {
		return false;
	}
}

void CFFIScope::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_type", "name"), &CFFIScope::get_type);
	ClassDB::bind_method(D_METHOD("register_struct", "name", "fields"), &CFFIScope::register_struct);
}

void CFFIScope::register_builtin_types() {
	builtin_types["void"] = Ref<CFFIType>(memnew(CFFIType("void", ffi_type_void)));
	builtin_types["uint8"] = Ref<CFFIType>(memnew(CFFIType("uint8", ffi_type_uint8)));
	builtin_types["sint8"] = Ref<CFFIType>(memnew(CFFIType("sint8", ffi_type_sint8)));
	builtin_types["uint16"] = Ref<CFFIType>(memnew(CFFIType("uint16", ffi_type_uint16)));
	builtin_types["sint16"] = Ref<CFFIType>(memnew(CFFIType("sint16", ffi_type_sint16)));
	builtin_types["uint32"] = Ref<CFFIType>(memnew(CFFIType("uint32", ffi_type_uint32)));
	builtin_types["sint32"] = Ref<CFFIType>(memnew(CFFIType("sint32", ffi_type_sint32)));
	builtin_types["uint64"] = Ref<CFFIType>(memnew(CFFIType("uint64", ffi_type_uint64)));
	builtin_types["sint64"] = Ref<CFFIType>(memnew(CFFIType("sint64", ffi_type_void)));
	builtin_types["float"] = Ref<CFFIType>(memnew(CFFIType("float", ffi_type_float)));
	builtin_types["double"] = Ref<CFFIType>(memnew(CFFIType("double", ffi_type_double)));
	builtin_types["pointer"] = Ref<CFFIType>(memnew(CFFIType("pointer", ffi_type_pointer)));
	builtin_types["long double"] = Ref<CFFIType>(memnew(CFFIType("long double", ffi_type_longdouble)));
#ifdef FFI_TARGET_HAS_COMPLEX_TYPE
	builtin_types["complex float"] = Ref<CFFIType>(memnew(CFFIType("complex float", ffi_type_complex_float)));
	builtin_types["complex double"] = Ref<CFFIType>(memnew(CFFIType("complex double", ffi_type_complex_double)));
	builtin_types["complex longdouble"] = Ref<CFFIType>(memnew(CFFIType("complex longdouble", ffi_type_complex_longdouble)));
#endif

	// libffi aliases
	builtin_types["char"] = Ref<CFFIType>(memnew(CFFIType("char", ffi_type_schar)));
	builtin_types["unsigned char"] = Ref<CFFIType>(memnew(CFFIType("unsigned char", ffi_type_uchar)));
	builtin_types["short"] = Ref<CFFIType>(memnew(CFFIType("short", ffi_type_sshort)));
	builtin_types["unsigned short"] = Ref<CFFIType>(memnew(CFFIType("unsigned short", ffi_type_ushort)));
	builtin_types["int"] = Ref<CFFIType>(memnew(CFFIType("int", ffi_type_sint)));
	builtin_types["unsigned int"] = Ref<CFFIType>(memnew(CFFIType("unsigned int", ffi_type_uint)));
	builtin_types["long"] = Ref<CFFIType>(memnew(CFFIType("long", ffi_type_slong)));
	builtin_types["unsigned long"] = Ref<CFFIType>(memnew(CFFIType("unsigned long", ffi_type_ulong)));

	// stdint.h aliases
	builtin_types["int8_t"] = builtin_types["sint8"];
	builtin_types["uint8_t"] = builtin_types["uint8"];
	builtin_types["int16_t"] = builtin_types["sint16"];
	builtin_types["uint16_t"] = builtin_types["uint16"];
	builtin_types["int32_t"] = builtin_types["sint32"];
	builtin_types["uint32_t"] = builtin_types["uint32"];
	builtin_types["int64_t"] = builtin_types["sint64"];
	builtin_types["uint64_t"] = builtin_types["uint64"];
}

void CFFIScope::clear_builtin_types() {
	builtin_types.clear();
}

HashMap<String, Ref<CFFIType>> CFFIScope::builtin_types;

}
