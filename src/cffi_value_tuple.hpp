#ifndef __CFFI_VALUE_TUPLE_HPP__
#define __CFFI_VALUE_TUPLE_HPP__

#include <godot_cpp/templates/local_vector.hpp>

using namespace godot;

namespace cffi {

class CFFITypeTuple;

/**
 * Tuple of FFI values.
 * Used solely to convert arguments from Godot Variants to raw data usable by the FFI.
 */
class CFFIValueTuple {
public:
	/**
	 * Create an empty value tuple.
	 */
	CFFIValueTuple();

	/**
	 * Get the tuple size.
	 */
	uint32_t size() const;
	/**
	 * Get the raw data pointer usable by the FFI.
	 */
	void * const *get_value_addresses() const;

	/**
	 * Create a value tuple from functions' arguments and their types.
	 *
	 * @return Value tuple with arguments.
	 *         If the number of arguments doesn't match the number of types, or
	 *         any argument cannot represent the respective type, returns an empty tuple.
	 */
	static CFFIValueTuple from_varargs(const CFFITypeTuple& type_tuple, const Variant **args, GDExtensionInt arg_count);
	/**
	 * Create a value tuple from an Array of arguments and their types.
	 *
	 * @return Value tuple with arguments.
	 *         If the number of arguments doesn't match the number of types, or
	 *         any argument cannot represent the respective type, returns an empty tuple.
	 */
	static CFFIValueTuple from_array(const CFFITypeTuple& type_tuple, const Array& array);

protected:
	/**
	 * Create a value tuple from `bytes` and a list of byte offsets.
	 */
	CFFIValueTuple(const PackedByteArray& bytes, const PackedInt32Array& byte_offsets);

private:
	PackedByteArray bytes;
	LocalVector<void *> value_addresses;
};

}

#endif  // __CFFI_VALUE_TUPLE_HPP__