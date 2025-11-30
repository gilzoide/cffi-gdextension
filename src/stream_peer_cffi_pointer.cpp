#include "stream_peer_cffi_pointer.hpp"

namespace cffi {

Error StreamPeerCFFIPointer::_get_data(uint8_t *r_buffer, int32_t r_bytes, int32_t *r_received) {
	ERR_FAIL_COND_V(pointer.is_null(), ERR_INVALID_DATA);
	*r_received = MIN(r_bytes, _get_available_bytes());
	memcpy(r_buffer, pointer->address_offset_by(cursor), *r_received);
	cursor += *r_received;
	return OK;
}

Error StreamPeerCFFIPointer::_get_partial_data(uint8_t *r_buffer, int32_t r_bytes, int32_t *r_received) {
	return _get_data(r_buffer, r_bytes, r_received);
}

Error StreamPeerCFFIPointer::_put_data(const uint8_t *p_data, int32_t p_bytes, int32_t *r_sent) {
	ERR_FAIL_COND_V(pointer.is_null(), ERR_INVALID_DATA);
	*r_sent = MIN(p_bytes, _get_available_bytes());
	memcpy(pointer->address_offset_by(cursor), p_data, *r_sent);
	cursor += *r_sent;
	return OK;
}

Error StreamPeerCFFIPointer::_put_partial_data(const uint8_t *p_data, int32_t p_bytes, int32_t *r_sent) {
	return _put_data(p_data, p_bytes, r_sent);
}

int32_t StreamPeerCFFIPointer::_get_available_bytes() const {
	return size - cursor;
}

void StreamPeerCFFIPointer::set_pointer(Ref<CFFIPointer> pointer) {
	this->pointer = pointer;
	cursor = 0;
}

Ref<CFFIPointer> StreamPeerCFFIPointer::get_pointer() const {
	return this->pointer;
}

void StreamPeerCFFIPointer::set_size(int size) {
	ERR_FAIL_COND(size < 0);
	this->size = size;
}

int StreamPeerCFFIPointer::get_size() const {
	return size;
}

void StreamPeerCFFIPointer::seek(int p_pos) {
	ERR_FAIL_COND(pointer.is_null());
	ERR_FAIL_COND(p_pos < 0);
	ERR_FAIL_COND(p_pos > size);
	cursor = p_pos;
}

int StreamPeerCFFIPointer::get_position() const {
	return cursor;
}

void StreamPeerCFFIPointer::clear() {
	if (pointer.is_valid()) {
		memset(pointer->address_offset_by(0), 0, size);
	}
	cursor = 0;
}

void StreamPeerCFFIPointer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_pointer"), &StreamPeerCFFIPointer::get_pointer);
	ClassDB::bind_method(D_METHOD("set_pointer", "pointer"), &StreamPeerCFFIPointer::set_pointer);
	ClassDB::bind_method(D_METHOD("get_size"), &StreamPeerCFFIPointer::get_size);
	ClassDB::bind_method(D_METHOD("set_size", "size"), &StreamPeerCFFIPointer::set_size);

	ClassDB::bind_method(D_METHOD("seek", "position"), &StreamPeerCFFIPointer::seek);
	ClassDB::bind_method(D_METHOD("get_position"), &StreamPeerCFFIPointer::get_position);
	ClassDB::bind_method(D_METHOD("clear"), &StreamPeerCFFIPointer::clear);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "pointer", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, CFFIPointer::get_class_static()), "set_pointer", "get_pointer");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");
}

}
