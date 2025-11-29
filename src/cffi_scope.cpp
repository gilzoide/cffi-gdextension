#include "cffi.hpp"
#include "cffi_callable_function.hpp"
#include "cffi_pointer_type.hpp"
#include "cffi_struct_type.hpp"
#include "cffi_scope.hpp"
#include "cffi_type_parser.hpp"

namespace cffi {

Ref<CFFIType> CFFIScope::find_type(const String& name) const {
	CFFITypeParser parser;
	ERR_FAIL_COND_V_EDMSG(!parser.parse(name), nullptr, String("Invalid type name \"%s\"") % name);

	// pointer types
	auto& base_name = parser.get_base_name();
	auto base_type = defined_types.getptr(base_name) ?: get_globally_defined_types().getptr(base_name);
	ERR_FAIL_COND_V_EDMSG(base_type == nullptr, nullptr, String("Unknown type name: \"%s\"") % name);
	auto type = *base_type;
	for (int i = 0; i < parser.get_pointer_level(); i++) {
		type = Ref<CFFIType>(memnew(CFFIPointerType(type)));
	}
	return type;
}

Ref<CFFIStructType> CFFIScope::define_struct(const String& name, const Dictionary& fields) {
	CFFITypeParser parser;
	ERR_FAIL_COND_V_EDMSG(
		!parser.parse(name) || parser.get_pointer_level() != 0,
		nullptr,
		String("Invalid type name: \"%s\"") % name
	);

	auto& base_name = parser.get_base_name();
	ERR_FAIL_COND_V_EDMSG(
		defined_types.has(base_name) || get_globally_defined_types().has(base_name),
		nullptr,
		String("Type name already defined: \"%s\"") % name
	);

	auto type = CFFIStructType::from_dictionary(base_name, fields, this);
	if (type.is_valid()) {
		defined_types[base_name] = type;
	}
	return type;
}

Ref<CFFICallableFunction> CFFIScope::create_function(const Callable& callable, const Variant& return_type_var, const Array& argument_types_arr) {
	ERR_FAIL_COND_V_MSG(!callable.is_valid(), nullptr, "Callable is invalid");

	Ref<CFFIType> return_type = CFFIType::from_variant(return_type_var, this);
	ERR_FAIL_COND_V_MSG(return_type == nullptr, nullptr, String("Could not find return type: %s") % return_type_var.stringify());

	CFFITypeTuple argument_types = CFFITypeTuple::from_array(argument_types_arr, this);
	ERR_FAIL_COND_V_MSG(argument_types.size() != argument_types_arr.size(), nullptr, "Invalid argument types");

	return memnew(CFFICallableFunction(callable, return_type, argument_types));
}

bool CFFIScope::_get(const StringName& property_name, Variant& r_value) const {
	auto type = find_type(property_name);
	if (type.is_valid()) {
		r_value = type;
		return true;
	}
	else {
		return false;
	}
}

void CFFIScope::_bind_methods() {
	ClassDB::bind_method(D_METHOD("find_type", "name"), &CFFIScope::find_type);
	ClassDB::bind_method(D_METHOD("define_struct", "name", "fields"), &CFFIScope::define_struct);
	ClassDB::bind_method(D_METHOD("create_function", "callable", "return_type", "argument_types"), &CFFIScope::create_function, DEFVAL(Array()));
}

const HashMap<String, Ref<CFFIType>>& CFFIScope::get_globally_defined_types() {
	return CFFI::get_singleton()->defined_types;
}

}
