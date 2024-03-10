#include "cffi_library.hpp"

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

FFILibrary::FFILibrary() {}
FFILibrary::FFILibrary(void *library_handle) : library_handle(library_handle) {}

FFILibrary::~FFILibrary() {
    if (library_handle) {
        os_close_library(library_handle);
    }
}

FFILibrary *FFILibrary::open(const String& name) {
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
    if (!name.contains(".")) {
        path += SHLIBSUFFIX;
    }
    void *library_handle = os_open_library(path.is_empty() ? nullptr : path.utf8().get_data());
    ERR_FAIL_COND_V_MSG(library_handle == nullptr, nullptr, os_get_last_error());
    return memnew(FFILibrary(library_handle));
}

Ref<FFIFunction> FFILibrary::define_function(const String& name, Ref<FFIType> return_type, TypedArray<FFIType> argument_types, bool is_variadic) {
    void *address = os_get_symbol(library_handle, name.ascii().get_data());
    ERR_FAIL_COND_V_MSG(address == nullptr, nullptr, os_get_last_error());
    return memnew(FFIFunction(name, address, return_type, argument_types, is_variadic));
}

void FFILibrary::_bind_methods() {
    ClassDB::bind_method(D_METHOD("define_function", "name", "return_type", "argument_types", "is_variadic"), &FFILibrary::define_function, DEFVAL(true));
}

String FFILibrary::_to_string() const {
    return String("[%s:0x%x]") % Array::make(get_class(), (uint64_t) library_handle);
}

}