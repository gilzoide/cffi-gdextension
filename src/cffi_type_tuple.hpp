#ifndef __CFFI_STRUCT_TYPE_HPP__
#define __CFFI_STRUCT_TYPE_HPP__

#include "cffi_type.hpp"

#include <godot_cpp/templates/local_vector.hpp>

using namespace godot;

namespace cffi {

typedef LocalVector<Ref<FFIType>, int> FFITypeVector;

class FFITypeTuple {
public:
	FFITypeTuple();
	FFITypeTuple(FFITypeVector&& fields);
	FFITypeTuple(const FFITypeVector& fields);

	const int size() const;
	const FFITypeVector& get_fields() const;

	String to_string() const;

	ffi_type **alloc_argument_types() const;

	static FFITypeTuple from_array(const Array& array);

private:
	FFITypeVector fields;
};

}

#endif  // __CFFI_STRUCT_TYPE_HPP__