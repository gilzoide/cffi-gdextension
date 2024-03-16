#ifndef __CFFI_POINTER_TYPE_HPP__
#define __CFFI_POINTER_TYPE_HPP__

#include "cffi_type.hpp"

using namespace godot;

namespace cffi {

class CFFIPointerType : public CFFIType {
	GDCLASS(CFFIPointerType, CFFIType);
public:
	CFFIPointerType();
	CFFIPointerType(Ref<CFFIType> element_type);

	Ref<CFFIType> get_element_type() const;

	bool get_return_value(const uint8_t *ptr, Variant& r_variant) const override;
	bool serialize_value_into(const Variant& value, uint8_t *buffer) const override;

protected:
	Ref<CFFIType> element_type;
};

}

#endif  // __CFFI_POINTER_TYPE_HPP__