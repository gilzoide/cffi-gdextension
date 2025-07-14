#include "cffi_pointer_type.hpp"
#include "cffi.hpp"
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
	if (value) {
		r_variant = memnew(CFFIPointer(element_type, value));
	}
	else {
		r_variant = nullptr;
	}
	return true;
}

bool CFFIPointerType::variant_to_data(const Variant& value, PackedByteArray& buffer) const {
	int64_t previous_size = buffer.size();
	buffer.resize(previous_size + ffi_handle.size);
	switch (value.get_type()) {
		case Variant::Type::STRING:
		case Variant::Type::STRING_NAME: {
			size_t element_size = element_type->get_ffi_type().size;
			switch (element_size) {
				case 1:
					buffer.append_array(CFFI::null_terminated_utf8_buffer(value));
					break;

				case 2:
					buffer.append_array(CFFI::null_terminated_utf16_buffer(value));
					break;

				case 4:
					buffer.append_array(CFFI::null_terminated_utf32_buffer(value));
					break;

				default:
					ERR_FAIL_V_MSG(false, String("String is incompatible with pointer for element of size %d. Only pointers to elements of size 1, 2 or 4 are supported.") % Array::make((uint64_t) element_size));
			}
			void **ptr = (void **) (buffer.ptrw() + previous_size);
			*ptr = ptr + 1;
			return true;
		}

		default:
			return variant_to_data(value, buffer.ptrw() + previous_size);
	}
}

bool CFFIPointerType::variant_to_data(const Variant& value, uint8_t *buffer) const {
	switch (value.get_type()) {
		case Variant::Type::NIL:
			*(const uint8_t **) buffer = nullptr;
			return true;

		case Variant::Type::PACKED_BYTE_ARRAY: {
			PackedByteArray bytes = value;
			*(const uint8_t **) buffer = bytes.ptr();
			return true;
		}

		case Variant::Type::OBJECT:
			if (auto pointer_value = Object::cast_to<CFFIPointer>(value)) {
				*(uint8_t **) buffer = pointer_value->address_offset_by(0);
				return true;
			}
			break;

		default:
			break;
	}
	ERR_FAIL_V_EDMSG(false, String("Invalid type \"%s\" for pointer type \"%s\"") % Array::make(value.get_type_name(value.get_type()), name));
}

void CFFIPointerType::_bind_methods() {
}

}