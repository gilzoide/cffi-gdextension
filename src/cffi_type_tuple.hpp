#ifndef __CFFI_TYPE_TUPLE_HPP__
#define __CFFI_TYPE_TUPLE_HPP__

#include <ffi.h>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/templates/local_vector.hpp>

using namespace godot;

namespace cffi {

class CFFIType;
class CFFIScope;
typedef LocalVector<Ref<CFFIType>> CFFITypeVector;

/**
 * Tuple of FFI types.
 * Used to define FFI functions' argument types, as well as struct and union types.
 */
class CFFITypeTuple {
public:
	/**
	 * Create an empty type tuple.
	 */
	CFFITypeTuple();
	/**
	 * Create a type tuple from `fields`.
	 */
	CFFITypeTuple(CFFITypeVector&& fields);
	/**
	 * Create a type tuple from `fields`.
	 */
	CFFITypeTuple(const CFFITypeVector& fields);

	/**
	 * Get the tuple size.
	 */
	uint32_t size() const;
	/**
	 * Get the tuple's inner fields list.
	 */
	const CFFITypeVector& get_fields() const;

	/**
	 * Get a String representing this type tuple "(type0, type1, ...)".
	 */
	String to_string() const;

	/**
	 * Get the raw data pointer usable by the FFI.
	 */
	ffi_type **get_element_types();

	/**
	 * Create a type tuple from an Array of types.
	 *
	 * @see CFFIType::from_variant
	 */
	static CFFITypeTuple from_array(const Array& array, const CFFIScope *type_scope);

protected:
	CFFITypeVector fields;
	LocalVector<ffi_type*> ffi_fields;
};

}

#endif  // __CFFI_TYPE_TUPLE_HPP__