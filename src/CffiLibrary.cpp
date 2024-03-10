#include "CffiLibrary.hpp"

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

static String os_get_open_library_error() {
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
    void *library_handle = os_open_library(name.utf8().get_data());
    ERR_FAIL_COND_V_MSG(library_handle == nullptr, nullptr, os_get_open_library_error());
    return memnew(CffiLibrary(library_handle));
}

void CffiLibrary::_bind_methods() {}

}