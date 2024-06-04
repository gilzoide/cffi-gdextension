#include "cffi_owned_value.hpp"
#include "cffi_pointer.hpp"
#include "cffi_pointer_type.hpp"
#include "cffi_struct_type.hpp"

namespace cffi {

class CFFIPointerType;

CFFIPointer::CFFIPointer() {}
CFFIPointer::CFFIPointer(Ref<CFFIType> element_type, uint8_t *address)
	: element_type(element_type)
	, address(address)
{
}

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

Ref<CFFIOwnedValue> CFFIPointer::duplicate() const {
	return memnew(CFFIOwnedValue(element_type, address));
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

PackedByteArray CFFIPointer::to_byte_array(int length) const {
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

Ref<CFFIPointer> CFFIPointer::get_field(const StringName& field) const {
	auto struct_type = Object::cast_to<CFFIStructType>(element_type.ptr());
	ERR_FAIL_COND_V_EDMSG(!struct_type, nullptr, "Only struct and union values support address_of");

	auto field_type = struct_type->type_of(field);
	if (field_type.is_null()) {
		return nullptr;
	}

	int offset = struct_type->offset_of(field);
	return memnew(CFFIPointer(field_type, address + offset));
}

Dictionary CFFIPointer::to_dictionary() const {
	auto struct_type = Object::cast_to<CFFIStructType>(element_type.ptr());
	ERR_FAIL_COND_V_EDMSG(!struct_type, Dictionary(), "Only struct and union values support to_dictionary");
	return struct_type->get_dictionary_from_struct_data(address);
}

void CFFIPointer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("cast_elements", "element_type"), &CFFIPointer::cast_elements);
	ClassDB::bind_method(D_METHOD("duplicate"), &CFFIPointer::duplicate);
	ClassDB::bind_method(D_METHOD("get_element_type"), &CFFIPointer::get_element_type);
	ClassDB::bind_method(D_METHOD("get_field", "field_name"), &CFFIPointer::get_field);
	ClassDB::bind_method(D_METHOD("get_string_from_ascii", "length"), &CFFIPointer::get_string_from_ascii, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_string_from_utf8", "length"), &CFFIPointer::get_string_from_utf8, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_string_from_utf16", "length"), &CFFIPointer::get_string_from_utf16, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_string_from_utf32", "length"), &CFFIPointer::get_string_from_utf32, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_string_from_wchar", "length"), &CFFIPointer::get_string_from_wchar, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_value", "index"), &CFFIPointer::get_value, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("offset_by", "offset"), &CFFIPointer::offset_by);
	ClassDB::bind_method(D_METHOD("set_value", "value", "index"), &CFFIPointer::set_value, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("to_array", "length"), &CFFIPointer::to_array);
	ClassDB::bind_method(D_METHOD("to_byte_array", "length"), &CFFIPointer::to_byte_array);
	ClassDB::bind_method(D_METHOD("to_dictionary"), &CFFIPointer::to_dictionary);
}

bool CFFIPointer::_get(const StringName& property_name, Variant& r_value) const {
	auto field_ptr = get_field(property_name);
	if (field_ptr.is_valid()) {
		if (Object::cast_to<CFFIStructType>(field_ptr->element_type.ptr())) {
			r_value = field_ptr;
		}
		else {
			r_value = field_ptr->get_value();
		}
		return true;
	}
	else {
		return false;
	}
}

bool CFFIPointer::_set(const StringName& property_name, const Variant& value) {
	auto field_ptr = get_field(property_name);
	if (field_ptr.is_valid()) {
		field_ptr->set_value(value);
		return true;
	}
	else {
		return false;
	}
}

String CFFIPointer::_to_string() const {
	return String("[%s:(%s*) 0x%x]") % Array::make(get_class_static(), element_type->get_name(), (uint64_t) address);
}

}