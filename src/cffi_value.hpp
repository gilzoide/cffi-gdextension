#ifndef __CFFI_VALUE_HPP__
#define __CFFI_VALUE_HPP__

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

namespace cffi {

class CFFIPointer;
class CFFIType;

/**
 * Godot object that stores a typed FFI value.
 *
 * This object allocates memory for the value upon construction and releases it when destroyed.
 */
class CFFIValue : public RefCounted {
	GDCLASS(CFFIValue, RefCounted);
public:
	/**
	 * Necessary to define a Godot class.
	 * @warning Never use this constructor.
	 */
	CFFIValue();
	/**
	 * Allocate a new value of `type`.
	 *
	 * @param type  The FFI type for this value.
	 *        Must not be null.
	 * @param initialize_with_zeros  If true, the allocated value will be zero-initialized.
	 *        Otherwise, the allocated memory will not be initialized and may contain garbage data.
	 */
	CFFIValue(Ref<CFFIType> type, bool initialize_with_zeros = true);
	/**
	 * Allocate a new value of `type`, copying bytes from `existing_data`.
	 *
	 * @param type  The FFI type for this value.
	 *        Must not be null.
	 * @param existing_data  Pointer to the raw data that should be copied to the new value.
	 *        This should be a valid pointer to a block of data with at least the given `type` size.
	 *        If null is passed, the new value will not be initialized and may contain garbage data.
	 */
	CFFIValue(Ref<CFFIType> type, const uint8_t *existing_data);
	/**
	 * Frees the allocated memory for this value.
	 */
	~CFFIValue();

	/**
	 * Get the type of this value.
	 */
	Ref<CFFIType> get_type() const;
	/**
	 * Get this value as a Variant.
	 */
	Variant get_value() const;
	/**
	 * Set this value's data using the Variant `value`.
	 */
	bool set_value(const Variant& value);
	/**
	 * Get the pointer to this value's address.
	 */
	Ref<CFFIPointer> get_address() const;

	// MARK: Struct/Union values only
	/**
	 * Get the address of a Struct or Union field.
	 *
	 * @note Only available on values of Struct or Union types.
	 *
	 * @return The address of the existing field, or null if it cannot be found.
	 */
	Ref<CFFIPointer> address_of(const StringName& field) const;
	/**
	 * Get a Dictionary representation of this value.
	 *
	 * @note Only available on values of Struct or Union types.
	 */
	Dictionary to_dictionary() const;

protected:
	static void _bind_methods();
	bool _get(const StringName& property_name, Variant& r_value) const;
	bool _set(const StringName& property_name, const Variant& value);
	String _to_string() const;

	Ref<CFFIType> type;
	uint8_t *address;
};

}

#endif  // __CFFI_VALUE_HPP__