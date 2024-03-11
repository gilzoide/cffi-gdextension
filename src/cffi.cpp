#include "cffi.hpp"

#include <godot_cpp/classes/engine.hpp>

namespace cffi {

FFI::FFI() {
    builtin_types["void"] = memnew(FFIType("void", ffi_type_void));
    builtin_types["uint8"] = memnew(FFIType("uint8", ffi_type_uint8));
    builtin_types["sint8"] = memnew(FFIType("sint8", ffi_type_sint8));
    builtin_types["uint16"] = memnew(FFIType("uint16", ffi_type_uint16));
    builtin_types["sint16"] = memnew(FFIType("sint16", ffi_type_sint16));
    builtin_types["uint32"] = memnew(FFIType("uint32", ffi_type_uint32));
    builtin_types["sint32"] = memnew(FFIType("sint32", ffi_type_sint32));
    builtin_types["uint64"] = memnew(FFIType("uint64", ffi_type_uint64));
    builtin_types["sint64"] = memnew(FFIType("sint64", ffi_type_void));
    builtin_types["float"] = memnew(FFIType("float", ffi_type_float));
    builtin_types["double"] = memnew(FFIType("double", ffi_type_double));
    builtin_types["pointer"] = memnew(FFIType("pointer", ffi_type_pointer));
    builtin_types["long double"] = memnew(FFIType("long double", ffi_type_longdouble));
#ifdef FFI_TARGET_HAS_COMPLEX_TYPE
    builtin_types["complex float"] = memnew(FFIType("complex float", ffi_type_complex_float));
    builtin_types["complex double"] = memnew(FFIType("complex double", ffi_type_complex_double));
    builtin_types["complex longdouble"] = memnew(FFIType("complex longdouble", ffi_type_complex_longdouble));
#endif

    // libffi aliases
    builtin_types["char"] = memnew(FFIType("char", ffi_type_schar));
    builtin_types["unsigned char"] = memnew(FFIType("unsigned char", ffi_type_uchar));
    builtin_types["short"] = memnew(FFIType("short", ffi_type_sshort));
    builtin_types["unsigned short"] = memnew(FFIType("unsigned short", ffi_type_ushort));
    builtin_types["int"] = memnew(FFIType("int", ffi_type_sint));
    builtin_types["unsigned int"] = memnew(FFIType("unsigned int", ffi_type_uint));
    builtin_types["long"] = memnew(FFIType("long", ffi_type_slong));
    builtin_types["unsigned long"] = memnew(FFIType("unsigned long", ffi_type_ulong));

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

FFIType *FFI::get_type(const String& name) const {
    return Object::cast_to<FFIType>(builtin_types.get(name, Variant()));
}

FFILibraryHandle *FFI::open(const String& name) const {
    return FFILibraryHandle::open(name);
}

void FFI::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_type", "name"), &FFI::get_type);
    ClassDB::bind_method(D_METHOD("open", "library_name"), &FFI::open);
}

bool FFI::_get(const StringName& property_name, Variant& r_value) const {
    if (auto type = get_type(property_name)) {
        r_value = type;
        return true;
    }
    else {
        return false;
    }
}

void FFI::_get_property_list(List<PropertyInfo> *p_list) const {
    StringName cffi_type_class_name = FFIType::get_class_static();
    Array keys = builtin_types.keys();
    for (int i = 0; i < keys.size(); i++) {
        p_list->push_back(PropertyInfo(Variant::OBJECT, keys[i], PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY, cffi_type_class_name));
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