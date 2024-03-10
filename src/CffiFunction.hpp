#ifndef __CFFI_FUNCTION_HPP__
#define __CFFI_FUNCTION_HPP__

#include <ffi.h>
#include <godot_cpp/classes/ref_counted.hpp>

#include "CffiType.hpp"
#include "godot_cpp/variant/typed_array.hpp"

using namespace godot;

namespace cffi {

class CffiFunction : public RefCounted {
    GDCLASS(CffiFunction, RefCounted);
public:
    CffiFunction();
    CffiFunction(const String& name, void *address, Ref<CffiType> return_type, TypedArray<CffiType> argument_types, bool is_variadic = false, ffi_abi abi = FFI_DEFAULT_ABI);
    ~CffiFunction();

protected:
    static void _bind_methods();

private:
    String name;
    void *address;
    ffi_cif ffi_handle;
    ffi_type **ffi_argument_types;
    Ref<CffiType> return_type;
    TypedArray<CffiType> argument_types;
    bool is_variadic;
};

}

#endif  // __CFFI_FUNCTION_HPP__