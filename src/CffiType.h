#include <ffi.h>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/wrapped.hpp>

using namespace godot;

namespace cffi {

class CffiType : public RefCounted {
    GDCLASS(CffiType, RefCounted);
public:
    CffiType();
    CffiType(ffi_type ffi_type);

    // TODO: get_size, get_alignment
    ffi_type get_ffi_type() const;

protected:
    static void _bind_methods();

    ffi_type ffi_type;
};

}