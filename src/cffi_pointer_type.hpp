#ifndef __CFFI_POINTER_TYPE_HPP__
#define __CFFI_POINTER_TYPE_HPP__

#include "cffi_type.hpp"

using namespace godot;

namespace cffi {

/**
 * `CFFIType` specialized for pointer types.
 */
class CFFIPointerType : public CFFIType {
	GDCLASS(CFFIPointerType, CFFIType);
public:
	/**
	 * Necessary to define a Godot class.
	 * @warning Never use this constructor.
	 */
	CFFIPointerType();
	/**
	 * Create a pointer type that points to elements with type `element_type`.
	 */
	CFFIPointerType(Ref<CFFIType> element_type);

	/**
	 * Get the element type.
	 */
	Ref<CFFIType> get_element_type() const;

	/**
	 * Write a `CFFIPointer` to `r_variant`.
	 *
	 * @return True.
	 */
	bool data_to_variant(const uint8_t *ptr, Variant& r_variant) const override;
	/**
	 * Write the Variant `value` into `buffer`.
	 *
	 * Only PackedByteArray and CFFIPointers are supported.
	 *
	 * @return True if the conversion succeeded, false otherwise.
	 */
	bool variant_to_data(const Variant& value, uint8_t *buffer) const override;

protected:
	static void _bind_methods();

	Ref<CFFIType> element_type;
};

}

#endif  // __CFFI_POINTER_TYPE_HPP__