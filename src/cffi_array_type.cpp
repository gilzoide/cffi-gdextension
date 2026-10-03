#include "cffi_array_type.hpp"
#include "cffi_owned_array.hpp"
#include "cffi_span.hpp"

#include <cstdint>

using namespace godot;

namespace cffi {

CFFIArrayType::CFFIArrayType() {
	kind = TypeKind::Array;
}
CFFIArrayType::CFFIArrayType(Ref<CFFIType> element_type, int64_t length)
	: CFFIType(String("%s[%d]") % Array::make(element_type->get_name(), length), create_array_type(element_type->get_ffi_type(), length))
	, element_type(element_type)
	, length(length)
{
	kind = TypeKind::Array;
}

Ref<CFFIType> CFFIArrayType::get_element_type() const {
	return element_type;
}

int64_t CFFIArrayType::get_length() const {
	return length;
}

bool CFFIArrayType::data_to_variant(const uint8_t *ptr, Variant& r_variant) const {
	r_variant = memnew(CFFIOwnedArray(element_type, length, ptr));
	return true;
}

bool CFFIArrayType::variant_to_data(const Variant& value, uint8_t *buffer) const {
	int64_t buffer_size_bytes = get_size() ?: INT64_MAX;
	switch (value.get_type()) {
		case Variant::Type::PACKED_BYTE_ARRAY: {
			PackedByteArray array = value;
			size_t size_bytes = MIN(buffer_size_bytes, array.size());
			memcpy(buffer, array.ptr(), size_bytes);
			return true;
		}
		case Variant::Type::PACKED_INT32_ARRAY: {
			if (element_type->get_size() != sizeof(int32_t)) {
				break;
			}
			PackedInt32Array array = value;
			size_t size_bytes = MIN(buffer_size_bytes, array.size() * sizeof(int32_t));
			memcpy(buffer, array.ptr(), size_bytes);
			return true;
		}
		case Variant::Type::PACKED_INT64_ARRAY: {
			if (element_type->get_size() != sizeof(int64_t)) {
				break;
			}
			PackedInt64Array array = value;
			size_t size_bytes = MIN(buffer_size_bytes, array.size() * sizeof(int64_t));
			memcpy(buffer, array.ptr(), size_bytes);
			return true;
		}
		case Variant::Type::PACKED_FLOAT32_ARRAY: {
			if (element_type->get_size() != sizeof(float)) {
				break;
			}
			PackedFloat32Array array = value;
			size_t size_bytes = MIN(buffer_size_bytes, array.size() * sizeof(float));
			memcpy(buffer, array.ptr(), size_bytes);
			return true;
		}
		case Variant::Type::PACKED_FLOAT64_ARRAY: {
			if (element_type->get_size() != sizeof(double)) {
				break;
			}
			PackedFloat64Array array = value;
			size_t size_bytes = MIN(buffer_size_bytes, array.size() * sizeof(double));
			memcpy(buffer, array.ptr(), size_bytes);
			return true;
		}
		case Variant::Type::PACKED_VECTOR2_ARRAY: {
			if (element_type->get_size() != sizeof(Vector2)) {
				break;
			}
			PackedVector2Array array = value;
			size_t size_bytes = MIN(buffer_size_bytes, array.size() * sizeof(Vector2));
			memcpy(buffer, array.ptr(), size_bytes);
			return true;
		}
		case Variant::Type::PACKED_VECTOR3_ARRAY: {
			if (element_type->get_size() != sizeof(Vector3)) {
				break;
			}
			PackedVector3Array array = value;
			size_t size_bytes = MIN(buffer_size_bytes, array.size() * sizeof(Vector3));
			memcpy(buffer, array.ptr(), size_bytes);
			return true;
		}
		case Variant::Type::PACKED_VECTOR4_ARRAY: {
			if (element_type->get_size() != sizeof(Vector4)) {
				break;
			}
			PackedVector4Array array = value;
			size_t size_bytes = MIN(buffer_size_bytes, array.size() * sizeof(Vector4));
			memcpy(buffer, array.ptr(), size_bytes);
			return true;
		}
		case Variant::Type::PACKED_COLOR_ARRAY: {
			if (element_type->get_size() != sizeof(Color)) {
				break;
			}
			PackedColorArray array = value;
			size_t size_bytes = MIN(buffer_size_bytes, array.size() * sizeof(Color));
			memcpy(buffer, array.ptr(), size_bytes);
			return true;
		}

		case Variant::Type::OBJECT:
			if (auto span = Object::cast_to<CFFISpan>(value)) {
				size_t size_bytes = MIN(buffer_size_bytes, span->get_size_bytes());
				memcpy(buffer, span->get_data()->address_offset_by(0), size_bytes);
				return true;
			}
			else if (auto pointer = Object::cast_to<CFFIPointer>(value)) {
				ERR_FAIL_COND_V_MSG(length <= 0, false, "Only sized arrays can be initialized with pointer");
				memcpy(buffer, pointer->address_offset_by(0), buffer_size_bytes);
				return true;
			}
			break;

		default:
			break;
	}
	ERR_FAIL_V_EDMSG(false, String("Invalid type \"%s\" for array type \"%s\"") % Array::make(value.get_type_name(value.get_type()), name));
}

Ref<CFFIArrayType> CFFIArrayType::from(const Variant& type, int64_t length) {
	ERR_FAIL_COND_V(length < 0, nullptr);
	Ref<CFFIType> element_type = CFFIType::from_variant(type, nullptr);
	if (element_type.is_valid()) {
		return memnew(CFFIArrayType(element_type, length));
	}
	else {
		return nullptr;
	}
}

void CFFIArrayType::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_element_type"), &CFFIArrayType::get_element_type);
	ClassDB::bind_method(D_METHOD("get_length"), &CFFIArrayType::get_length);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("from", "type", "length"), &CFFIArrayType::from);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "element_type", PROPERTY_HINT_NONE, CFFIType::get_class_static(), PROPERTY_USAGE_NONE), "", "get_element_type");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "length", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "", "get_length");
}

ffi_type CFFIArrayType::create_array_type(const ffi_type& element_type, int64_t length) {
	static ffi_type* null_ffi_handle = nullptr;

	ffi_type type = {};
	type.alignment = element_type.alignment;
	type.size = element_type.size * length;
	type.type = FFI_TYPE_STRUCT;
	type.elements = &null_ffi_handle;
	return type;
}

}
