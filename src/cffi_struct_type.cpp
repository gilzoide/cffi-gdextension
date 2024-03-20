#include "cffi_struct_type.hpp"
#include "cffi_type_tuple.hpp"

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

ffi_type CFFIStructType::create_struct_type() {
	ffi_type type = {};
	type.type = FFI_TYPE_STRUCT;
	type.elements = get_argument_types();
	return type;
}

void CFFIStructType::_bind_methods() {
}

String CFFIStructType::_to_string() const {
	Dictionary dict;
	for (KeyValue<StringName, int> it : field_map) {
		dict[it.key] = fields[it.value]->get_name();
	}
	return String("[%s:%s %s]") % Array::make(get_class(), name, dict);
}

}
