#ifndef __CFFI_STRUCT_TYPE_HPP__
#define __CFFI_STRUCT_TYPE_HPP__

#include "cffi_type.hpp"
#include "cffi_type_tuple.hpp"

#include <godot_cpp/templates/hash_map.hpp>

using namespace godot;

namespace cffi {

class CFFIStructType : public CFFITypeTuple, public CFFIType {
	GDCLASS(CFFIStructType, CFFIType);
public:
	CFFIStructType();

	static Ref<CFFIStructType> from_dictionary(const String& name, const Dictionary& fields);

protected:
	static void _bind_methods();
	String _to_string() const override;

	HashMap<StringName, int> field_map;

private:
	CFFIStructType(const String& name, CFFITypeTuple&& fields, HashMap<StringName, int>&& field_map);

	ffi_type create_struct_type();
};

}

#endif  // __CFFI_STRUCT_TYPE_HPP__