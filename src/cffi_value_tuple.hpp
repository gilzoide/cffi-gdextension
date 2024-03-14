#ifndef __CFFI_VALUE_TUPLE_HPP__
#define __CFFI_VALUE_TUPLE_HPP__

#include "cffi_type.hpp"

#include <godot_cpp/templates/local_vector.hpp>

using namespace godot;

namespace cffi {

typedef LocalVector<void*> voidp_Vector;
class FFITypeTuple;

class FFIValueTuple {
public:
	FFIValueTuple();
	FFIValueTuple(const PackedByteArray& bytes, const PackedInt32Array& byteOffsets);

	uint32_t size() const;
	void * const *get_value_addresses() const;

	static FFIValueTuple from_varargs(const FFITypeTuple& type_tuple, const Variant **args, GDExtensionInt arg_count);
	static FFIValueTuple from_array(const FFITypeTuple& type_tuple, const Array& array);

private:
	PackedByteArray bytes;
	voidp_Vector value_addresses;
};

}

#endif  // __CFFI_VALUE_TUPLE_HPP__