#include "cffi_function.hpp"
#include "cffi_type.hpp"

namespace cffi {

FFIFunction::FFIFunction() {}
FFIFunction::FFIFunction(const String& name, void *address, Ref<FFIType> return_type, TypedArray<FFIType> argument_types, bool is_variadic, ffi_abi abi)
    : name(name), address(address), return_type(return_type), argument_types(argument_types), is_variadic(is_variadic)
{
    ffi_argument_types = memnew_arr(ffi_type*, argument_types.size());
    for (int i = 0; i < argument_types.size(); i++) {
        ffi_argument_types[i] = &Object::cast_to<FFIType>(argument_types[i])->get_ffi_type();
    }
    ffi_prep_cif(&ffi_handle, abi, argument_types.size(), &return_type->get_ffi_type(), ffi_argument_types);
}

FFIFunction::~FFIFunction() {
    if (ffi_argument_types) {
        memdelete_arr(ffi_argument_types);
    }
}

void FFIFunction::_bind_methods() {}

String FFIFunction::_to_string() const {
    PackedStringArray arguments_str;
    for (int i = 0; i < argument_types.size(); i++) {
        arguments_str.append(Object::cast_to<FFIType>(argument_types[0])->get_name());
    }
    return String("[%s:0x%x:%s %s(%s)]") % Array::make(get_class(), (uint64_t) address, return_type->get_name(), name, String(", ").join(arguments_str));
}

}