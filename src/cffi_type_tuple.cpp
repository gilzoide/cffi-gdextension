#include "cffi_type_tuple.hpp"

namespace cffi {

FFITypeTuple::FFITypeTuple() {}
FFITypeTuple::FFITypeTuple(FFITypeVector&& fields) : fields(fields) {}
FFITypeTuple::FFITypeTuple(const FFITypeVector& fields) : fields(fields) {}

const FFITypeVector& FFITypeTuple::get_fields() const {
	return fields;
}

const int FFITypeTuple::size() const {
	return fields.size();
}

String FFITypeTuple::to_string() const {
	PackedStringArray types_str;
	for (int i = 0; i < fields.size(); i++) {
		types_str.append(fields[i]->get_name());
	}
	return String("(%s)") % String(", ").join(types_str);
}

ffi_type **FFITypeTuple::get_argument_types() {
	if (ffi_handle.size() != fields.size()) {
		ffi_handle.resize(fields.size());
		for (int i = 0; i < fields.size(); i++) {
			ffi_handle[i] = &fields[i]->get_ffi_type();
		}
	}
	return ffi_handle.ptr();
}

FFITypeTuple FFITypeTuple::from_array(const Array& array) {
	FFITypeVector fields;
	fields.resize(array.size());
	for (int64_t i = 0; i < array.size(); i++) {
		auto& var = array[i];
		FFIType *field_type = FFIType::from_variant(var);
		ERR_FAIL_COND_V_EDMSG(field_type == nullptr, FFITypeTuple(), String("Invalid type: %s") % var.stringify());
		fields[i].reference_ptr(field_type);
	}
	return FFITypeTuple(std::move(fields));
}

}