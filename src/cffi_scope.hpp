#ifndef __CFFI_SCOPE_HPP__
#define __CFFI_SCOPE_HPP__

#include <ffi.h>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

namespace cffi {

class CFFIStructType;
class CFFIType;

class CFFIScope : public RefCounted {
	GDCLASS(CFFIScope, RefCounted);
public:
	Ref<CFFIType> get_type(const String& name) const;
	Ref<CFFIStructType> define_struct(const String& name, const Dictionary& fields);

	static void register_builtin_types();
	static void clear_builtin_types();

protected:
	bool _get(const StringName& property_name, Variant& r_value) const;
	static void _bind_methods();

	HashMap<String, Ref<CFFIType>> defined_types;
	static HashMap<String, Ref<CFFIType>> builtin_types;
};

}

#endif  // __CFFI_SCOPE_HPP__
