#include "cffi_value_tuple.hpp"
#include "cffi_type_tuple.hpp"
#include "cffi_type.hpp"

#include <godot_cpp/classes/stream_peer_buffer.hpp>

using namespace godot;

namespace cffi {

CFFIValueTuple::CFFIValueTuple() {}
CFFIValueTuple::CFFIValueTuple(const PackedByteArray& bytes, const PackedInt32Array& byte_offsets) : bytes(bytes) {
	value_addresses.resize(byte_offsets.size());
	const uint8_t *ptr = this->bytes.ptr();
	for (int i = 0; i < byte_offsets.size(); i++) {
		value_addresses[i] = (void *) (ptr + byte_offsets[i]);
	}
}

uint32_t CFFIValueTuple::size() const {
	return value_addresses.size();
}

void * const *CFFIValueTuple::get_value_addresses() const {
	return value_addresses.ptr();
}

CFFIValueTuple CFFIValueTuple::from_varargs(const CFFITypeTuple& type_tuple, const Variant **args, GDExtensionInt arg_count) {
	ERR_FAIL_COND_V_EDMSG(type_tuple.size() != arg_count, CFFIValueTuple(), "Array size doesn't match types size");

	const CFFITypeVector& fields = type_tuple.get_fields();
	PackedByteArray buffer;
	PackedInt32Array offsets;
	for (GDExtensionInt i = 0; i < arg_count; i++) {
		offsets.append(buffer.size());
		ERR_FAIL_COND_V(!fields[i]->variant_to_data(*args[i], buffer), CFFIValueTuple());
	}
	// fixup any string pointers that could have been repositioned by a realloc in buffer
	for (GDExtensionInt i = 0; i < arg_count - 1; i++) {
		if (args[i]->get_type() == Variant::Type::STRING || args[i]->get_type() == Variant::Type::STRING_NAME) {
			void **ptr = (void **) (buffer.ptr() + offsets[i]);
			*ptr = ptr + 1;
		}
	}
	return CFFIValueTuple(buffer, offsets);
}

CFFIValueTuple CFFIValueTuple::from_array(const CFFITypeTuple& type_tuple, const Array& array) {
	ERR_FAIL_COND_V_EDMSG(type_tuple.size() != array.size(), CFFIValueTuple(), "Array size doesn't match types size");

	const CFFITypeVector& fields = type_tuple.get_fields();
	PackedByteArray buffer;
	PackedInt32Array offsets;
	for (int64_t i = 0; i < array.size(); i++) {
		offsets.append(buffer.size());
		ERR_FAIL_COND_V(!fields[i]->variant_to_data(array[i], buffer), CFFIValueTuple());
	}
	// fixup any string pointers that could have been repositioned by a realloc in buffer
	for (GDExtensionInt i = 0; i < array.size() - 1; i++) {
		if (array[i].get_type() == Variant::Type::STRING || array[i].get_type() == Variant::Type::STRING_NAME) {
			void **ptr = (void **) (buffer.ptr() + offsets[i]);
			*ptr = ptr + 1;
		}
	}
	return CFFIValueTuple(buffer, offsets);
}

}