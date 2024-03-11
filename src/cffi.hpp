#ifndef __CFFI_HPP__
#define __CFFI_HPP__

#include <ffi.h>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/dictionary.hpp>

#include "cffi_library_handle.hpp"
#include "cffi_type.hpp"

using namespace godot;

namespace cffi {

class FFI : public RefCounted {
	GDCLASS(FFI, RefCounted);
public:
	FFI();

	FFIType *get_type(const String& name) const;

	FFILibraryHandle *open(const String& name) const;

	static FFI *get_singleton();
	static FFI *get_or_create_singleton();
	static void delete_singleton();

protected:
	static void _bind_methods();

	bool _get(const StringName& property_name, Variant& r_value) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

	Dictionary builtin_types;

private:
	static FFI *instance;
};

}

#endif  // __CFFI_HPP__
