#ifndef __CFFI_TYPE_HPP__
#define __CFFI_TYPE_HPP__

#include <ffi.h>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/stream_peer_buffer.hpp>

using namespace godot;

namespace cffi {

class CFFIScope;
class CFFIValue;

/**
 * Godot class that represents an FFI type.
 */
class CFFIType : public RefCounted {
	GDCLASS(CFFIType, RefCounted);
public:
	/**
	 * Necessary to define a Godot class.
	 * @warning Never use this constructor.
	 */
	CFFIType();
	/**
	 * Create a type named `name` that represents the FFI type `ffi_type`.
	 */
	CFFIType(const String& name, ffi_type ffi_type);

	/**
	 * Get the FFI type.
	 */
	ffi_type& get_ffi_type();
	/**
	 * Get the FFI type.
	 */
	const ffi_type& get_ffi_type() const;
	/**
	 * Get the FFI type's alignment in bytes.
	 */
	int get_alignment() const;
	/**
	 * Get the type's name.
	 */
	const String& get_name() const;
	/**
	 * Get the FFI type's size in bytes.
	 */
	uint64_t get_size() const;

	/**
	 * Write the typed `data` into `r_variant`.
	 *
	 * @return True if the conversion succeeded, false otherwise.
	 */
	bool data_to_variant(const PackedByteArray& data, Variant& r_variant) const;
	/**
	 * Write the typed `data` into `r_variant`.
	 *
	 * @return True if the conversion succeeded, false otherwise.
	 */
	virtual bool data_to_variant(const uint8_t *data, Variant& r_variant) const;

	/**
	 * Write the Variant `value` into `buffer`.
	 *
	 * @return True if the conversion succeeded, false otherwise.
	 */
	bool variant_to_data(const Variant& value, PackedByteArray& buffer) const;
	/**
	 * Write the Variant `value` into `buffer`.
	 *
	 * @return True if the conversion succeeded, false otherwise.
	 */
	virtual bool variant_to_data(const Variant& value, uint8_t *buffer) const;

	/**
	 * Allocates a new value of this type.
	 *
	 * @param initialize_with_zeros  If true, the allocated value will be zero-initialized.
	 *        Otherwise, the allocated memory will not be initialized and may contain garbage data.
	 */
	Ref<CFFIValue> alloc(bool initialize_with_zeros = true);

	/**
	 * Get the type represented by `var`.
	 *
	 * @param var  Variant that represents a type.
	 *             Should be an instance of `CFFIType` or a `String`.
	 * @param type_scope  Type scope used to search for the type by name.
	 *                    If null, searches in the global scope represented by `CFFI`.
	 *
	 * @return If `var` is an instance of `CFFIType`, returns the type unchanged.
	 *         Otherwise, searches for a type registered in `type_scope` using `var` as name.
	 */
	static Ref<CFFIType> from_variant(const Variant& var, CFFIScope *type_scope);

protected:
	static void _bind_methods();
	virtual String _to_string() const;

	String name;
	ffi_type ffi_handle;
};

}

#endif  // __CFFI_TYPE_HPP__