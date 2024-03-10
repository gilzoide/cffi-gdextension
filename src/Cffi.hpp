#ifndef __CFFI_HPP__
#define __CFFI_HPP__

#include <ffi.h>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/dictionary.hpp>

#include "CffiType.hpp"

using namespace godot;

namespace cffi {

class Cffi : public RefCounted {
    GDCLASS(Cffi, RefCounted);
public:
    Cffi();

    CffiType *get_type(const String& name) const;

    static Cffi *get_singleton();
	static Cffi *get_or_create_singleton();
	static void delete_singleton();

protected:
    static void _bind_methods();

    Dictionary builtin_types;

private:
    static Cffi *instance;
};

}

#endif  // __CFFI_HPP__