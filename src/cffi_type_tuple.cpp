#include "cffi_type_tuple.hpp"
#include "cffi_type.hpp"

namespace cffi {

CFFITypeTuple::CFFITypeTuple() {}
CFFITypeTuple::CFFITypeTuple(CFFITypeVector&& fields) : fields(fields) {}
CFFITypeTuple::CFFITypeTuple(const CFFITypeVector& fields) : fields(fields) {}

const CFFITypeVector& CFFITypeTuple::get_fields() const {
	return fields;
}

uint32_t CFFITypeTuple::size() const {
	return fields.size();
}

String CFFITypeTuple::to_string() const {
	PackedStringArray types_str;
	for (int i = 0; i < fields.size(); i++) {
		types_str.append(fields[i]->get_name());
	}
	return String("(%s)") % String(", ").join(types_str);
}

ffi_type **CFFITypeTuple::get_argument_types() {
	if (ffi_handle.is_empty()) {
		ffi_handle.resize(fields.size() + 1);
		for (int i = 0; i < fields.size(); i++) {
			ffi_handle[i] = &fields[i]->get_ffi_type();
		}
		ffi_handle[fields.size()] = nullptr;
	}
	return ffi_handle.ptr();
}

CFFITypeTuple CFFITypeTuple::from_array(const Array& array) {
	CFFITypeVector fields;
	fields.resize(array.size());
	for (int64_t i = 0; i < array.size(); i++) {
		auto& var = array[i];
		Ref<CFFIType> field_type = CFFIType::from_variant(var);
		ERR_FAIL_COND_V_EDMSG(field_type == nullptr, CFFITypeTuple(), String("Invalid type: %s") % var.stringify());
		fields[i] = field_type;
	}
	return CFFITypeTuple(std::move(fields));
}

}