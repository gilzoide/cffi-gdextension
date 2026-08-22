#include "cffi_owned_value.hpp"
#include "cffi_span.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

namespace cffi {

class CFFIPointerType;

CFFISpan::CFFISpan() {}
CFFISpan::CFFISpan(Ref<CFFIPointer> pointer, int64_t element_count)
	: data(pointer)
	, length(element_count)
{
}

Ref<CFFIPointer> CFFISpan::get_data() const {
	return data;
}

Ref<CFFIType> CFFISpan::get_element_type() const {
	return data->get_element_type();
}

int64_t CFFISpan::get_length() const {
	return length;
}

int64_t CFFISpan::get_size_bytes() const {
	ERR_FAIL_COND_V(data == nullptr, 0);
	return get_length() * get_element_type()->get_size();
}

bool CFFISpan::is_empty() const {
	return length == 0;
}

Ref<CFFISpan> CFFISpan::subspan(int from, int count) const {
	ERR_FAIL_COND_V(from < 0, nullptr);
	ERR_FAIL_COND_V(from > length, nullptr);
	if (count < 0) {
		count = length - from;
	}
	ERR_FAIL_COND_V(from + count > length, nullptr);
	return memnew(CFFISpan(data->offset_by(from), count));
}

Ref<CFFIPointer> CFFISpan::get_pointer(int index) const {
	ERR_FAIL_COND_V(index < 0, nullptr);
	ERR_FAIL_COND_V(index >= length, nullptr);
	return data->offset_by(index);
}

Variant CFFISpan::get_value(int index) const {
	Ref<CFFIPointer> pointer = get_pointer(index);
	if (pointer.is_valid()) {
		return pointer->get_value();
	}
	else {
		return {};
	}
}

bool CFFISpan::set_value(int index, const Variant& value) const {
	ERR_FAIL_COND_V(index < 0, false);
	ERR_FAIL_COND_V(index >= length, false);
	Ref<CFFIPointer> pointer = get_pointer(index);
	if (pointer.is_valid()) {
		return pointer->set_value(value);
	}
	else {
		return {};
	}
}

Ref<CFFIOwnedValue> CFFISpan::duplicate() const {
	return data->duplicate_array(length);
}

String CFFISpan::get_string_from_ascii() const {
	ERR_FAIL_COND_V(data == nullptr, "");
	ERR_FAIL_COND_V_EDMSG(get_element_type()->get_size() != sizeof(char), "", String("Element mismatch, expected char, found %s") % get_element_type()->get_name());
	String s;
	godot::internal::gdextension_interface_string_new_with_latin1_chars_and_len(s._native_ptr(), (const char *) data->address_offset_by(0), length);
	return s;
}

String CFFISpan::get_string_from_utf8() const {
	ERR_FAIL_COND_V(data == nullptr, "");
	ERR_FAIL_COND_V_EDMSG(get_element_type()->get_size() != sizeof(char), "", String("Element mismatch, expected char, found %s") % get_element_type()->get_name());
	String s;
	godot::internal::gdextension_interface_string_new_with_utf8_chars_and_len(s._native_ptr(), (const char *) data->address_offset_by(0), length);
	return s;
}

String CFFISpan::get_string_from_utf16() const {
	ERR_FAIL_COND_V(data == nullptr, "");
	ERR_FAIL_COND_V_EDMSG(get_element_type()->get_size() != sizeof(char16_t), "", String("Element mismatch, expected char16_t, found %s") % get_element_type()->get_name());
	String s;
	godot::internal::gdextension_interface_string_new_with_utf16_chars_and_len(s._native_ptr(), (const char16_t *) data->address_offset_by(0), length);
	return s;
}

String CFFISpan::get_string_from_utf32() const {
	ERR_FAIL_COND_V(data == nullptr, "");
	ERR_FAIL_COND_V_EDMSG(get_element_type()->get_size() != sizeof(char32_t), "", String("Element mismatch, expected char32_t, found %s") % get_element_type()->get_name());
	String s;
	godot::internal::gdextension_interface_string_new_with_utf32_chars_and_len(s._native_ptr(), (const char32_t *) data->address_offset_by(0), length);
	return s;
}

String CFFISpan::get_string_from_wchar() const {
	ERR_FAIL_COND_V(data == nullptr, "");
	ERR_FAIL_COND_V_EDMSG(get_element_type()->get_size() != sizeof(wchar_t), "", String("Element mismatch, expected wchar_t, found %s") % get_element_type()->get_name());
	String s;
	godot::internal::gdextension_interface_string_new_with_wide_chars_and_len(s._native_ptr(), (const wchar_t *) data->address_offset_by(0), length);
	return s;
}

PackedByteArray CFFISpan::to_byte_array() const {
	ERR_FAIL_COND_V(data == nullptr, PackedByteArray());
	PackedByteArray array;
	array.resize(length);
	memcpy(array.ptrw(), data->address_offset_by(0), length);
	return array;
}

PackedInt32Array CFFISpan::to_int32_array() const {
	ERR_FAIL_COND_V(data == nullptr, PackedInt32Array());
	ERR_FAIL_COND_V_EDMSG(get_element_type()->get_size() != sizeof(int32_t), PackedInt32Array(), String("Element mismatch, expected int32_t, found %s") % get_element_type()->get_name());
	PackedInt32Array array;
	array.resize(length);
	memcpy(array.ptrw(), data->address_offset_by(0), length * sizeof(int32_t));
	return array;
}

PackedInt64Array CFFISpan::to_int64_array() const {
	ERR_FAIL_COND_V(data == nullptr, PackedInt64Array());
	ERR_FAIL_COND_V_EDMSG(get_element_type()->get_size() != sizeof(int64_t), PackedInt64Array(), String("Element mismatch, expected int64_t, found %s") % get_element_type()->get_name());
	PackedInt64Array array;
	array.resize(length);
	memcpy(array.ptrw(), data->address_offset_by(0), length * sizeof(int64_t));
	return array;
}

PackedFloat32Array CFFISpan::to_float32_array() const {
	ERR_FAIL_COND_V(data == nullptr, PackedFloat32Array());
	ERR_FAIL_COND_V_EDMSG(get_element_type()->get_size() != sizeof(float), PackedFloat32Array(), String("Element mismatch, expected float, found %s") % get_element_type()->get_name());
	PackedFloat32Array array;
	array.resize(length);
	memcpy(array.ptrw(), data->address_offset_by(0), length * sizeof(float));
	return array;
}

PackedFloat64Array CFFISpan::to_float64_array() const {
	ERR_FAIL_COND_V(data == nullptr, PackedFloat64Array());
	ERR_FAIL_COND_V_EDMSG(get_element_type()->get_size() != sizeof(double), PackedFloat64Array(), String("Element mismatch, expected double, found %s") % get_element_type()->get_name());
	PackedFloat64Array array;
	array.resize(length);
	memcpy(array.ptrw(), data->address_offset_by(0), length * sizeof(double));
	return array;
}

PackedVector2Array CFFISpan::to_vector2_array() const {
	ERR_FAIL_COND_V(data == nullptr, PackedVector2Array());
	ERR_FAIL_COND_V_EDMSG(get_element_type()->get_size() != sizeof(Vector2), PackedVector2Array(), String("Element mismatch, expected Vector2, found %s") % get_element_type()->get_name());
	PackedVector2Array array;
	array.resize(length);
	memcpy(array.ptrw(), data->address_offset_by(0), length * sizeof(Vector2));
	return array;
}

PackedVector3Array CFFISpan::to_vector3_array() const {
	ERR_FAIL_COND_V(data == nullptr, PackedVector3Array());
	ERR_FAIL_COND_V_EDMSG(get_element_type()->get_size() != sizeof(Vector3), PackedVector3Array(), String("Element mismatch, expected Vector3, found %s") % get_element_type()->get_name());
	PackedVector3Array array;
	array.resize(length);
	memcpy(array.ptrw(), data->address_offset_by(0), length * sizeof(Vector3));
	return array;
}

PackedVector4Array CFFISpan::to_vector4_array() const {
	ERR_FAIL_COND_V(data == nullptr, PackedVector4Array());
	ERR_FAIL_COND_V_EDMSG(get_element_type()->get_size() != sizeof(Vector4), PackedVector4Array(), String("Element mismatch, expected Vector4, found %s") % get_element_type()->get_name());
	PackedVector4Array array;
	array.resize(length);
	memcpy(array.ptrw(), data->address_offset_by(0), length * sizeof(Vector4));
	return array;
}

PackedColorArray CFFISpan::to_color_array() const {
	ERR_FAIL_COND_V(data == nullptr, PackedColorArray());
	ERR_FAIL_COND_V_EDMSG(get_element_type()->get_size() != sizeof(Color), PackedColorArray(), String("Element mismatch, expected Color, found %s") % get_element_type()->get_name());
	PackedColorArray array;
	array.resize(length);
	memcpy(array.ptrw(), data->address_offset_by(0), length * sizeof(Color));
	return array;
}

Array CFFISpan::to_array() const {
	ERR_FAIL_COND_V(data == nullptr, Array());
	Ref<CFFIType> element_type = get_element_type();
	Array array;
	array.resize(length);
	for (int i = 0; i < length; i++) {
		if (!element_type->data_to_variant(data->address_offset_by(i), array[i])) {
			return Array();
		}
	}
	return array;
}

Ref<CFFISpan> CFFISpan::from(Ref<CFFIPointer> pointer, int64_t count) {
	ERR_FAIL_COND_V(pointer == nullptr, nullptr);
	ERR_FAIL_COND_V(count < 0, nullptr);
	return memnew(CFFISpan(pointer, count));
}

void CFFISpan::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_data"), &CFFISpan::get_data);
	ClassDB::bind_method(D_METHOD("get_element_type"), &CFFISpan::get_element_type);
	ClassDB::bind_method(D_METHOD("get_length"), &CFFISpan::get_length);
	ClassDB::bind_method(D_METHOD("get_size_bytes"), &CFFISpan::get_size_bytes);
	ClassDB::bind_method(D_METHOD("is_empty"), &CFFISpan::is_empty);
	ClassDB::bind_method(D_METHOD("subspan", "from", "count"), &CFFISpan::subspan, DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("get_pointer", "index"), &CFFISpan::get_pointer);
	ClassDB::bind_method(D_METHOD("duplicate"), &CFFISpan::duplicate);
	ClassDB::bind_method(D_METHOD("get_string_from_ascii"), &CFFISpan::get_string_from_ascii);
	ClassDB::bind_method(D_METHOD("get_string_from_utf8"), &CFFISpan::get_string_from_utf8);
	ClassDB::bind_method(D_METHOD("get_string_from_utf16"), &CFFISpan::get_string_from_utf16);
	ClassDB::bind_method(D_METHOD("get_string_from_utf32"), &CFFISpan::get_string_from_utf32);
	ClassDB::bind_method(D_METHOD("get_string_from_wchar"), &CFFISpan::get_string_from_wchar);
	ClassDB::bind_method(D_METHOD("get_value", "index"), &CFFISpan::get_value);
	ClassDB::bind_method(D_METHOD("set_value", "index", "value"), &CFFISpan::set_value);
	ClassDB::bind_method(D_METHOD("to_array"), &CFFISpan::to_array);
	ClassDB::bind_method(D_METHOD("to_byte_array"), &CFFISpan::to_byte_array);
	ClassDB::bind_method(D_METHOD("to_int32_array"), &CFFISpan::to_int32_array);
	ClassDB::bind_method(D_METHOD("to_int64_array"), &CFFISpan::to_int64_array);
	ClassDB::bind_method(D_METHOD("to_float32_array"), &CFFISpan::to_float32_array);
	ClassDB::bind_method(D_METHOD("to_float64_array"), &CFFISpan::to_float64_array);
	ClassDB::bind_method(D_METHOD("to_vector2_array"), &CFFISpan::to_vector2_array);
	ClassDB::bind_method(D_METHOD("to_vector3_array"), &CFFISpan::to_vector3_array);
	ClassDB::bind_method(D_METHOD("to_vector4_array"), &CFFISpan::to_vector4_array);
	ClassDB::bind_method(D_METHOD("to_color_array"), &CFFISpan::to_color_array);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("from", "pointer", "count"), &CFFISpan::from);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_NONE, CFFIPointer::get_class_static(), PROPERTY_USAGE_NONE), "", "get_data");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "element_type", PROPERTY_HINT_NONE, CFFIType::get_class_static(), PROPERTY_USAGE_NONE), "", "get_element_type");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "length", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "", "get_length");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size_bytes", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "", "get_size_bytes");
}

String CFFISpan::_to_string() const {
	return String("[%s:%s[%d] 0x%x]") % Array::make(get_class_static(), get_element_type()->get_name(), length, (uint64_t) data->address_offset_by(0));
}

}