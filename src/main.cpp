#include <ffi.h>
#include <godot_cpp/godot.hpp>

#include "cffi.hpp"
#include "cffi_function.hpp"
#include "cffi_library_handle.hpp"
#include "cffi_type.hpp"

using namespace cffi;
using namespace godot;

static void initialize(ModuleInitializationLevel level) {
	if (level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ClassDB::register_abstract_class<FFILibraryHandle>();
	ClassDB::register_abstract_class<FFIFunction>();
	ClassDB::register_abstract_class<FFIType>();
	ClassDB::register_abstract_class<FFI>();
	FFI::get_or_create_singleton();
}

static void deinitialize(ModuleInitializationLevel level) {
	if (level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	FFI::delete_singleton();
}

extern "C" GDExtensionBool cffi_entrypoint(
	const GDExtensionInterfaceGetProcAddress p_getprocaccess,
	GDExtensionClassLibraryPtr p_library,
	GDExtensionInitialization *r_initialization
) {
	GDExtensionBinding::InitObject init_obj(p_getprocaccess, p_library, r_initialization);

	init_obj.register_initializer(&initialize);
	init_obj.register_terminator(&deinitialize);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
