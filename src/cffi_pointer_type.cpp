#include "cffi_pointer_type.hpp"

namespace cffi {

FFIPointerType::FFIPointerType() {}
FFIPointerType::FFIPointerType(Ref<FFIType> element_type)
	: FFIType(element_type->get_name() + String("*"), ffi_type_pointer)
	, element_type(element_type)
	{}

}