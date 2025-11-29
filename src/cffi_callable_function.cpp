#include "cffi_callable_function.hpp"
#include "cffi_type.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

namespace cffi {

CFFICallableFunction::CFFICallableFunction() : CFFIFunction() {}
CFFICallableFunction::CFFICallableFunction(const Callable& callable, const Ref<CFFIType>& return_type, const CFFITypeTuple& argument_types)
	: CFFIFunction(Variant(callable).stringify(), nullptr, return_type, argument_types)
	, callable(callable)
	, closure((ffi_closure *) ffi_closure_alloc(sizeof(ffi_closure), &address))
{
	ffi_status status = ffi_prep_closure_loc(closure, &ffi_handle, closure_handler, this, address);
	ERR_FAIL_COND(status != FFI_OK);
}

CFFICallableFunction::~CFFICallableFunction() {
	if (closure) {
		ffi_closure_free(closure);
	}
}

const Callable& CFFICallableFunction::get_callable() const {
	return callable;
}

void CFFICallableFunction::set_callable(const Callable& value) {
	callable = value;
}

void CFFICallableFunction::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_callable"), &CFFICallableFunction::get_callable);
	ClassDB::bind_method(D_METHOD("set_callable", "value"), &CFFICallableFunction::set_callable);

	ADD_PROPERTY(PropertyInfo(Variant::CALLABLE, "callable"), "set_callable", "get_callable");
}

void CFFICallableFunction::closure_handler(ffi_cif *cif, void *ret, void **args, void *userdata) {
	CFFICallableFunction *ffi_callable_function = (CFFICallableFunction *) userdata;
	ERR_FAIL_COND_MSG(!ffi_callable_function->callable.is_valid(), "Callable is not valid");

	Array gdargs;
	gdargs.resize(ffi_callable_function->argument_types.size());

	auto&& fields = ffi_callable_function->argument_types.get_fields();
	for (int i = 0; i < fields.size(); i++) {
		fields[i]->data_to_variant((uint8_t *) args[i], gdargs[i]);
	}
	Variant gdret = ffi_callable_function->callable.callv(gdargs);
	ffi_callable_function->return_type->variant_to_data(gdret, (uint8_t *) ret);
}

}
