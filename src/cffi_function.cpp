#include "cffi_function.hpp"

namespace cffi {

FFIFunction::FFIFunction() {}
FFIFunction::FFIFunction(const String& name, void *address, const Ref<FFIType>& return_type, const TypedArray<FFIType>& argument_types, bool is_variadic, ffi_abi abi)
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

Variant FFIFunction::invokev(const Array& arguments) {
	PackedByteArray argument_data;
	PackedByteArray return_data;
	return_data.resize(MAX(return_type->get_ffi_type().size, sizeof(ffi_arg)));
	ffi_call(&ffi_handle, (void(*)()) address, (void *) return_data.ptr(), (void **) argument_data.ptr());
	Variant return_value;
	bool return_type_valid = return_type->get_return_value(return_data, return_value);
	ERR_FAIL_COND_V_MSG(!return_type_valid, Variant(), "Return type is not supported");
	return return_value;
}

void FFIFunction::_bind_methods() {
	ClassDB::bind_method(D_METHOD("invokev", "arguments"), &FFIFunction::invokev, DEFVAL(Array()));
}

String FFIFunction::_to_string() const {
	PackedStringArray arguments_str;
	for (int i = 0; i < argument_types.size(); i++) {
		arguments_str.append(Object::cast_to<FFIType>(argument_types[0])->get_name());
	}
	return String("[%s:0x%x:%s %s(%s)]") % Array::make(get_class(), (uint64_t) address, return_type->get_name(), name, String(", ").join(arguments_str));
}

}