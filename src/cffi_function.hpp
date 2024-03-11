#ifndef __CFFI_FUNCTION_HPP__
#define __CFFI_FUNCTION_HPP__

#include <ffi.h>
#include <godot_cpp/classes/ref_counted.hpp>

#include "cffi_type.hpp"
#include "godot_cpp/variant/typed_array.hpp"

using namespace godot;

namespace cffi {

class FFIFunction : public RefCounted {
	GDCLASS(FFIFunction, RefCounted);
public:
	FFIFunction();
	FFIFunction(const String& name, void *address, Ref<FFIType> return_type, TypedArray<FFIType> argument_types, bool is_variadic = false, ffi_abi abi = FFI_DEFAULT_ABI);
	~FFIFunction();

protected:
	static void _bind_methods();
	String _to_string() const;

private:
	String name;
	void *address;
	ffi_cif ffi_handle;
	ffi_type **ffi_argument_types;
	Ref<FFIType> return_type;
	TypedArray<FFIType> argument_types;
	bool is_variadic;
};

}

#endif  // __CFFI_FUNCTION_HPP__
