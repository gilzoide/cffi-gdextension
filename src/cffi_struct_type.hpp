#ifndef __CFFI_STRUCT_TYPE_HPP__
#define __CFFI_STRUCT_TYPE_HPP__

#include "cffi_type.hpp"
#include "cffi_type_tuple.hpp"

#include <godot_cpp/templates/hash_map.hpp>

using namespace godot;

namespace cffi {

class CFFIScope;

/**
 * `CFFIType` specialized for Struct types.
 */
class CFFIStructType : public CFFITypeTuple, public CFFIType {
	GDCLASS(CFFIStructType, CFFIType);
public:
	/**
	 * Necessary to define a Godot class.
	 * @warning Never use this constructor.
	 */
	CFFIStructType();

	/**
	 * @return Type of the field named `field_name`.
	 *         Returns `null` if the field cannot be found in the struct.
	 */
	Ref<CFFIType> type_of(const StringName& field_name) const;
	/**
	 * @return Type of the field named `field_name`, or `null` if there is no such field.
	 *
	 * Unlike `type_of`, an unknown field name is not an error. Use this when
	 * probing whether a name is a field at all, such as when resolving a
	 * property access that may be referring to something else entirely.
	 */
	Ref<CFFIType> find_type_of(const StringName& field_name) const;
	/**
	 * Resolve a field in a single probe, for the hot path.
	 *
	 * Returns the field's type — borrowed, valid for as long as this struct type
	 * is — and writes its byte offset into `r_offset`, or returns null if there
	 * is no such field. Not bound to GDScript: it exists so reading a field does
	 * one hash lookup instead of two, and no reference count traffic.
	 */
	const Ref<CFFIType> *find_field(const StringName& field_name, int64_t& r_offset) const;
	/**
	 * @return Offset of the field named `field_name`, in bytes.
	 *         Returns `-1` if the field cannot be found in the struct.
	 */
	int64_t offset_of(const StringName& field_name) const;

	/**
	 * Serializes the struct data read from `ptr` into a Dictionary.
	 *
	 * The returned dictionary's keys use the same name as the struct field names.
	 *
	 * @warning `ptr` must point to an instance of this struct type.
	 */
	Dictionary get_dictionary_from_struct_data(const uint8_t *ptr) const;
	/**
	 * Serializes `dict` into `buffer`, using only the values matching the struct's field names.
	 *
	 * Fields that are not present in the dictionary will be zero-initialized.
	 *
	 * @warning `buffer` must point to an instance of this struct type.
	 */
	void dictionary_to_data(const Dictionary& dict, uint8_t *buffer) const;

	/**
	 * Write a `CFFIValue` with a shallow copy of this struct's data into `r_variant`.
	 *
	 * @return True.
	 */
	bool data_to_variant(const uint8_t *data, Variant& r_variant) const override;
	/**
	 * Write the Variant `value` into `buffer`.
	 *
	 * Only Dictionary and CFFIValues with this struct type are supported.
	 *
	 * @return True if the conversion succeeded, false otherwise.
	 */
	bool variant_to_data(const Variant& value, uint8_t *buffer) const override;

	/**
	 * Create a struct type with `name` and `fields`.
	 *
	 * `fields` keys should be strings, while its values can be either strings or `CFFIType`s.
	 * @note Iterating a Dictionary returns the values in insertion order.
	 *       Thus the order of definition for the struct fields is guaranteed to be the same as in the dictionary.
	 *
	 * @return New struct type definition or `null` if an error occurred.
	 */
	static Ref<CFFIStructType> from_dictionary(const String& name, const Dictionary& fields, CFFIScope *type_scope);

protected:
	static void _bind_methods();
	String _to_string() const override;

	HashMap<StringName, int> field_map;
	/**
	 * Field name to index in `offsets`.
	 *
	 * Differs from `field_map` for a field declared after a zero-sized array: the
	 * FFI gives a zero-sized field no slot of its own, so it and the following
	 * field share one, and `offsets` has fewer meaningful entries than `fields`.
	 */
	HashMap<StringName, int> offset_map;
	/**
	 * Declared field index to `offsets` index, so a name resolved through
	 * `field_map` can reach its offset without a second hash lookup.
	 */
	LocalVector<int> offset_index_by_field;
	LocalVector<size_t> offsets;

private:
	CFFIStructType(const String& name, CFFITypeTuple&& fields, HashMap<StringName, int>&& field_map, HashMap<StringName, int>&& offset_map, LocalVector<int>&& offset_index_by_field);

	ffi_type create_struct_type();
	void fill_extra_offsets();
};

}

#endif  // __CFFI_STRUCT_TYPE_HPP__