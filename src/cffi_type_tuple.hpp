#ifndef __CFFI_TYPE_TUPLE_HPP__
#define __CFFI_TYPE_TUPLE_HPP__

#include "cffi_type.hpp"

#include <godot_cpp/templates/local_vector.hpp>

using namespace godot;

namespace cffi {

typedef LocalVector<Ref<FFIType>> FFITypeVector;
typedef LocalVector<ffi_type*> ffi_typeVector;

class FFITypeTuple {
public:
	FFITypeTuple();
	FFITypeTuple(FFITypeVector&& fields);
	FFITypeTuple(const FFITypeVector& fields);

	uint32_t size() const;
	const FFITypeVector& get_fields() const;

	String to_string() const;

	ffi_type **get_argument_types();

	static FFITypeTuple from_array(const Array& array);

private:
	FFITypeVector fields;
	ffi_typeVector ffi_handle;
};

}

#endif  // __CFFI_TYPE_TUPLE_HPP__