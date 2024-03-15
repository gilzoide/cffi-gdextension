#ifndef __CFFI_POINTER_HPP__
#define __CFFI_POINTER_HPP__

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

namespace cffi {

class FFIType;

class FFIPointer : public RefCounted {
	GDCLASS(FFIPointer, RefCounted);
public:
	FFIPointer();
	FFIPointer(Ref<FFIType> element_type, uint8_t *address);

	uint8_t *address_offset_by(int offset) const;
	Ref<FFIPointer> offset_by(int offset) const;
	Variant get_value(int index = 0) const;
	bool set_value(const Variant& value, int index = 0) const;

protected:
	static void _bind_methods();
	String _to_string() const;

	Ref<FFIType> element_type;
	uint8_t *address;
};

}

#endif