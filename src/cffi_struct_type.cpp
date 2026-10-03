#include "cffi_pointer.hpp"
#include "cffi_struct_type.hpp"
#include "cffi_type_tuple.hpp"
#include "cffi_owned_value.hpp"

#define ROUND_UP(size, alignment) ((size) + ((alignment) - 1)) & (~((alignment) - 1))

namespace cffi {

CFFIStructType::CFFIStructType() {}
CFFIStructType::CFFIStructType(const String& name, CFFITypeTuple&& fields, HashMap<StringName, int>&& field_map, HashMap<StringName, int>&& offset_map)
	: CFFITypeTuple(fields)
	, CFFIType(name, create_struct_type())
	, field_map(field_map)
	, offset_map(offset_map)
{
	offsets.resize(fields.size());
	switch (ffi_get_struct_offsets(FFI_DEFAULT_ABI, &ffi_handle, offsets.ptr())) {
		case FFI_BAD_ABI:
			ERR_PRINT_ED("Invalid ABI");
			break;

		case FFI_BAD_TYPEDEF:
			ERR_PRINT_ED("Invalid struct definition");
			break;

		default:
			break;
	}
	fill_extra_offsets();
}

Ref<CFFIStructType> CFFIStructType::from_dictionary(const String& name, const Dictionary& fields, CFFIScope *type_scope) {
	CFFITypeTuple field_types = CFFITypeTuple::from_array(fields.values(), type_scope);
	if (field_types.size() != fields.size()) {
		return nullptr;
	}
	Array names = fields.keys();
	// Two index spaces, so two maps. `fields` is indexed by declaration order;
	// `offsets` is indexed the way the FFI sees the struct, where a zero-sized
	// field (a flexible array member) has no slot of its own and shares the one
	// belonging to the field after it. Mapping a name to the FFI index and then
	// using that to look up `fields` gave every field declared after a flexible
	// array member the flexible member's own type.
	HashMap<StringName, int> field_map;
	HashMap<StringName, int> offset_map;
	for (int i = 0, offset_index = 0; i < names.size(); i++) {
		field_map[names[i]] = i;
		offset_map[names[i]] = offset_index;
		if (field_types.get_fields()[i]->get_size() != 0) {
			++offset_index;
		}
	}
	return memnew(CFFIStructType(name, std::move(field_types), std::move(field_map), std::move(offset_map)));
}

Ref<CFFIType> CFFIStructType::type_of(const StringName& field_name) const {
	Ref<CFFIType> type = find_type_of(field_name);
	ERR_FAIL_COND_V_EDMSG(type.is_null(), nullptr, String("Unknown field: \"%s\"") % field_name);
	return type;
}

Ref<CFFIType> CFFIStructType::find_type_of(const StringName& field_name) const {
	const int *index_ptr = field_map.getptr(field_name);
	return index_ptr ? fields[*index_ptr] : Ref<CFFIType>();
}

int64_t CFFIStructType::offset_of(const StringName& field_name) const {
	const int *index_ptr = offset_map.getptr(field_name);
	ERR_FAIL_COND_V_EDMSG(index_ptr == nullptr, -1, String("Unknown field: \"%s\"") % field_name);
	return offsets[*index_ptr];
}

Dictionary CFFIStructType::get_dictionary_from_struct_data(const uint8_t *ptr) const {
	Dictionary dict;
	for (auto it : field_map) {
		int index = it.value;
		fields[index]->data_to_variant(ptr + offsets[offset_map[it.key]], dict[it.key]);
	}
	return dict;
}

void CFFIStructType::dictionary_to_data(const Dictionary& dict, uint8_t *buffer) const {
	for (auto it : field_map) {
		auto field_type = fields[it.value];
		Variant value = dict.get(it.key, Variant());
		size_t offset = offsets[offset_map[it.key]];
		if (value.booleanize()) {
			field_type->variant_to_data(value, buffer + offset);
		}
		else {
			memset(buffer + offset, 0, field_type->get_size());
		}
	}
}

bool CFFIStructType::data_to_variant(const uint8_t *ptr, Variant& r_variant) const {
	r_variant = memnew(CFFIOwnedValue(Ref<CFFIType>(this), ptr));
	return true;
}

bool CFFIStructType::variant_to_data(const Variant& value, uint8_t *buffer) const {
	switch (value.get_type()) {
		case Variant::Type::DICTIONARY: {
			dictionary_to_data(value, buffer);
			return true;
		}

		case Variant::Type::OBJECT: {
			if (auto ffi_value = Object::cast_to<CFFIPointer>(value)) {
				if (ffi_value->get_element_type() == this) {
					memcpy(buffer, ffi_value->address_offset_by(0), get_size());
					return true;
				}
			}
			break;
		}

		default:
			break;
	}
	ERR_FAIL_V_EDMSG(false, String("Invalid type \"%s\" for struct type \"%s\"") % Array::make(value.get_type_name(value.get_type()), name));
}

ffi_type CFFIStructType::create_struct_type() {
	ffi_type type = {};
	type.type = FFI_TYPE_STRUCT;
	type.elements = get_element_types();
	return type;
}

void CFFIStructType::fill_extra_offsets() {
	unsigned int ffi_fields_size = ffi_fields.size() - 1; // -1 to account for the ending nullptr
	if (ffi_fields_size == fields.size()) {
		return;
	}

	int alignment = get_alignment();
	for (unsigned int i = ffi_fields_size; i < fields.size(); ++i) {
		alignment = MAX(alignment, fields[i]->get_alignment());
	}

	ffi_handle.size = ROUND_UP(get_size(), alignment);
	for (unsigned int i = ffi_fields_size; i < fields.size(); ++i) {
		offsets[i] = ffi_handle.size;
	}
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
