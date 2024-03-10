#ifndef __CFFI_LIBRARY_HPP__
#define __CFFI_LIBRARY_HPP__

#include "cffi_function.hpp"
#include "cffi_type.hpp"

#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

namespace cffi {

class FFILibrary : public RefCounted {
    GDCLASS(FFILibrary, RefCounted);
public:
    FFILibrary();
    FFILibrary(void *library_handle);
    ~FFILibrary();

    static FFILibrary *open(const String& name);

    Ref<FFIFunction> define_function(const String& name, Ref<FFIType> return_type, TypedArray<FFIType> argument_types, bool is_variadic = false);

protected:
    static void _bind_methods();
    String _to_string() const;

private:
    void *library_handle;
};

}

#endif  // __CFFI_LIBRARY_HPP__