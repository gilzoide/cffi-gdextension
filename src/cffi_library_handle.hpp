#ifndef __CFFI_LIBRARY_HPP__
#define __CFFI_LIBRARY_HPP__

#include "cffi_scope.hpp"

using namespace godot;

namespace cffi {

class CFFIFunction;

class CFFILibraryHandle : public CFFIScope {
	GDCLASS(CFFILibraryHandle, CFFIScope);
public:
	CFFILibraryHandle();
	CFFILibraryHandle(void *library_handle);
	~CFFILibraryHandle();

	static Ref<CFFILibraryHandle> open(const String& name);

	Ref<CFFIFunction> get_function(const String& name, const Variant& return_type, const Array& argument_types, bool is_variadic = false);

protected:
	static void _bind_methods();
	String _to_string() const;

private:
	void *library_handle;
};

}

#endif  // __CFFI_LIBRARY_HPP__