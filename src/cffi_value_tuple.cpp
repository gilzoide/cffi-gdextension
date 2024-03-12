#include "cffi_value_tuple.hpp"
#include "cffi_type_tuple.hpp"

using namespace godot;

namespace cffi {

int FFIValueTuple::size() const {
	return value_addresses.size();
}

void **FFIValueTuple::get_value_addresses() {
	return value_addresses.ptr();
}

FFIValueTuple FFIValueTuple::from_array(const FFITypeTuple& type_tuple, const Array& array) {
	ERR_FAIL_COND_V_EDMSG(type_tuple.size() != array.size(), FFIValueTuple(), "Array size doesn't match types size");

	auto& fields = type_tuple.get_fields();
	Ref<StreamPeerBuffer> buffer = memnew(StreamPeerBuffer);
	for (int64_t i = 0; i < array.size(); i++) {
		ERR_FAIL_COND_V(!fields[i]->serialize_value_into(array[i], buffer), FFIValueTuple());
	}
	FFIValueTuple values;
	values.bytes = buffer->get_data_array();
	values.value_addresses.resize(array.size());
	const uint8_t *ptr = values.bytes.ptr();
	for (int64_t i = 0; i < array.size(); i++) {
		int value_size = fields.size();
		values.value_addresses[i] = (void *) (ptr);
		ptr += value_size;
	}
	return values;
}

}