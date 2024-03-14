#ifndef __CFFI_FUNCTION_HPP__
#define __CFFI_FUNCTION_HPP__

#include <ffi.h>
#include <godot_cpp/classes/ref_counted.hpp>

#include "cffi_type_tuple.hpp"

using namespace godot;

namespace cffi {

class FFITypeTuple;
class FFIValueTuple;

class FFIFunction : public RefCounted {
	GDCLASS(FFIFunction, RefCounted);
public:
	FFIFunction();
	FFIFunction(const String& name, void *address, const Ref<FFIType>& return_type, const FFITypeTuple& argument_types, bool is_variadic = false, ffi_abi abi = FFI_DEFAULT_ABI);

	Variant invokev(const Array& arguments);
	Variant invoke_variadic(const Variant **args, GDExtensionInt arg_count, GDExtensionCallError &error);

protected:
	static void _bind_methods();
	String _to_string() const;

	Variant invoke(const FFIValueTuple& arguments);

private:
	String name;
	void *address;
	ffi_cif ffi_handle;
	ffi_type **ffi_argument_types;
	Ref<FFIType> return_type;
	FFITypeTuple argument_types;
	bool is_variadic;
};

}

#endif  // __CFFI_FUNCTION_HPP__
