#include "cffi_pointer.hpp"
#include "cffi_pointer_type.hpp"

namespace cffi {

class FFIPointerType;

FFIPointer::FFIPointer() {}
FFIPointer::FFIPointer(Ref<FFIType> type, uint8_t *address) : element_type(type), address(address) {}

uint8_t *FFIPointer::address_offset_by(int offset) const {
	return address + element_type->get_size() * offset;
}

Ref<FFIPointer> FFIPointer::offset_by(int offset) const {
	return memnew(FFIPointer(element_type, address_offset_by(offset)));
}

Variant FFIPointer::get_value(int index) const {
	Variant value;
	if (element_type->get_return_value(address_offset_by(index), value)) {
		return value;
	}
	else {
		return Variant();
	}
}

bool FFIPointer::set_value(const Variant& value, int index) const {
	return element_type->serialize_value_into(value, address_offset_by(index));
}

String FFIPointer::get_string_from_ascii(int length) const {
	ERR_FAIL_COND_V_EDMSG(element_type->get_size() != sizeof(char), "", String("Element mismatch, expected char, found %s") % element_type->get_name());
	String s;
	godot::internal::gdextension_interface_string_new_with_latin1_chars_and_len(s._native_ptr(), (const char *) address, length);
	return s;
}

String FFIPointer::get_string_from_utf8(int length) const {
	ERR_FAIL_COND_V_EDMSG(element_type->get_size() != sizeof(char), "", String("Element mismatch, expected char, found %s") % element_type->get_name());
	String s;
	godot::internal::gdextension_interface_string_new_with_utf8_chars_and_len(s._native_ptr(), (const char *) address, length);
	return s;
}

String FFIPointer::get_string_from_utf16(int length) const {
	ERR_FAIL_COND_V_EDMSG(element_type->get_size() != sizeof(char16_t), "", String("Element mismatch, expected char16_t, found %s") % element_type->get_name());
	String s;
	godot::internal::gdextension_interface_string_new_with_utf16_chars_and_len(s._native_ptr(), (const char16_t *) address, length);
	return s;
}

String FFIPointer::get_string_from_utf32(int length) const {
	ERR_FAIL_COND_V_EDMSG(element_type->get_size() != sizeof(char32_t), "", String("Element mismatch, expected char32_t, found %s") % element_type->get_name());
	String s;
	godot::internal::gdextension_interface_string_new_with_utf32_chars_and_len(s._native_ptr(), (const char32_t *) address, length);
	return s;
}

String FFIPointer::get_string_from_wchar(int length) const {
	ERR_FAIL_COND_V_EDMSG(element_type->get_size() != sizeof(wchar_t), "", String("Element mismatch, expected wchar_t, found %s") % element_type->get_name());
	String s;
	godot::internal::gdextension_interface_string_new_with_wide_chars_and_len(s._native_ptr(), (const wchar_t *) address, length);
	return s;
}

void FFIPointer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("offset_by", "offset"), &FFIPointer::offset_by);
	ClassDB::bind_method(D_METHOD("get_value", "index"), &FFIPointer::get_value, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("set_value", "value", "index"), &FFIPointer::set_value, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("get_string_from_ascii", "length"), &FFIPointer::get_string_from_ascii, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_string_from_utf8", "length"), &FFIPointer::get_string_from_utf8, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_string_from_utf16", "length"), &FFIPointer::get_string_from_utf16, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_string_from_utf32", "length"), &FFIPointer::get_string_from_utf32, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_string_from_wchar", "length"), &FFIPointer::get_string_from_wchar, DEFVAL(-1));
}

String FFIPointer::_to_string() const {
	return String("[%s:(%s*) 0x%x]") % Array::make(get_class_static(), element_type->get_name(), (uint64_t) address);
}

}