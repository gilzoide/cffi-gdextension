#include "cffi_pointer_type.hpp"
#include "cffi_pointer.hpp"

namespace cffi {

CFFIPointerType::CFFIPointerType() {}
CFFIPointerType::CFFIPointerType(Ref<CFFIType> element_type)
	: CFFIType(element_type->get_name() + String("*"), ffi_type_pointer)
	, element_type(element_type)
{
}

Ref<CFFIType> CFFIPointerType::get_element_type() const {
	return element_type;
}

bool CFFIPointerType::data_to_variant(const uint8_t *ptr, Variant& r_variant) const {
	uint8_t *value = *(uint8_t **) ptr;
	r_variant = memnew(CFFIPointer(element_type, value));
	return true;
}

bool CFFIPointerType::variant_to_data(const Variant& value, uint8_t *buffer) const {
	switch (value.get_type()) {
		case Variant::Type::PACKED_BYTE_ARRAY: {
			PackedByteArray bytes = value;
			*(const uint8_t **) buffer = bytes.ptr();
			return true;
		}

		case Variant::Type::OBJECT: {
			if (auto pointer_value = Object::cast_to<CFFIPointer>(value)) {
				*(uint8_t **) buffer = pointer_value->address_offset_by(0);
				return true;
			}
		}

		default:
			break;
	}
	ERR_FAIL_V_EDMSG(false, String("Invalid type \"%s\" for pointer type \"%s\"") % Array::make(value.get_type_name(value.get_type()), name));
}

void CFFIPointerType::_bind_methods() {
}

}