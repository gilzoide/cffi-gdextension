#include "cffi_owned_array.hpp"

namespace cffi {

CFFIOwnedArray::CFFIOwnedArray() {}
CFFIOwnedArray::CFFIOwnedArray(Ref<CFFIType> type, int64_t length, bool initialize_with_zeros)
	: CFFISpan(type, (uint8_t *) memalloc(length * type->get_size()), length)
{
	ERR_FAIL_COND_EDMSG(address == nullptr, String("Could not allocate %d bytes for %s") % Array::make(length * type->get_size(), type->get_name()));
	if (initialize_with_zeros) {
		memset(address, 0, get_size_bytes());
	}
}
CFFIOwnedArray::CFFIOwnedArray(Ref<CFFIType> type, int64_t length, const uint8_t *existing_data)
	: CFFISpan(type, (uint8_t *) memalloc(length * type->get_size()), length)
{
	ERR_FAIL_COND_EDMSG(address == nullptr, String("Could not allocate %d bytes for %s") % Array::make(length * type->get_size(), type->get_name()));
	if (existing_data) {
		memcpy(address, existing_data, get_size_bytes());
	}
}

CFFIOwnedArray::~CFFIOwnedArray() {
	if (address) {
		memfree(address);
	}
}

void CFFIOwnedArray::_bind_methods() {
}

}