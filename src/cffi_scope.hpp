#ifndef __CFFI_SCOPE_HPP__
#define __CFFI_SCOPE_HPP__

#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

namespace cffi {

class CFFIStructType;
class CFFIType;

/**
 * CFFI type scopes, so that different libraries can have distinct struct definitions with the same name.
 */
class CFFIScope : public RefCounted {
	GDCLASS(CFFIScope, RefCounted);
public:
	/**
	 * Finds a type by name.
	 *
	 * If the base type name cannot be found in this scope, the global scope (the `CFFI` singleton) is searched.
	 * `name` is parsed using `CFFITypeParser`, so pointers and CV qualifiers are supported.
	 *
	 * This method is called when accessing properties by name, so that `some_cffi_scope["int"]` returns the CFFIType for `int`.
	 */
	Ref<CFFIType> find_type(const String& name) const;
	/**
	 * Defines a new struct type.
	 *
	 * `name` cannot have any pointer levels.
	 * If a struct with the same name is found in this scope, `null` is returned and an error printed.
	 *
	 * @see CFFIStructType::from_dictionary
	 */
	Ref<CFFIStructType> define_struct(const String& name, const Dictionary& fields);

protected:
	bool _get(const StringName& property_name, Variant& r_value) const;
	static void _bind_methods();

	HashMap<String, Ref<CFFIType>> defined_types;
	static const HashMap<String, Ref<CFFIType>>& get_globally_defined_types();
};

}

#endif  // __CFFI_SCOPE_HPP__
