#ifndef __CFFI_STRUCT_TYPE_HPP__
#define __CFFI_STRUCT_TYPE_HPP__

#include "cffi_type.hpp"
#include "cffi_type_tuple.hpp"

#include <godot_cpp/templates/hash_map.hpp>

using namespace godot;

namespace cffi {

class CFFIScope;

class CFFIStructType : public CFFITypeTuple, public CFFIType {
	GDCLASS(CFFIStructType, CFFIType);
public:
	CFFIStructType();

	Ref<CFFIType> type_of(const StringName& field_name) const;
	int64_t offset_of(const StringName& field_name) const;

	Dictionary get_dictionary_from_struct_data(const uint8_t *ptr) const;
	bool data_to_variant(const uint8_t *ptr, Variant& r_variant) const override;
	bool variant_to_data(const Variant& value, uint8_t *buffer) const override;

	static Ref<CFFIStructType> from_dictionary(const String& name, const Dictionary& fields, CFFIScope *type_scope);

protected:
	static void _bind_methods();
	String _to_string() const override;

	HashMap<StringName, int> field_map;
	LocalVector<size_t> offsets;

private:
	CFFIStructType(const String& name, CFFITypeTuple&& fields, HashMap<StringName, int>&& field_map);

	ffi_type create_struct_type();
};

}

#endif  // __CFFI_STRUCT_TYPE_HPP__