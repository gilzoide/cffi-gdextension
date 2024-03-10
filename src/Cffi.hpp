#ifndef __CFFI_HPP__
#define __CFFI_HPP__

#include <ffi.h>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/dictionary.hpp>

#include "CffiType.hpp"
#include "CffiLibrary.hpp"

using namespace godot;

namespace cffi {

class Cffi : public RefCounted {
    GDCLASS(Cffi, RefCounted);
public:
    Cffi();

    CffiType *get_type(const String& name) const;

    CffiLibrary *open(const String& name) const;

    static Cffi *get_singleton();
	static Cffi *get_or_create_singleton();
	static void delete_singleton();

protected:
    static void _bind_methods();
    
    bool _get(const StringName& property_name, Variant& r_value) const;
    void _get_property_list(List<PropertyInfo> *p_list) const;

    Dictionary builtin_types;

private:
    static Cffi *instance;
};

}

#endif  // __CFFI_HPP__