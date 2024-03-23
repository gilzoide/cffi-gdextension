#ifndef __CFFI_VALUE_HPP__
#define __CFFI_VALUE_HPP__

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

namespace cffi {

class CFFIPointer;
class CFFIType;

class CFFIValue : public RefCounted {
	GDCLASS(CFFIValue, RefCounted);
public:
	CFFIValue();
	CFFIValue(Ref<CFFIType> type, bool initialize_with_zeros = true);
	~CFFIValue();

	Ref<CFFIType> get_type() const;
	Variant get_value() const;
	bool set_value(const Variant& value) const;
	Ref<CFFIPointer> get_address() const;

	Ref<CFFIPointer> address_of(const StringName& field) const;

protected:
	static void _bind_methods();
	bool _get(const StringName& property_name, Variant& r_value) const;
	bool _set(const StringName& property_name, const Variant& value);
	String _to_string() const;

	Ref<CFFIType> type;
	uint8_t *address;
};

}

#endif  // __CFFI_VALUE_HPP__