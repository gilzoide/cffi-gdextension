#include <ffi.h>
#include <godot_cpp/godot.hpp>

#include "Cffi.hpp"
#include "CffiFunction.hpp"
#include "CffiLibrary.hpp"
#include "CffiType.hpp"

using namespace cffi;
using namespace godot;

static void initialize(ModuleInitializationLevel level) {
	if (level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ClassDB::register_abstract_class<CffiLibrary>();
	ClassDB::register_abstract_class<CffiFunction>();
	ClassDB::register_abstract_class<CffiType>();
	ClassDB::register_abstract_class<Cffi>();
	Cffi::get_or_create_singleton();
}

static void deinitialize(ModuleInitializationLevel level) {
	if (level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	Cffi::delete_singleton();
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
