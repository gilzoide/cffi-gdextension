#ifndef __CFFI_HPP__
#define __CFFI_HPP__

#include "cffi_scope.hpp"

using namespace godot;

namespace cffi {

class CFFILibraryHandle;
class CFFIStructType;
class CFFIType;

class CFFI : public CFFIScope {
	GDCLASS(CFFI, CFFIScope);
public:
	Ref<CFFILibraryHandle> open(const String& name) const;

	static CFFI *get_singleton();
	static CFFI *get_or_create_singleton();
	static void delete_singleton();

protected:
	static void _bind_methods();

private:
	static CFFI *instance;
};

}

#endif  // __CFFI_HPP__
