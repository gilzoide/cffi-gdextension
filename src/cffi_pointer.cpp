#include "cffi_pointer.hpp"
#include "cffi_pointer_type.hpp"

namespace cffi {

class CFFIPointerType;

CFFIPointer::CFFIPointer() {}
CFFIPointer::CFFIPointer(Ref<CFFIType> type, uint8_t *address) : element_type(type), address(address) {}

uint8_t *CFFIPointer::address_offset_by(int offset) const {
	return address + element_type->get_size() * offset;
}

Ref<CFFIPointer> CFFIPointer::offset_by(int offset) const {
	return memnew(CFFIPointer(element_type, address_offset_by(offset)));
}

Variant CFFIPointer::get_value(int index) const {
	Variant value;
	if (element_type->data_to_variant(address_offset_by(index), value)) {
		return value;
	}
	else {
		return Variant();
	}
}

bool CFFIPointer::set_value(const Variant& value, int index) const {
	return element_type->variant_to_data(value, address_offset_by(index));
}

Ref<CFFIType> CFFIPointer::get_element_type() const {
	return element_type;
}

Ref<CFFIPointer> CFFIPointer::cast_elements(const Variant& type) const {
	Ref<CFFIType> new_type = CFFIType::from_variant(type, nullptr);
	if (new_type.is_valid()) {
		return Ref<CFFIPointer>(memnew(CFFIPointer(new_type, address)));
	}
	else {
		return nullptr;
	}
}

String CFFIPointer::get_string_from_ascii(int length) const {
	ERR_FAIL_COND_V_EDMSG(element_type->get_size() != sizeof(char), "", String("Element mismatch, expected char, found %s") % element_type->get_name());
	String s;
	godot::internal::gdextension_interface_string_new_with_latin1_chars_and_len(s._native_ptr(), (const char *) address, length);
	return s;
}

String CFFIPointer::get_string_from_utf8(int length) const {
	ERR_FAIL_COND_V_EDMSG(element_type->get_size() != sizeof(char), "", String("Element mismatch, expected char, found %s") % element_type->get_name());
	String s;
	godot::internal::gdextension_interface_string_new_with_utf8_chars_and_len(s._native_ptr(), (const char *) address, length);
	return s;
}

String CFFIPointer::get_string_from_utf16(int length) const {
	ERR_FAIL_COND_V_EDMSG(element_type->get_size() != sizeof(char16_t), "", String("Element mismatch, expected char16_t, found %s") % element_type->get_name());
	String s;
	godot::internal::gdextension_interface_string_new_with_utf16_chars_and_len(s._native_ptr(), (const char16_t *) address, length);
	return s;
}

String CFFIPointer::get_string_from_utf32(int length) const {
	ERR_FAIL_COND_V_EDMSG(element_type->get_size() != sizeof(char32_t), "", String("Element mismatch, expected char32_t, found %s") % element_type->get_name());
	String s;
	godot::internal::gdextension_interface_string_new_with_utf32_chars_and_len(s._native_ptr(), (const char32_t *) address, length);
	return s;
}

String CFFIPointer::get_string_from_wchar(int length) const {
	ERR_FAIL_COND_V_EDMSG(element_type->get_size() != sizeof(wchar_t), "", String("Element mismatch, expected wchar_t, found %s") % element_type->get_name());
	String s;
	godot::internal::gdextension_interface_string_new_with_wide_chars_and_len(s._native_ptr(), (const wchar_t *) address, length);
	return s;
}

PackedByteArray CFFIPointer::get_buffer(int length) const {
	ERR_FAIL_COND_V_EDMSG(length < 0, PackedByteArray(), "Buffer length cannot be negative");
	int length_in_bytes = length * element_type->get_size();
	PackedByteArray array;
	array.resize(length_in_bytes);
	memcpy(array.ptrw(), address, length_in_bytes);
	return array;
}

Array CFFIPointer::to_array(int length) const {
	ERR_FAIL_COND_V_EDMSG(length < 0, Array(), "Array length cannot be negative");
	Array array;
	array.resize(length);
	for (int i = 0; i < length; i++) {
		if (!element_type->data_to_variant(address_offset_by(i), array[i])) {
			return Array();
		}
	}
	return array;
}

void CFFIPointer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("offset_by", "offset"), &CFFIPointer::offset_by);
	ClassDB::bind_method(D_METHOD("get_value", "index"), &CFFIPointer::get_value, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("set_value", "value", "index"), &CFFIPointer::set_value, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("get_element_type"), &CFFIPointer::get_element_type);
	ClassDB::bind_method(D_METHOD("cast_elements", "element_type"), &CFFIPointer::cast_elements);
	ClassDB::bind_method(D_METHOD("get_string_from_ascii", "length"), &CFFIPointer::get_string_from_ascii, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_string_from_utf8", "length"), &CFFIPointer::get_string_from_utf8, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_string_from_utf16", "length"), &CFFIPointer::get_string_from_utf16, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_string_from_utf32", "length"), &CFFIPointer::get_string_from_utf32, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_string_from_wchar", "length"), &CFFIPointer::get_string_from_wchar, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_buffer", "length"), &CFFIPointer::get_buffer);
	ClassDB::bind_method(D_METHOD("to_array", "length"), &CFFIPointer::to_array);
}

String CFFIPointer::_to_string() const {
	return String("[%s:(%s*) 0x%x]") % Array::make(get_class_static(), element_type->get_name(), (uint64_t) address);
}

}