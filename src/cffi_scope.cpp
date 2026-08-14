#include "cffi.hpp"
#include "cffi_callable_function.hpp"
#include "cffi_pointer_type.hpp"
#include "cffi_struct_type.hpp"
#include "cffi_scope.hpp"
#include "cffi_type_parser.hpp"

namespace cffi {

/// Utility to register a type alias
template<typename T, typename Map> void register_alias(Map& defined_types, const String& name) {
	if constexpr (std::is_floating_point_v<T>) {
		if constexpr (sizeof(T) == sizeof(float)) {
			defined_types[name] = defined_types["float"];
		}
		if constexpr (sizeof(T) == sizeof(double)) {
			defined_types[name] = defined_types["double"];
		}
	}
	else if constexpr (std::is_unsigned_v<T>) {
		if constexpr (sizeof(T) == sizeof(uint8_t)) {
			defined_types[name] = defined_types["uint8_t"];
		}
		if constexpr (sizeof(T) == sizeof(uint16_t)) {
			defined_types[name] = defined_types["uint16_t"];
		}
		if constexpr (sizeof(T) == sizeof(uint32_t)) {
			defined_types[name] = defined_types["uint32_t"];
		}
		if constexpr (sizeof(T) == sizeof(uint64_t)) {
			defined_types[name] = defined_types["uint64_t"];
		}
	}
	else {
		if constexpr (sizeof(T) == sizeof(int8_t)) {
			defined_types[name] = defined_types["int8_t"];
		}
		if constexpr (sizeof(T) == sizeof(int16_t)) {
			defined_types[name] = defined_types["int16_t"];
		}
		if constexpr (sizeof(T) == sizeof(int32_t)) {
			defined_types[name] = defined_types["int32_t"];
		}
		if constexpr (sizeof(T) == sizeof(int64_t)) {
			defined_types[name] = defined_types["int64_t"];
		}
	}
}

Ref<CFFIType> CFFIScope::find_type(const String& name) const {
	CFFITypeParser parser;
	ERR_FAIL_COND_V_EDMSG(!parser.parse(name), nullptr, String("Invalid type name \"%s\"") % name);

	// pointer types
	auto& base_name = parser.get_base_name();
	auto base_type = defined_types.getptr(base_name) ?: get_globally_defined_types().getptr(base_name);
	ERR_FAIL_COND_V_EDMSG(base_type == nullptr, nullptr, String("Unknown type name: \"%s\"") % name);
	auto type = *base_type;
	for (int i = 0; i < parser.get_pointer_level(); i++) {
		type = Ref<CFFIType>(memnew(CFFIPointerType(type)));
	}
	return type;
}

Ref<CFFIStructType> CFFIScope::define_struct(const String& name, const Dictionary& fields) {
	CFFITypeParser parser;
	ERR_FAIL_COND_V_EDMSG(
		!parser.parse(name) || parser.get_pointer_level() != 0,
		nullptr,
		String("Invalid type name: \"%s\"") % name
	);

	auto& base_name = parser.get_base_name();
	ERR_FAIL_COND_V_EDMSG(
		defined_types.has(base_name) || get_globally_defined_types().has(base_name),
		nullptr,
		String("Type name already defined: \"%s\"") % name
	);

	auto type = CFFIStructType::from_dictionary(base_name, fields, this);
	if (type.is_valid()) {
		defined_types[base_name] = type;
	}
	return type;
}

Ref<CFFICallableFunction> CFFIScope::create_function(const Callable& callable, const Variant& return_type_var, const Array& argument_types_arr) {
	ERR_FAIL_COND_V_MSG(!callable.is_valid(), nullptr, "Callable is invalid");

	Ref<CFFIType> return_type = CFFIType::from_variant(return_type_var, this);
	ERR_FAIL_COND_V_MSG(return_type == nullptr, nullptr, String("Could not find return type: %s") % return_type_var.stringify());

	CFFITypeTuple argument_types = CFFITypeTuple::from_array(argument_types_arr, this);
	ERR_FAIL_COND_V_MSG(argument_types.size() != argument_types_arr.size(), nullptr, "Invalid argument types");

	return memnew(CFFICallableFunction(callable, return_type, argument_types));
}

bool CFFIScope::_get(const StringName& property_name, Variant& r_value) const {
	auto type = find_type(property_name);
	if (type.is_valid()) {
		r_value = type;
		return true;
	}
	else {
		return false;
	}
}

void CFFIScope::_bind_methods() {
	ClassDB::bind_method(D_METHOD("find_type", "name"), &CFFIScope::find_type);
	ClassDB::bind_method(D_METHOD("define_struct", "name", "fields"), &CFFIScope::define_struct);
	ClassDB::bind_method(D_METHOD("create_function", "callable", "return_type", "argument_types"), &CFFIScope::create_function, DEFVAL(Array()));
}

void CFFIScope::setup_builtin_types() {
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

	register_alias<size_t>(defined_types, "size_t");
	register_alias<ssize_t>(defined_types, "ssize_t");
	register_alias<intptr_t>(defined_types, "intptr_t");
	register_alias<uintptr_t>(defined_types, "uintptr_t");

	register_alias<char16_t>(defined_types, "char16_t");
	register_alias<char32_t>(defined_types, "char32_t");

	register_alias<real_t>(defined_types, "real_t");
}

const HashMap<String, Ref<CFFIType>>& CFFIScope::get_globally_defined_types() {
	return CFFI::get_singleton()->get_scope()->defined_types;
}

}
