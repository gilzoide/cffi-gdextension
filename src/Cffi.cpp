#include "Cffi.hpp"

#include <godot_cpp/classes/engine.hpp>

namespace cffi {

Cffi::Cffi() {
    builtin_types["void"] = memnew(CffiType("void", ffi_type_void));
    builtin_types["uint8"] = memnew(CffiType("uint8", ffi_type_uint8));
    builtin_types["sint8"] = memnew(CffiType("sint8", ffi_type_sint8));
    builtin_types["uint16"] = memnew(CffiType("uint16", ffi_type_uint16));
    builtin_types["sint16"] = memnew(CffiType("sint16", ffi_type_sint16));
    builtin_types["uint32"] = memnew(CffiType("uint32", ffi_type_uint32));
    builtin_types["sint32"] = memnew(CffiType("sint32", ffi_type_sint32));
    builtin_types["uint64"] = memnew(CffiType("uint64", ffi_type_uint64));
    builtin_types["sint64"] = memnew(CffiType("sint64", ffi_type_void));
    builtin_types["float"] = memnew(CffiType("float", ffi_type_float));
    builtin_types["double"] = memnew(CffiType("double", ffi_type_double));
    builtin_types["pointer"] = memnew(CffiType("pointer", ffi_type_pointer));
    builtin_types["long double"] = memnew(CffiType("long double", ffi_type_longdouble));
#ifdef FFI_TARGET_HAS_COMPLEX_TYPE
    builtin_types["complex float"] = memnew(CffiType("complex float", ffi_type_complex_float));
    builtin_types["complex double"] = memnew(CffiType("complex double", ffi_type_complex_double));
    builtin_types["complex longdouble"] = memnew(CffiType("complex longdouble", ffi_type_complex_longdouble));
#endif

    // libffi aliases
    builtin_types["char"] = memnew(CffiType("char", ffi_type_schar));
    builtin_types["unsigned char"] = memnew(CffiType("unsigned char", ffi_type_uchar));
    builtin_types["short"] = memnew(CffiType("short", ffi_type_sshort));
    builtin_types["unsigned short"] = memnew(CffiType("unsigned short", ffi_type_ushort));
    builtin_types["int"] = memnew(CffiType("int", ffi_type_sint));
    builtin_types["unsigned int"] = memnew(CffiType("unsigned int", ffi_type_uint));
    builtin_types["long"] = memnew(CffiType("long", ffi_type_slong));
    builtin_types["unsigned long"] = memnew(CffiType("unsigned long", ffi_type_ulong));

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

CffiType *Cffi::get_type(const String& name) const {
    return Object::cast_to<CffiType>(builtin_types.get(name, Variant()));
}

CffiLibrary *Cffi::open(const String& name) const {
    return CffiLibrary::open(name);
}

void Cffi::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_type", "name"), &Cffi::get_type);
    ClassDB::bind_method(D_METHOD("open", "library_name"), &Cffi::open);
}

bool Cffi::_get(const StringName& property_name, Variant& r_value) const {
    if (auto type = get_type(property_name)) {
        r_value = type;
        return true;
    }
    else {
        return false;
    }
}

void Cffi::_get_property_list(List<PropertyInfo> *p_list) const {
    StringName cffi_type_class_name = CffiType::get_class_static();
    Array keys = builtin_types.keys();
    for (int i = 0; i < keys.size(); i++) {
        p_list->push_back(PropertyInfo(Variant::OBJECT, keys[i], PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY, cffi_type_class_name));
    }
}

Cffi *Cffi::get_singleton() {
	return instance;
}

Cffi *Cffi::get_or_create_singleton() {
	if (!instance) {
		instance = memnew(Cffi);
		Engine::get_singleton()->register_singleton("Cffi", instance);
	}
	return instance;
}

void Cffi::delete_singleton() {
	if (instance) {
		Engine::get_singleton()->unregister_singleton("Cffi");
		memdelete(instance);
		instance = nullptr;
	}
}

Cffi *Cffi::instance;

}