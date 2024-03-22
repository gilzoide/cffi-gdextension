#include "cffi.hpp"
#include "cffi_library_handle.hpp"
#include "cffi_pointer_type.hpp"
#include "cffi_struct_type.hpp"
#include "cffi_type_parser.hpp"

#include <godot_cpp/classes/engine.hpp>

namespace cffi {

Ref<CFFILibraryHandle> CFFI::open(const String& name) const {
	return CFFILibraryHandle::open(name);
}

void CFFI::_bind_methods() {
	ClassDB::bind_method(D_METHOD("open", "library_name"), &CFFI::open);
}

CFFI *CFFI::get_singleton() {
	return instance;
}

CFFI *CFFI::get_or_create_singleton() {
	if (!instance) {
		instance = memnew(CFFI);
		Engine::get_singleton()->register_singleton(CFFI::get_class_static(), instance);
	}
	return instance;
}

void CFFI::delete_singleton() {
	if (instance) {
		Engine::get_singleton()->unregister_singleton(CFFI::get_class_static());
		memdelete(instance);
		instance = nullptr;
	}
}

CFFI *CFFI::instance;

}
