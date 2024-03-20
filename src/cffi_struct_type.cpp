#include "cffi_struct_type.hpp"
#include "cffi_type_tuple.hpp"

#define ROUND_UP(size, alignment) ((size) + ((alignment) - 1)) & (~((alignment) - 1))

namespace cffi {

CFFIStructType::CFFIStructType() {}
CFFIStructType::CFFIStructType(const String& name, CFFITypeTuple&& fields, HashMap<StringName, int>&& field_map)
	: CFFITypeTuple(fields)
	, CFFIType(name, create_struct_type())
	, field_map(field_map)
{
}

Ref<CFFIStructType> CFFIStructType::from_dictionary(const String& name, const Dictionary& fields) {
	CFFITypeTuple field_types = CFFITypeTuple::from_array(fields.values());
	Array names = fields.keys();
	if (field_types.size() != names.size()) {
		return nullptr;
	}
	HashMap<StringName, int> field_map;
	for (int i = 0; i < names.size(); i++) {
		field_map[names[i]] = i;
	}
	return memnew(CFFIStructType(name, std::move(field_types), std::move(field_map)));
}

Ref<CFFIType> CFFIStructType::type_of(const StringName& field_name) {
	const int *index_ptr = field_map.getptr(field_name);
	ERR_FAIL_COND_V_EDMSG(index_ptr == nullptr, nullptr, String("Unknown field: \"%s\"") % field_name);
	return fields[*index_ptr];
}

int CFFIStructType::offset_of(const StringName& field_name) {
	const int *index_ptr = field_map.getptr(field_name);
	ERR_FAIL_COND_V_EDMSG(index_ptr == nullptr, 0, String("Unknown field: \"%s\"") % field_name);
	int index = *index_ptr;
	int offset = 0;
	for (int i = 0; i < index; i++) {
		auto& type = fields[i]->get_ffi_type();
		offset += ROUND_UP(type.size, type.alignment);
	}
	return offset;
}

ffi_type CFFIStructType::create_struct_type() {
	ffi_type type = {};
	type.type = FFI_TYPE_STRUCT;
	type.elements = get_element_types();
	ffi_get_struct_offsets(FFI_DEFAULT_ABI, &type, nullptr);
	return type;
}

void CFFIStructType::_bind_methods() {
	ClassDB::bind_method(D_METHOD("type_of", "field_name"), &CFFIStructType::type_of);
	ClassDB::bind_method(D_METHOD("offset_of", "field_name"), &CFFIStructType::offset_of);
}

String CFFIStructType::_to_string() const {
	Dictionary dict;
	for (KeyValue<StringName, int> it : field_map) {
		dict[it.key] = fields[it.value]->get_name();
	}
	return String("[%s:%s %s]") % Array::make(get_class(), name, dict);
}

}
