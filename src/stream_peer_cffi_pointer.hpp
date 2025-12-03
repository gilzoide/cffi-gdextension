#ifndef __CFFI_POINTER_STREAM_PEER_HPP__
#define __CFFI_POINTER_STREAM_PEER_HPP__

#include <godot_cpp/classes/stream_peer_extension.hpp>

#include "cffi_pointer.hpp"

using namespace godot;

namespace cffi {

class StreamPeerCFFIPointer : public StreamPeerExtension {
	GDCLASS(StreamPeerCFFIPointer, StreamPeerExtension)
public:
	Error _get_data(uint8_t *r_buffer, int32_t r_bytes, int32_t *r_received) override;
	Error _get_partial_data(uint8_t *r_buffer, int32_t r_bytes, int32_t *r_received) override;
	Error _put_data(const uint8_t *p_data, int32_t p_bytes, int32_t *r_sent) override;
	Error _put_partial_data(const uint8_t *p_data, int32_t p_bytes, int32_t *r_sent) override;
	int32_t _get_available_bytes() const override;

	void set_pointer(Ref<CFFIPointer> pointer);
	Ref<CFFIPointer> get_pointer() const;

	void set_size_in_bytes(int size);
	int get_size_in_bytes() const;

	void set_size(int size);
	int get_size() const;

	void seek(int p_pos);
	int get_position() const;

	void clear();

protected:
	static void _bind_methods();

private:
	Ref<CFFIPointer> pointer = nullptr;
	int size_in_bytes = 0;
	int cursor = 0;
};

}

#endif  // __CFFI_POINTER_STREAM_PEER_HPP__