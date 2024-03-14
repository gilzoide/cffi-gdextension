#include "cffi_value_tuple.hpp"
#include "cffi_type_tuple.hpp"
#include "cffi_type.hpp"

#include <godot_cpp/classes/stream_peer_buffer.hpp>

using namespace godot;

namespace cffi {

FFIValueTuple::FFIValueTuple() {}
FFIValueTuple::FFIValueTuple(const PackedByteArray& bytes, const PackedInt32Array& byteOffsets) : bytes(bytes) {
	value_addresses.resize(byteOffsets.size());
	const uint8_t *ptr = this->bytes.ptr();
	for (int i = 0; i < byteOffsets.size(); i++) {
		value_addresses[i] = (void *) (ptr + byteOffsets[i]);
	}
}

uint32_t FFIValueTuple::size() const {
	return value_addresses.size();
}

void * const *FFIValueTuple::get_value_addresses() const {
	return value_addresses.ptr();
}

FFIValueTuple FFIValueTuple::from_varargs(const FFITypeTuple& type_tuple, const Variant **args, GDExtensionInt arg_count) {
	ERR_FAIL_COND_V_EDMSG(type_tuple.size() != arg_count, FFIValueTuple(), "Array size doesn't match types size");

	auto& fields = type_tuple.get_fields();
	Ref<StreamPeerBuffer> buffer = memnew(StreamPeerBuffer);
	PackedInt32Array offsets;
	for (GDExtensionInt i = 0; i < arg_count; i++) {
		offsets.append(buffer->get_size());
		ERR_FAIL_COND_V(!fields[i]->serialize_value_into(*args[i], buffer), FFIValueTuple());
	}
	return FFIValueTuple(buffer->get_data_array(), offsets);
}

FFIValueTuple FFIValueTuple::from_array(const FFITypeTuple& type_tuple, const Array& array) {
	ERR_FAIL_COND_V_EDMSG(type_tuple.size() != array.size(), FFIValueTuple(), "Array size doesn't match types size");

	auto& fields = type_tuple.get_fields();
	Ref<StreamPeerBuffer> buffer = memnew(StreamPeerBuffer);
	PackedInt32Array offsets;
	for (int64_t i = 0; i < array.size(); i++) {
		offsets.append(buffer->get_size());
		ERR_FAIL_COND_V(!fields[i]->serialize_value_into(array[i], buffer), FFIValueTuple());
	}
	return FFIValueTuple(buffer->get_data_array(), offsets);
}

}