#ifndef __CFFI_TYPE_HPP__
#define __CFFI_TYPE_HPP__

#include <ffi.h>
#include <godot_cpp/classes/ref_counted.hpp>

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

	bool get_return_value(const PackedByteArray& data, Variant& r_variant) const;

	static FFIType *from_variant(const Variant& var);

protected:
	static void _bind_methods();
	String _to_string() const;

	String name;
	ffi_type ffi_handle;
};

}

#endif  // __CFFI_TYPE_HPP__