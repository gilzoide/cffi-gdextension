#ifndef __CFFI_OWNED_VALUE_HPP__
#define __CFFI_OWNED_VALUE_HPP__

#include "cffi_pointer.hpp"

using namespace godot;

namespace cffi {

/**
 * Godot object that stores a pointer with managed memory.
 *
 * This object allocates memory for the value upon construction and releases it when destroyed.
 */
class CFFIOwnedValue : public CFFIPointer {
	GDCLASS(CFFIOwnedValue, CFFIPointer);
public:
	/**
	 * Necessary to define a Godot class.
	 * @warning Never use this constructor.
	 */
	CFFIOwnedValue();
	/**
	 * Allocate a new value of `type`.
	 *
	 * @param type  The FFI type for this value.
	 *        Must not be null.
	 * @param initialize_with_zeros  If true, the allocated value will be zero-initialized.
	 *        Otherwise, the allocated memory will not be initialized and may contain garbage data.
	 */
	CFFIOwnedValue(Ref<CFFIType> type, bool initialize_with_zeros = true);
	/**
	 * Allocate a new value of `type`, copying bytes from `existing_data`.
	 *
	 * @param type  The FFI type for this value.
	 *        Must not be null.
	 * @param existing_data  Pointer to the raw data that should be copied to the new value.
	 *        This should be a valid pointer to a block of data with at least the given `type` size.
	 *        If null is passed, the new value will not be initialized and may contain garbage data.
	 */
	CFFIOwnedValue(Ref<CFFIType> type, const uint8_t *existing_data);
	/**
	 * Frees the allocated memory for this value.
	 */
	~CFFIOwnedValue();

	/**
	 * Get base address.
	 *
	 * Alias for `offset_by(0)`.
	 */
	Ref<CFFIPointer> get_base_address() const;

protected:
	static void _bind_methods();
	String _to_string() const override;
};

}

#endif  // __CFFI_OWNED_VALUE_HPP__