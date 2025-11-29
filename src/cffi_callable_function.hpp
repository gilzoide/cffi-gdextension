#ifndef __CFFI_CALLABLE_FUNCTION_HPP__
#define __CFFI_CALLABLE_FUNCTION_HPP__

#include "cffi_function.hpp"

namespace cffi {

/**
 * A native function that invokes a user provided Callable.
 *
 * @warning You are responsible for keeping this instance alive while native code has access to it.
 * Attepting to invoke a released instance from native code is undefined behavior.
 */
class CFFICallableFunction : public CFFIFunction {
	GDCLASS(CFFICallableFunction, CFFIFunction);
public:
	/**
	 * Necessary to define a Godot class.
	 * @warning Never use this constructor.
	 */
	CFFICallableFunction();
	/**
	 * @param name  Function name. Used only for printing.
	 * @param address  Native function address, got from `dlsym` or `GetProcAddress`.
	 * @param return_type  Return type. Must not be null.
	 * @param argument_types  Argument types as a tuple.
	 * @param is_variadic  Flag for variadic functions. For now, it's ignored.
	 * @param abi  FFI ABI. When in doubt, use the default value `FFI_DEFAULT_ABI`.
	 */
	CFFICallableFunction(const Callable& callable, const Ref<CFFIType>& return_type, const CFFITypeTuple& argument_types);

	~CFFICallableFunction() override;

	/**
	 * Get internal Callable.
	 */
	const Callable& get_callable() const;
	/**
	 * Set internal Callable
	 */
	void set_callable(const Callable& value);

protected:
	static void _bind_methods();

private:
	Callable callable;
	ffi_closure *closure;

	static void closure_handler(ffi_cif *cif, void *ret, void **args, void *userdata);
};

}

#endif  // __CFFI_CALLABLE_FUNCTION_HPP__