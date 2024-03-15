#include "cffi.hpp"
#include "cffi_library_handle.hpp"
#include "cffi_pointer_type.hpp"
#include "cffi_type_parser.hpp"

#include <godot_cpp/classes/engine.hpp>

namespace cffi {

FFI::FFI() {
	builtin_types["void"] = Ref<FFIType>(memnew(FFIType("void", ffi_type_void)));
	builtin_types["uint8"] = Ref<FFIType>(memnew(FFIType("uint8", ffi_type_uint8)));
	builtin_types["sint8"] = Ref<FFIType>(memnew(FFIType("sint8", ffi_type_sint8)));
	builtin_types["uint16"] = Ref<FFIType>(memnew(FFIType("uint16", ffi_type_uint16)));
	builtin_types["sint16"] = Ref<FFIType>(memnew(FFIType("sint16", ffi_type_sint16)));
	builtin_types["uint32"] = Ref<FFIType>(memnew(FFIType("uint32", ffi_type_uint32)));
	builtin_types["sint32"] = Ref<FFIType>(memnew(FFIType("sint32", ffi_type_sint32)));
	builtin_types["uint64"] = Ref<FFIType>(memnew(FFIType("uint64", ffi_type_uint64)));
	builtin_types["sint64"] = Ref<FFIType>(memnew(FFIType("sint64", ffi_type_void)));
	builtin_types["float"] = Ref<FFIType>(memnew(FFIType("float", ffi_type_float)));
	builtin_types["double"] = Ref<FFIType>(memnew(FFIType("double", ffi_type_double)));
	builtin_types["pointer"] = Ref<FFIType>(memnew(FFIType("pointer", ffi_type_pointer)));
	builtin_types["long double"] = Ref<FFIType>(memnew(FFIType("long double", ffi_type_longdouble)));
#ifdef FFI_TARGET_HAS_COMPLEX_TYPE
	builtin_types["complex float"] = Ref<FFIType>(memnew(FFIType("complex float", ffi_type_complex_float)));
	builtin_types["complex double"] = Ref<FFIType>(memnew(FFIType("complex double", ffi_type_complex_double)));
	builtin_types["complex longdouble"] = Ref<FFIType>(memnew(FFIType("complex longdouble", ffi_type_complex_longdouble)));
#endif

	// libffi aliases
	builtin_types["char"] = Ref<FFIType>(memnew(FFIType("char", ffi_type_schar)));
	builtin_types["unsigned char"] = Ref<FFIType>(memnew(FFIType("unsigned char", ffi_type_uchar)));
	builtin_types["short"] = Ref<FFIType>(memnew(FFIType("short", ffi_type_sshort)));
	builtin_types["unsigned short"] = Ref<FFIType>(memnew(FFIType("unsigned short", ffi_type_ushort)));
	builtin_types["int"] = Ref<FFIType>(memnew(FFIType("int", ffi_type_sint)));
	builtin_types["unsigned int"] = Ref<FFIType>(memnew(FFIType("unsigned int", ffi_type_uint)));
	builtin_types["long"] = Ref<FFIType>(memnew(FFIType("long", ffi_type_slong)));
	builtin_types["unsigned long"] = Ref<FFIType>(memnew(FFIType("unsigned long", ffi_type_ulong)));

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

Ref<FFIType> FFI::get_type(const String& name) const {
	FFITypeParser parser;
	ERR_FAIL_COND_V_EDMSG(!parser.parse(name), nullptr, String("Invalid type \"%s\"") % name);

	// pointer types
	auto base_type = builtin_types.getptr(parser.get_base_name());
	ERR_FAIL_COND_V_EDMSG(base_type == nullptr, nullptr, String("Invalid type \"%s\"") % name);
	auto type = *base_type;
	for (int i = 0; i < parser.get_pointer_level(); i++) {
		type = Ref<FFIType>(memnew(FFIPointerType(type)));
	}
	return type;
}

Ref<FFILibraryHandle> FFI::open(const String& name) const {
	return FFILibraryHandle::open(name);
}

void FFI::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_type", "name"), &FFI::get_type);
	ClassDB::bind_method(D_METHOD("open", "library_name"), &FFI::open);
}

bool FFI::_get(const StringName& property_name, Variant& r_value) const {
	auto type = get_type(property_name);
	if (type.is_valid()) {
		r_value = type;
		return true;
	}
	else {
		return false;
	}
}

FFI *FFI::get_singleton() {
	return instance;
}

FFI *FFI::get_or_create_singleton() {
	if (!instance) {
		instance = memnew(FFI);
		Engine::get_singleton()->register_singleton(FFI::get_class_static(), instance);
	}
	return instance;
}

void FFI::delete_singleton() {
	if (instance) {
		Engine::get_singleton()->unregister_singleton(FFI::get_class_static());
		memdelete(instance);
		instance = nullptr;
	}
}

FFI *FFI::instance;

}
