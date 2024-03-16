#ifndef __CFFI_VALUE_TUPLE_HPP__
#define __CFFI_VALUE_TUPLE_HPP__

#include <godot_cpp/templates/local_vector.hpp>

using namespace godot;

namespace cffi {

typedef LocalVector<void*> voidp_Vector;
class CFFITypeTuple;

class CFFIValueTuple {
public:
	CFFIValueTuple();
	CFFIValueTuple(const PackedByteArray& bytes, const PackedInt32Array& byteOffsets);

	uint32_t size() const;
	void * const *get_value_addresses() const;

	static CFFIValueTuple from_varargs(const CFFITypeTuple& type_tuple, const Variant **args, GDExtensionInt arg_count);
	static CFFIValueTuple from_array(const CFFITypeTuple& type_tuple, const Array& array);

private:
	PackedByteArray bytes;
	voidp_Vector value_addresses;
};

}

#endif  // __CFFI_VALUE_TUPLE_HPP__