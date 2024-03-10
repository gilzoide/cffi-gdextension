#include "CffiFunction.hpp"

#include <godot_cpp/core/memory.hpp>

namespace cffi {

CffiFunction::CffiFunction() {}
CffiFunction::CffiFunction(const String& name, void *address, Ref<CffiType> return_type, TypedArray<CffiType> argument_types, bool is_variadic, ffi_abi abi)
    : name(name), address(address), return_type(return_type), argument_types(argument_types), is_variadic(is_variadic)
{
    ffi_argument_types = memnew_arr(ffi_type*, argument_types.size());
    for (int i = 0; i < argument_types.size(); i++) {
        ffi_argument_types[i] = &Object::cast_to<CffiType>(argument_types[i])->get_ffi_type();
    }
    ffi_prep_cif(&ffi_handle, abi, argument_types.size(), &return_type->get_ffi_type(), ffi_argument_types);
}

CffiFunction::~CffiFunction() {
    memdelete_arr(ffi_argument_types);
}

void CffiFunction::_bind_methods() {}

}