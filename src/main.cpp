#include <ffi.h>
#include <godot_cpp/godot.hpp>

#include "cffi.hpp"
#include "cffi_function.hpp"
#include "cffi_library_handle.hpp"
#include "cffi_pointer.hpp"
#include "cffi_pointer_type.hpp"
#include "cffi_scope.hpp"
#include "cffi_struct_type.hpp"
#include "cffi_type.hpp"
#include "cffi_value.hpp"

using namespace cffi;
using namespace godot;

static void initialize(ModuleInitializationLevel level) {
	if (level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ClassDB::register_abstract_class<CFFIScope>();
	ClassDB::register_abstract_class<CFFILibraryHandle>();
	ClassDB::register_abstract_class<CFFIFunction>();
	ClassDB::register_abstract_class<CFFIType>();
	ClassDB::register_abstract_class<CFFIPointerType>();
	ClassDB::register_abstract_class<CFFIStructType>();
	ClassDB::register_abstract_class<CFFIPointer>();
	ClassDB::register_abstract_class<CFFIValue>();
	ClassDB::register_abstract_class<CFFI>();
	CFFIScope::register_builtin_types();
	CFFI::get_or_create_singleton();
}

static void deinitialize(ModuleInitializationLevel level) {
	if (level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	CFFIScope::clear_builtin_types();
	CFFI::delete_singleton();
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
