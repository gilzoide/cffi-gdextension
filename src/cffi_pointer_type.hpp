#ifndef __CFFI_POINTER_TYPE_HPP__
#define __CFFI_POINTER_TYPE_HPP__

#include "cffi_type.hpp"

using namespace godot;

namespace cffi {

class FFIPointerType : public FFIType {
	GDCLASS(FFIPointerType, FFIType);
public:
	FFIPointerType();
	FFIPointerType(Ref<FFIType> element_type);

protected:
	Ref<FFIType> element_type;
};

}

#endif  // __CFFI_POINTER_TYPE_HPP__