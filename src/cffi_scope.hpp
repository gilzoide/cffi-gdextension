#ifndef __CFFI_SCOPE_HPP__
#define __CFFI_SCOPE_HPP__

#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

namespace cffi {

class CFFIStructType;
class CFFIType;

class CFFIScope : public RefCounted {
	GDCLASS(CFFIScope, RefCounted);
public:
	Ref<CFFIType> find_type(const String& name) const;
	Ref<CFFIStructType> define_struct(const String& name, const Dictionary& fields);

protected:
	bool _get(const StringName& property_name, Variant& r_value) const;
	static void _bind_methods();

	HashMap<String, Ref<CFFIType>> defined_types;
	static const HashMap<String, Ref<CFFIType>>& get_globally_defined_types();
};

}

#endif  // __CFFI_SCOPE_HPP__
