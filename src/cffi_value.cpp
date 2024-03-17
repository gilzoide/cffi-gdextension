#include "cffi_value.hpp"
#include "cffi_pointer.hpp"
#include "cffi_type.hpp"

namespace cffi {

CFFIValue::CFFIValue() {}
CFFIValue::CFFIValue(Ref<CFFIType> type, bool initialize_with_zeros) : type(type) {
	address = (uint8_t *) memalloc(type->get_size());
	if (initialize_with_zeros) {
		memset(address, 0, type->get_size());
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

void CFFIValue::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_type"), &CFFIValue::get_type);
	ClassDB::bind_method(D_METHOD("get_value"), &CFFIValue::get_value);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &CFFIValue::set_value);
	ClassDB::bind_method(D_METHOD("get_address"), &CFFIValue::get_address);
}

String CFFIValue::_to_string() const {
	return String("[%s:(%s) %s]") % Array::make(get_class(), type->get_name(), get_value());
}

}