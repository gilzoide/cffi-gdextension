#ifndef __CFFI_HPP__
#define __CFFI_HPP__

#include <ffi.h>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

namespace cffi {

class CFFILibraryHandle;
class CFFIType;

class CFFI : public Object {
	GDCLASS(CFFI, Object);
public:
	CFFI();

	Ref<CFFIType> get_type(const String& name) const;

	Ref<CFFILibraryHandle> open(const String& name) const;

	static CFFI *get_singleton();
	static CFFI *get_or_create_singleton();
	static void delete_singleton();

protected:
	static void _bind_methods();

	bool _get(const StringName& property_name, Variant& r_value) const;

	HashMap<String, Ref<CFFIType>> builtin_types;

private:
	static CFFI *instance;
};

}

#endif  // __CFFI_HPP__
