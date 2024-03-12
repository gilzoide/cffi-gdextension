#include "cffi_function.hpp"
#include "cffi_value_tuple.hpp"

namespace cffi {

FFIFunction::FFIFunction() {}
FFIFunction::FFIFunction(const String& name, void *address, const Ref<FFIType>& return_type, const FFITypeTuple& argument_types, bool is_variadic, ffi_abi abi)
	: name(name), address(address), return_type(return_type), argument_types(argument_types), is_variadic(is_variadic)
{
	ffi_prep_cif(&ffi_handle, abi, argument_types.size(), &return_type->get_ffi_type(), this->argument_types.get_argument_types());
}

Variant FFIFunction::invokev(const Array& arguments) {
	PackedByteArray return_data;
	return_data.resize(MAX(return_type->get_ffi_type().size, sizeof(ffi_arg)));
	FFIValueTuple argument_data = FFIValueTuple::from_array(argument_types, arguments);
	ERR_FAIL_COND_V(argument_data.size() != arguments.size(), Variant());
	ffi_call(&ffi_handle, (void(*)()) address, (void *) return_data.ptr(), argument_data.get_value_addresses());
	Variant return_value;
	bool return_type_valid = return_type->get_return_value(return_data, return_value);
	ERR_FAIL_COND_V_MSG(!return_type_valid, Variant(), "Return type is not supported");
	return return_value;
}

void FFIFunction::_bind_methods() {
	ClassDB::bind_method(D_METHOD("invokev", "arguments"), &FFIFunction::invokev, DEFVAL(Array()));
}

String FFIFunction::_to_string() const {
	return String("[%s:0x%x:%s %s%s]") % Array::make(get_class(), (uint64_t) address, return_type->get_name(), name, argument_types.to_string());
}

}