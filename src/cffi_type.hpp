#ifndef __CFFI_TYPE_HPP__
#define __CFFI_TYPE_HPP__

#include <ffi.h>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/wrapped.hpp>

using namespace godot;

namespace cffi {

class FFIType : public RefCounted {
	GDCLASS(FFIType, RefCounted);
public:
	FFIType();
	FFIType(const String& name, ffi_type ffi_type);

	// TODO: get_size, get_alignment
	ffi_type& get_ffi_type();
	const ffi_type& get_ffi_type() const;

	const String& get_name() const;

	static FFIType *from_variant(const Variant& var);

protected:
	static void _bind_methods();
	String _to_string() const;

	String name;
	ffi_type ffi_type;
};

}

#endif  // __CFFI_TYPE_HPP__