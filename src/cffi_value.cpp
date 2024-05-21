#include "cffi_pointer.hpp"
#include "cffi_struct_type.hpp"
#include "cffi_type.hpp"
#include "cffi_value.hpp"

namespace cffi {

CFFIValue::CFFIValue() {}
CFFIValue::CFFIValue(Ref<CFFIType> type, bool initialize_with_zeros) : type(type) {
	address = (uint8_t *) memalloc(type->get_size());
	ERR_FAIL_COND_EDMSG(address == nullptr, String("Could not allocate %d bytes for %s") % Array::make(type->get_size(), type->get_name()));
	if (initialize_with_zeros) {
		memset(address, 0, type->get_size());
	}
}
CFFIValue::CFFIValue(Ref<CFFIType> type, const uint8_t *existing_data) : type(type) {
	address = (uint8_t *) memalloc(type->get_size());
	ERR_FAIL_COND_EDMSG(address == nullptr, String("Could not allocate %d bytes for %s") % Array::make(type->get_size(), type->get_name()));
	if (existing_data) {
		memcpy(address, existing_data, type->get_size());
	}
}

CFFIValue::~CFFIValue() {
	if (address) {
		memfree(address);
	}
}

Ref<CFFIType> CFFIValue::get_type() const {
	return type;
}

Variant CFFIValue::get_value() const {
	Variant var;
	type->get_return_value(address, var);
	return var;
}

bool CFFIValue::set_value(const Variant& value) const {
	return type->serialize_value_into(value, address);
}

Ref<CFFIPointer> CFFIValue::get_address() const {
	return memnew(CFFIPointer(type, address));
}

Ref<CFFIPointer> CFFIValue::address_of(const StringName& field) const {
	auto struct_type = Object::cast_to<CFFIStructType>(type.ptr());
	ERR_FAIL_COND_V_EDMSG(!struct_type, nullptr, "Only struct and union values support address_of");

	auto field_type = struct_type->type_of(field);
	if (field_type.is_null()) {
		return nullptr;
	}

	int offset = struct_type->offset_of(field);
	return memnew(CFFIPointer(field_type, address + offset));
}

Dictionary CFFIValue::to_dictionary() const {
	auto struct_type = Object::cast_to<CFFIStructType>(type.ptr());
	ERR_FAIL_COND_V_EDMSG(!struct_type, Dictionary(), "Only struct and union values support to_dictionary");
	return struct_type->get_dictionary_from_struct_data(address);
}

bool CFFIValue::_get(const StringName& property_name, Variant& r_value) const {
	auto field_ptr = address_of(property_name);
	if (field_ptr.is_valid()) {
		r_value = field_ptr->get_value();
		return true;
	}
	else {
		return false;
	}
}

bool CFFIValue::_set(const StringName& property_name, const Variant& value) {
	auto field_ptr = address_of(property_name);
	if (field_ptr.is_valid()) {
		field_ptr->set_value(value);
		return true;
	}
	else {
		return false;
	}
}

void CFFIValue::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_type"), &CFFIValue::get_type);
	ClassDB::bind_method(D_METHOD("get_value"), &CFFIValue::get_value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &CFFIValue::set_value);
	ClassDB::bind_method(D_METHOD("get_address"), &CFFIValue::get_address);
	ClassDB::bind_method(D_METHOD("address_of", "field_name"), &CFFIValue::address_of);
	ClassDB::bind_method(D_METHOD("to_dictionary"), &CFFIValue::to_dictionary);
}

String CFFIValue::_to_string() const {
	Variant value;
	if (auto struct_type = Object::cast_to<CFFIStructType>(type.ptr())) {
		value = struct_type->get_dictionary_from_struct_data(address);
	}
	else {
		value = get_value();
	}
	return String("[%s:(%s) %s]") % Array::make(get_class(), type->get_name(), value);
}

}