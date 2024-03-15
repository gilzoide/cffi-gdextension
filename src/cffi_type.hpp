#ifndef __CFFI_TYPE_HPP__
#define __CFFI_TYPE_HPP__

#include <ffi.h>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/stream_peer_buffer.hpp>

using namespace godot;

namespace cffi {

class FFIType : public RefCounted {
	GDCLASS(FFIType, RefCounted);
public:
	FFIType();
	FFIType(const String& name, ffi_type ffi_type);

	ffi_type& get_ffi_type();
	const ffi_type& get_ffi_type() const;
	size_t get_size() const;
	unsigned short get_alignment() const;

	const String& get_name() const;

	bool get_return_value(const PackedByteArray& data, Variant& r_variant) const;
	virtual bool get_return_value(const uint8_t *ptr, Variant& r_variant) const;

	bool serialize_value_into(const Variant& value, PackedByteArray& buffer) const;
	virtual bool serialize_value_into(const Variant& value, uint8_t *buffer) const;

	static Ref<FFIType> from_variant(const Variant& var);

protected:
	static void _bind_methods();
	String _to_string() const;

	String name;
	ffi_type ffi_handle;
};

}

#endif  // __CFFI_TYPE_HPP__