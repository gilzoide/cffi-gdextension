#ifndef __CFFI_ARRAY_TYPE_HPP__
#define __CFFI_ARRAY_TYPE_HPP__

#include "cffi_type.hpp"

using namespace godot;

namespace cffi {

/**
 * `CFFIType` specialized for fixed-size array types.
 */
class CFFIArrayType : public CFFIType {
	GDCLASS(CFFIArrayType, CFFIType);
public:
	/**
	 * Necessary to define a Godot class.
	 * @warning Never use this constructor.
	 */
	CFFIArrayType();
	/**
	 * Create a array type that points to `length` elements with type `element_type`.
	 */
	CFFIArrayType(Ref<CFFIType> element_type, int64_t length);

	/**
	 * Get the element type.
	 */
	Ref<CFFIType> get_element_type() const;
	/**
	 * Number of elements in the fixed array.
	 */
	int64_t get_length() const;

	/**
	* Write a `CFFIArray` to `r_variant`.
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

	/**
	 * Construct a CFFIArrayType from the element type and length.
	 */
	static Ref<CFFIArrayType> from(const Variant& type, int64_t length);

protected:
	static void _bind_methods();
	static ffi_type create_array_type(const ffi_type& element_type, int64_t element_count);

	Ref<CFFIType> element_type;
	int64_t length;
};

}

#endif  // __CFFI_ARRAY_TYPE_HPP__