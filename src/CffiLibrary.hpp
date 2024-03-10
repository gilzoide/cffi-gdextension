#ifndef __CFFI_LIBRARY_HPP__
#define __CFFI_LIBRARY_HPP__

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

namespace cffi {

class CffiLibrary : public RefCounted {
    GDCLASS(CffiLibrary, RefCounted);
public:
    CffiLibrary();
    CffiLibrary(void *library_handle);
    ~CffiLibrary();

    static CffiLibrary *open(const String& name);

protected:
    static void _bind_methods();

private:
    void *library_handle;
};

}

#endif  // __CFFI_LIBRARY_HPP__