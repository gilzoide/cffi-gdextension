#ifndef __CFFI_TYPE_TUPLE_HPP__
#define __CFFI_TYPE_TUPLE_HPP__

#include <ffi.h>
#include <godot_cpp/templates/local_vector.hpp>

using namespace godot;

namespace cffi {

class CFFIType;
typedef LocalVector<Ref<CFFIType>> CFFITypeVector;
typedef LocalVector<ffi_type*> ffi_typeVector;

class CFFITypeTuple {
public:
	CFFITypeTuple();
	CFFITypeTuple(CFFITypeVector&& fields);
	CFFITypeTuple(const CFFITypeVector& fields);

	uint32_t size() const;
	const CFFITypeVector& get_fields() const;

	String to_string() const;

	ffi_type **get_argument_types();

	static CFFITypeTuple from_array(const Array& array);

protected:
	CFFITypeVector fields;
	ffi_typeVector ffi_handle;
};

}

#endif  // __CFFI_TYPE_TUPLE_HPP__