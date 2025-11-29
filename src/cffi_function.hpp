#ifndef __CFFI_FUNCTION_HPP__
#define __CFFI_FUNCTION_HPP__

#include <ffi.h>
#include <godot_cpp/classes/ref_counted.hpp>

#include "cffi_type_tuple.hpp"

using namespace godot;

namespace cffi {

class CFFITypeTuple;
class CFFIValueTuple;

/**
 * Godot object that represents a native function.
 *
 * CFFIFunctions can be invoked from GDScript, stored in struct fields or passed as arguments.
 */
class CFFIFunction : public RefCounted {
	GDCLASS(CFFIFunction, RefCounted);
public:
	/**
	 * Necessary to define a Godot class.
	 * @warning Never use this constructor.
	 */
	CFFIFunction();
	/**
	 * @param name  Function name. Used only for printing.
	 * @param address  Native function address, got from `dlsym` or `GetProcAddress`.
	 * @param return_type  Return type. Must not be null.
	 * @param argument_types  Argument types as a tuple.
	 * @param is_variadic  Flag for variadic functions. For now, it's ignored.
	 * @param abi  FFI ABI. When in doubt, use the default value `FFI_DEFAULT_ABI`.
	 */
	CFFIFunction(const String& name, void *address, const Ref<CFFIType>& return_type, const CFFITypeTuple& argument_types, bool is_variadic = false, ffi_abi abi = FFI_DEFAULT_ABI);

	/**
	 * Invoke the function passing `arguments`.
	 *
	 * The number of arguments must match the function's prototype.
	 * If any of the argument values don't match the type defined by the prototype, the call errors and returns `null`.
	 */
	Variant invokev(const Array& arguments);
	/**
	 * Invoke the function passing arguments from a variadic call.
	 *
	 * The number of arguments must match the function's prototype.
	 * If any of the argument values don't match the type defined by the prototype, the call errors and returns `null`.
	 */
	Variant invoke_variadic(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);

	/**
	 * Get the function's code address.
	 */
	void *get_code_address() const;

protected:
	static void _bind_methods();
	String _to_string() const;

	Variant invoke(const CFFIValueTuple& arguments);

	String name;
	void *address;
	ffi_cif ffi_handle;
	ffi_type **ffi_argument_types;
	Ref<CFFIType> return_type;
	CFFITypeTuple argument_types;
	bool is_variadic;
};

}

#endif  // __CFFI_FUNCTION_HPP__
