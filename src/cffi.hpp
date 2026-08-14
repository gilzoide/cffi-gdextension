#ifndef __CFFI_HPP__
#define __CFFI_HPP__

#include "cffi_scope.hpp"

using namespace godot;

namespace cffi {

class CFFIPointer;
class CFFILibraryHandle;

/**
 * CFFI singleton, the global FFI type scope and entrypoint for opening libraries.
 */
class CFFI : public Object {
	GDCLASS(CFFI, Object);
public:
	CFFI();
	/**
	 * Opens a native library by its name or path.
	 *
	 * @see CFFILibraryHandle::open
	 */
	Ref<CFFILibraryHandle> open(const String& name_or_path) const;

	static PackedByteArray null_terminated_ascii_buffer(const String& str);
	static PackedByteArray null_terminated_utf8_buffer(const String& str);
	static PackedByteArray null_terminated_utf16_buffer(const String& str);
	static PackedByteArray null_terminated_utf32_buffer(const String& str);
	static PackedByteArray null_terminated_wchar_buffer(const String& str);

	static Ref<CFFIPointer> memcpy(Ref<CFFIPointer> dest, Ref<CFFIPointer> src, int64_t size_bytes);
	static Ref<CFFIPointer> memmove(Ref<CFFIPointer> dest, Ref<CFFIPointer> src, int64_t size_bytes);
	static Ref<CFFIPointer> memset(Ref<CFFIPointer> dest, int byte_value, int64_t size_bytes);
	static int memcmp(Ref<CFFIPointer> s1, Ref<CFFIPointer> s2, int64_t size_bytes);
	static bool memequal(Ref<CFFIPointer> s1, Ref<CFFIPointer> s2, int64_t size_bytes);

	Ref<CFFIPointer> get_pointer(const Variant& string_or_packed_array);

	// from CFFIScope
	Ref<CFFIScope> get_scope() const;
	Ref<CFFIType> find_type(const String& name) const;
	Ref<CFFIStructType> define_struct(const String& name, const Dictionary& fields);
	Ref<CFFICallableFunction> create_function(const Callable& callable, const Variant& return_type, const Array& argument_types);

	// Singleton stuff
	static CFFI *get_singleton();
	static CFFI *get_or_create_singleton();
	static void delete_singleton();

protected:
	static void _bind_methods();
	bool _get(const StringName& property_name, Variant& r_value) const;

	Ref<CFFIScope> scope;

private:
	static CFFI *instance;
};

}

#endif  // __CFFI_HPP__
