#include "cffi_library_handle.hpp"
#include "cffi_function.hpp"
#include "cffi_type.hpp"

#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/project_settings.hpp>

#ifndef SHLIBSUFFIX
	#define SHLIBSUFFIX ".so"
#endif

#ifdef _WIN32
	#include <windows.h>
	#include <system_error>
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
	auto str = std::system_category().message(GetLastError());
	return String::utf8(str.c_str(), str.size());
#else
	return dlerror();
#endif
}

static int os_close_library(void *handle) {
#ifdef _WIN32
	return FreeLibrary((HMODULE) handle);
#else
	return dlclose(handle);
#endif
}

static void *os_get_symbol(void *handle, const char *symbol) {
#ifdef _WIN32
	return (void *) GetProcAddress((HMODULE) handle, symbol);
#else
	return dlsym(handle, symbol);
#endif
}

namespace cffi {

CFFILibraryHandle::CFFILibraryHandle() {}
CFFILibraryHandle::CFFILibraryHandle(void *library_handle) : library_handle(library_handle) {}

CFFILibraryHandle::~CFFILibraryHandle() {
	if (library_handle) {
		os_close_library(library_handle);
	}
}

Ref<CFFILibraryHandle> CFFILibraryHandle::open(const String& name) {
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
	return memnew(CFFILibraryHandle(library_handle));
}

Ref<CFFIFunction> CFFILibraryHandle::get_function(const String& name, const Variant& return_type_var, const Array& argument_types_arr, bool is_variadic) const {
	void *address = os_get_symbol(library_handle, name.ascii().get_data());
	ERR_FAIL_COND_V_MSG(address == nullptr, nullptr, os_get_last_error());

	auto return_type = CFFIType::from_variant(return_type_var, this);
	ERR_FAIL_COND_V_MSG(return_type == nullptr, nullptr, String("Could not find return type: %s") % return_type_var.stringify());

	CFFITypeTuple argument_types = CFFITypeTuple::from_array(argument_types_arr, this);
	ERR_FAIL_COND_V_MSG(argument_types.size() != argument_types_arr.size(), nullptr, "Invalid argument types");
	return memnew(CFFIFunction(name, address, return_type, argument_types, is_variadic));
}

void CFFILibraryHandle::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_function", "name", "return_type", "argument_types", "is_variadic"), &CFFILibraryHandle::get_function, DEFVAL(Array()), DEFVAL(false));
}

String CFFILibraryHandle::_to_string() const {
	return String("[%s:0x%x]") % Array::make(get_class(), (uint64_t) library_handle);
}

}