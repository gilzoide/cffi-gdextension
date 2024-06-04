#include "cffi_owned_value.hpp"
#include "cffi_struct_type.hpp"
#include "cffi_type.hpp"

namespace cffi {

CFFIOwnedValue::CFFIOwnedValue() {}
CFFIOwnedValue::CFFIOwnedValue(Ref<CFFIType> type, bool initialize_with_zeros)
	: CFFIPointer(type, (uint8_t *) memalloc(type->get_size()))
{
	ERR_FAIL_COND_EDMSG(address == nullptr, String("Could not allocate %d bytes for %s") % Array::make(type->get_size(), type->get_name()));
	if (initialize_with_zeros) {
		memset(address, 0, type->get_size());
	}
}
CFFIOwnedValue::CFFIOwnedValue(Ref<CFFIType> type, const uint8_t *existing_data)
	: CFFIPointer(type, (uint8_t *) memalloc(type->get_size()))
{
	ERR_FAIL_COND_EDMSG(address == nullptr, String("Could not allocate %d bytes for %s") % Array::make(type->get_size(), type->get_name()));
	if (existing_data) {
		memcpy(address, existing_data, type->get_size());
	}
}

CFFIOwnedValue::~CFFIOwnedValue() {
	if (address) {
		memfree(address);
	}
}

Ref<CFFIPointer> CFFIOwnedValue::get_base_address() const {
	return offset_by(0);
}

void CFFIOwnedValue::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_base_address"), &CFFIOwnedValue::get_base_address);
}

String CFFIOwnedValue::_to_string() const {
	Variant value;
	if (auto struct_type = Object::cast_to<CFFIStructType>(element_type.ptr())) {
		value = struct_type->get_dictionary_from_struct_data(address);
	}
	else {
		value = get_value();
	}
	return String("[%s:(%s) %s]") % Array::make(get_class(), element_type->get_name(), value);
}

}