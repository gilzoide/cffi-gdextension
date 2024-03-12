#ifndef __CFFI_VALUE_TUPLE_HPP__
#define __CFFI_VALUE_TUPLE_HPP__

#include "cffi_type.hpp"

#include <godot_cpp/templates/local_vector.hpp>

using namespace godot;

namespace cffi {

typedef LocalVector<void*, int> voidp_Vector;
class FFITypeTuple;

class FFIValueTuple {
public:
	int size() const;
	void **get_value_addresses();

	static FFIValueTuple from_array(const FFITypeTuple& types, const Array& array);

private:
	PackedByteArray bytes;
	voidp_Vector value_addresses;
};

}

#endif  // __CFFI_VALUE_TUPLE_HPP__