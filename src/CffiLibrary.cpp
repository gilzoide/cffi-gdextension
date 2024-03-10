#include "CffiLibrary.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>

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

CffiLibrary::CffiLibrary() {}
CffiLibrary::CffiLibrary(void *library_handle) : library_handle(library_handle) {}

CffiLibrary::~CffiLibrary() {
    if (library_handle) {
        os_close_library(library_handle);
    }
}

CffiLibrary *CffiLibrary::open(const String& name) {
    void *library_handle = os_open_library(name.is_empty() ? nullptr : name.utf8().get_data());
    ERR_FAIL_COND_V_MSG(library_handle == nullptr, nullptr, os_get_last_error());
    return memnew(CffiLibrary(library_handle));
}

Ref<CffiFunction> CffiLibrary::define_function(const String& name, Ref<CffiType> return_type, TypedArray<CffiType> argument_types, bool is_variadic) {
    void *address = os_get_symbol(library_handle, name.ascii().get_data());
    ERR_FAIL_COND_V_MSG(address == nullptr, nullptr, os_get_last_error());
    return memnew(CffiFunction(name, address, return_type, argument_types, is_variadic));
}

void CffiLibrary::_bind_methods() {
    ClassDB::bind_method(D_METHOD("define_function", "name", "return_type", "argument_types", "is_variadic"), &CffiLibrary::define_function, DEFVAL(true));
}

String CffiLibrary::_to_string() const {
    return String("[%s:0x%x]") % Array::make(get_class(), (uint64_t) library_handle);
}

}