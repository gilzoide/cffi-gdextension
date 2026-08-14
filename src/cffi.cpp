#include "cffi.hpp"
#include "cffi_callable_function.hpp"
#include "cffi_library_handle.hpp"
#include "cffi_pointer.hpp"
#include "cffi_struct_type.hpp"
#include "cffi_type.hpp"

#include <godot_cpp/classes/engine.hpp>

namespace cffi {

CFFI::CFFI() {
	scope.instantiate();
	scope->setup_builtin_types();
}

Ref<CFFILibraryHandle> CFFI::open(const String& name) const {
	return CFFILibraryHandle::open(name);
}

PackedByteArray CFFI::null_terminated_ascii_buffer(const String& str) {
	PackedByteArray buffer = str.to_ascii_buffer();
	buffer.append(0);
	return buffer;
}

PackedByteArray CFFI::null_terminated_utf8_buffer(const String& str) {
	PackedByteArray buffer = str.to_utf8_buffer();
	buffer.append(0);
	return buffer;
}

PackedByteArray CFFI::null_terminated_utf16_buffer(const String& str) {
	PackedByteArray buffer = str.to_utf16_buffer();
	buffer.append(0);
	buffer.append(0);
	return buffer;
}

PackedByteArray CFFI::null_terminated_utf32_buffer(const String& str) {
	PackedByteArray buffer = str.to_utf32_buffer();
	for (int i = 0; i < sizeof(char32_t); i++) {
		buffer.append(0);
	}
	return buffer;
}

PackedByteArray CFFI::null_terminated_wchar_buffer(const String& str) {
	PackedByteArray buffer = str.to_wchar_buffer();
	for (int i = 0; i < sizeof(wchar_t); i++) {
		buffer.append(0);
	}
	return buffer;
}

Ref<CFFIPointer> CFFI::memcpy(Ref<CFFIPointer> dest, Ref<CFFIPointer> src, int64_t size_bytes) {
	ERR_FAIL_COND_V(dest.is_null(), nullptr);
	ERR_FAIL_COND_V(src.is_null(), nullptr);
	::memcpy(dest->address_offset_by(0), src->address_offset_by(0), size_bytes);
	return dest;
}

Ref<CFFIPointer> CFFI::memmove(Ref<CFFIPointer> dest, Ref<CFFIPointer> src, int64_t size_bytes) {
	ERR_FAIL_COND_V(dest.is_null(), nullptr);
	ERR_FAIL_COND_V(src.is_null(), nullptr);
	::memmove(dest->address_offset_by(0), src->address_offset_by(0), size_bytes);
	return dest;
}

Ref<CFFIPointer> CFFI::memset(Ref<CFFIPointer> dest, int byte_value, int64_t size_bytes) {
	ERR_FAIL_COND_V(dest.is_null(), nullptr);
	::memset(dest->address_offset_by(0), byte_value, size_bytes);
	return dest;
}

int CFFI::memcmp(Ref<CFFIPointer> s1, Ref<CFFIPointer> s2, int64_t size_bytes) {
	if (s1.is_null() || s2.is_null()) {
		return s1.is_null() != s2.is_null();
	}
	return ::memcmp(s1->address_offset_by(0), s2->address_offset_by(0), size_bytes);
}

bool CFFI::memequal(Ref<CFFIPointer> s1, Ref<CFFIPointer> s2, int64_t size_bytes) {
	return memcmp(s1, s2, size_bytes) == 0;
}

Ref<CFFIPointer> CFFI::get_pointer(const Variant& value) {
	switch (value.get_type()) {
		case Variant::Type::NIL: {
			return nullptr;
		}
		case Variant::Type::STRING: {
			String s = value;
			return memnew(CFFIPointer(scope->defined_types["char32_t"], (uint8_t *) s.ptr()));
		}
		case Variant::Type::PACKED_BYTE_ARRAY: {
			PackedByteArray a = value;
			return memnew(CFFIPointer(scope->defined_types["uint8_t"], (uint8_t *) a.ptr()));
		}
		case Variant::Type::PACKED_INT32_ARRAY: {
			PackedInt32Array a = value;
			return memnew(CFFIPointer(scope->defined_types["int32_t"], (uint8_t *) a.ptr()));
		}
		case Variant::Type::PACKED_INT64_ARRAY: {
			PackedInt64Array a = value;
			return memnew(CFFIPointer(scope->defined_types["int64_t"], (uint8_t *) a.ptr()));
		}
		case Variant::Type::PACKED_FLOAT32_ARRAY: {
			PackedFloat32Array a = value;
			return memnew(CFFIPointer(scope->defined_types["float"], (uint8_t *) a.ptr()));
		}
		case Variant::Type::PACKED_FLOAT64_ARRAY: {
			PackedFloat64Array a = value;
			return memnew(CFFIPointer(scope->defined_types["double"], (uint8_t *) a.ptr()));
		}
		case Variant::Type::PACKED_VECTOR2_ARRAY: {
			PackedVector2Array a = value;
			return memnew(CFFIPointer(scope->defined_types["real_t"], (uint8_t *) a.ptr()));
		}
		case Variant::Type::PACKED_VECTOR3_ARRAY: {
			PackedVector3Array a = value;
			return memnew(CFFIPointer(scope->defined_types["real_t"], (uint8_t *) a.ptr()));
		}
		case Variant::Type::PACKED_VECTOR4_ARRAY: {
			PackedVector4Array a = value;
			return memnew(CFFIPointer(scope->defined_types["real_t"], (uint8_t *) a.ptr()));
		}
		case Variant::Type::PACKED_COLOR_ARRAY: {
			PackedColorArray a = value;
			return memnew(CFFIPointer(scope->defined_types["float"], (uint8_t *) a.ptr()));
		}

		case Variant::Type::OBJECT:
			if (auto pointer_value = Object::cast_to<CFFIPointer>(value)) {
				return pointer_value;
			}
			break;

		default:
			break;
	}
	ERR_FAIL_V_MSG(nullptr, "Cannot extract pointer from " + Variant::get_type_name(value.get_type()));
}

Ref<CFFIScope> CFFI::get_scope() const {
	return scope;
}

Ref<CFFIType> CFFI::find_type(const String& name) const {
	return scope->find_type(name);
}

Ref<CFFIStructType> CFFI::define_struct(const String& name, const Dictionary& fields) {
	return scope->define_struct(name, fields);
}

Ref<CFFICallableFunction> CFFI::create_function(const Callable& callable, const Variant& return_type, const Array& argument_types) {
	return scope->create_function(callable, return_type, argument_types);
}

void CFFI::_bind_methods() {
	ClassDB::bind_method(D_METHOD("find_type", "name"), &CFFI::find_type);
	ClassDB::bind_method(D_METHOD("define_struct", "name", "fields"), &CFFI::define_struct);
	ClassDB::bind_method(D_METHOD("create_function", "callable", "return_type", "argument_types"), &CFFI::create_function, DEFVAL(Array()));

	ClassDB::bind_method(D_METHOD("get_scope"), &CFFI::get_scope);
	ClassDB::bind_method(D_METHOD("open", "name_or_path"), &CFFI::open);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("null_terminated_ascii_buffer", "str"), &CFFI::null_terminated_ascii_buffer);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("null_terminated_utf8_buffer", "str"), &CFFI::null_terminated_utf8_buffer);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("null_terminated_utf16_buffer", "str"), &CFFI::null_terminated_utf16_buffer);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("null_terminated_utf32_buffer", "str"), &CFFI::null_terminated_utf32_buffer);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("null_terminated_wchar_buffer", "str"), &CFFI::null_terminated_wchar_buffer);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("memcpy", "dest", "src", "size_bytes"), &CFFI::memcpy);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("memmove", "dest", "src", "size_bytes"), &CFFI::memmove);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("memset", "dest", "byte_value", "size_bytes"), &CFFI::memset);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("memcmp", "s1", "s2", "size_bytes"), &CFFI::memcmp);
	ClassDB::bind_static_method(CFFI::get_class_static(), D_METHOD("memequal", "s1", "s2", "size_bytes"), &CFFI::memequal);
	ClassDB::bind_method(D_METHOD("get_pointer", "value"), &CFFI::get_pointer);

	ADD_PROPERTY(PropertyInfo(Variant::Type::OBJECT, "scope", PROPERTY_HINT_NONE, CFFIScope::get_class_static(), PROPERTY_USAGE_NONE), "", "get_scope");
}

bool CFFI::_get(const StringName& property_name, Variant& r_value) const {
	return scope->_get(property_name, r_value);
}

CFFI *CFFI::get_singleton() {
	return instance;
}

CFFI *CFFI::get_or_create_singleton() {
	if (!instance) {
		instance = memnew(CFFI);
		Engine::get_singleton()->register_singleton(CFFI::get_class_static(), instance);
	}
	return instance;
}

void CFFI::delete_singleton() {
	if (instance) {
		Engine::get_singleton()->unregister_singleton(CFFI::get_class_static());
		memdelete(instance);
	}
}

CFFI *CFFI::instance;

}
