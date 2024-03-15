#ifndef __CFFI_POINTER_TYPE_HPP__
#define __CFFI_POINTER_TYPE_HPP__

#include "cffi_type.hpp"

using namespace godot;

namespace cffi {

class FFIPointerType : public FFIType {
	GDCLASS(FFIPointerType, FFIType);
public:
	FFIPointerType();
	FFIPointerType(Ref<FFIType> element_type);

	Ref<FFIType> get_element_type() const;

	bool get_return_value(const uint8_t *ptr, Variant& r_variant) const override;
	bool serialize_value_into(const Variant& value, uint8_t *buffer) const override;

protected:
	Ref<FFIType> element_type;
};

}

#endif  // __CFFI_POINTER_TYPE_HPP__