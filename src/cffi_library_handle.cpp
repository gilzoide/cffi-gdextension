#include "cffi_library_handle.hpp"
#include "cffi_type_tuple.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/project_settings.hpp>

#ifndef SHLIBSUFFIX
	#define SHLIBSUFFIX ".so"
#endif

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <dlfcn.h>
#endif

static void *os_open_library(const char *path) {
#ifdef _WIN32
	return LoadLibrary(path);
#else
	return dlopen(path, RTLD_LAZY);
#endif
}

static String os_get_last_error() {
#ifdef _WIN32
	return GetLastError();
#else
	return dlerror();
#endif
}

static int os_close_library(void *handle) {
#ifdef _WIN32
	return FreeLibrary(handle);
#else
	return dlclose(handle);
#endif
}

static void *os_get_symbol(void *handle, const char *symbol) {
#ifdef _WIN32
	return (void *) GetProcAddress(handle, symbol);
#else
	return dlsym(handle, symbol);
#endif
}

namespace cffi {

FFILibraryHandle::FFILibraryHandle() {}
FFILibraryHandle::FFILibraryHandle(void *library_handle) : library_handle(library_handle) {}

FFILibraryHandle::~FFILibraryHandle() {
	if (library_handle) {
		os_close_library(library_handle);
	}
}

FFILibraryHandle *FFILibraryHandle::open(const String& name) {
	String path;
	if (name.begins_with("res://")) {
		if (OS::get_singleton()->has_feature("editor")) {
			path = ProjectSettings::get_singleton()->globalize_path(name);
		}
		else {
			path = OS::get_singleton()->get_executable_path().path_join(name.get_basename());
		}
	}
	else {
		path = name;
	}
	if (!name.is_empty() && !name.contains(".")) {
		path += SHLIBSUFFIX;
	}
	void *library_handle = os_open_library(path.is_empty() ? nullptr : path.utf8().get_data());
	ERR_FAIL_COND_V_MSG(library_handle == nullptr, nullptr, os_get_last_error());
	return memnew(FFILibraryHandle(library_handle));
}

Ref<FFIFunction> FFILibraryHandle::get_function(const String& name, const Variant& return_type_var, const Array& argument_types_arr, bool is_variadic) {
	void *address = os_get_symbol(library_handle, name.ascii().get_data());
	ERR_FAIL_COND_V_MSG(address == nullptr, nullptr, os_get_last_error());

	auto return_type = FFIType::from_variant(return_type_var);
	ERR_FAIL_COND_V_MSG(return_type == nullptr, nullptr, String("Could not find return type: %s") % return_type_var.stringify());

	FFITypeTuple argument_types = FFITypeTuple::from_array(argument_types_arr);
	ERR_FAIL_COND_V_MSG(argument_types.size() != argument_types_arr.size(), nullptr, "Invalid argument types");
	return memnew(FFIFunction(name, address, return_type, argument_types, is_variadic));
}

void FFILibraryHandle::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_function", "name", "return_type", "argument_types", "is_variadic"), &FFILibraryHandle::get_function, DEFVAL(Array()), DEFVAL(false));
}

String FFILibraryHandle::_to_string() const {
	return String("[%s:0x%x]") % Array::make(get_class(), (uint64_t) library_handle);
}

}