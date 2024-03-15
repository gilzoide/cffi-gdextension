#include "cffi_pointer_type.hpp"
#include "cffi_pointer.hpp"

namespace cffi {

FFIPointerType::FFIPointerType() {}
FFIPointerType::FFIPointerType(Ref<FFIType> element_type)
	: FFIType(element_type->get_name() + String("*"), ffi_type_pointer)
	, element_type(element_type)
	{}

Ref<FFIType> FFIPointerType::get_element_type() const {
	return element_type;
}

bool FFIPointerType::get_return_value(const uint8_t *ptr, Variant& r_variant) const {
	uint8_t *value = *(uint8_t **) ptr;
	r_variant = memnew(FFIPointer(element_type, value));
	return true;
}

bool FFIPointerType::serialize_value_into(const Variant& value, uint8_t *buffer) const {
	switch (value.get_type()) {
		case Variant::Type::PACKED_BYTE_ARRAY: {
			PackedByteArray bytes = value;
			*(uint8_t **) buffer = bytes.ptrw();
			return true;
		}

		case Variant::Type::OBJECT: {
			if (auto pointer_value = Object::cast_to<FFIPointer>(value)) {
				*(uint8_t **) buffer = pointer_value->address_offset_by(0);
				return true;
			}
		}

		default:
			break;
	}
	ERR_FAIL_V_EDMSG(false, String("Invalid type \"%s\" for pointer type \"%s\"") % Array::make(value.get_type_name(value.get_type()), name));
}

}