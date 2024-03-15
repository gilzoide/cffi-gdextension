#include "cffi_pointer.hpp"
#include "cffi_pointer_type.hpp"

namespace cffi {

class FFIPointerType;

FFIPointer::FFIPointer() {}
FFIPointer::FFIPointer(Ref<FFIType> type, uint8_t *address) : element_type(type), address(address) {}

uint8_t *FFIPointer::address_offset_by(int offset) const {
	return address + element_type->get_size() * offset;
}

Ref<FFIPointer> FFIPointer::offset_by(int offset) const {
	return memnew(FFIPointer(element_type, address_offset_by(offset)));
}

Variant FFIPointer::get_value(int index) const {
	Variant value;
	if (element_type->get_return_value(address_offset_by(index), value)) {
		return value;
	}
	else {
		return Variant();
	}
}

bool FFIPointer::set_value(const Variant& value, int index) const {
	return element_type->serialize_value_into(value, address_offset_by(index));
}

void FFIPointer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("offset_by", "offset"), &FFIPointer::offset_by);
	ClassDB::bind_method(D_METHOD("get_value", "index"), &FFIPointer::get_value, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("set_value", "value", "index"), &FFIPointer::set_value, DEFVAL(0));
}

String FFIPointer::_to_string() const {
	return String("[%s:(%s*) 0x%x]") % Array::make(get_class_static(), element_type->get_name(), (uint64_t) address);
}

}