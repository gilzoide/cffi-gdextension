#ifndef __CFFI_OWNED_ARRAY_HPP__
#define __CFFI_OWNED_ARRAY_HPP__

#include "cffi_span.hpp"

using namespace godot;

namespace cffi {

/**
 * Godot object that stores an array with managed memory.
 *
 * This object allocates memory for the values upon construction and releases it when destroyed.
 */
class CFFIOwnedArray : public CFFISpan {
	GDCLASS(CFFIOwnedArray, CFFISpan);
public:
	/**
	 * Necessary to define a Godot class.
	 * @warning Never use this constructor.
	 */
	CFFIOwnedArray();
	/**
	 * Allocate a new value of `type`.
	 *
	 * @param type  FFI type for the array elements.
	 *        Must not be null.
	 * @param length  Number of elements of the owned array.
	 * @param initialize_with_zeros  If true, the allocated value will be zero-initialized.
	 *        Otherwise, the allocated memory will not be initialized and may contain garbage data.
	 */
	CFFIOwnedArray(Ref<CFFIType> type, int64_t length, bool initialize_with_zeros = true);
	/**
	 * Allocate a new value of `type`, copying bytes from `existing_data`.
	 *
	 * @param type  FFI type for the array elements.
	 *        Must not be null.
	 * @param length  Number of elements of the owned array.
	 * @param existing_data  Pointer to the raw data that should be copied to the new value.
	 *        This should be a valid pointer to a block of data with at least `size` times the given `type` size.
	 *        If null is passed, the new value will not be initialized and may contain garbage data.
	 */
	CFFIOwnedArray(Ref<CFFIType> type, int64_t length, const uint8_t *existing_data);
	/**
	 * Frees the allocated memory for this value.
	 */
	virtual ~CFFIOwnedArray();

protected:
	static void _bind_methods();
};

}

#endif  // __CFFI_OWNED_ARRAY_HPP__