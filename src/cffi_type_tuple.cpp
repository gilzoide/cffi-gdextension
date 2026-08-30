#include "cffi_scope.hpp"
#include "cffi_type_tuple.hpp"
#include "cffi_type.hpp"

namespace cffi {

CFFITypeTuple::CFFITypeTuple() {}
CFFITypeTuple::CFFITypeTuple(CFFITypeVector&& fields) : fields(fields) {}
CFFITypeTuple::CFFITypeTuple(const CFFITypeVector& fields) : fields(fields) {}

CFFITypeVector& CFFITypeTuple::get_fields() {
	return fields;
}

const CFFITypeVector& CFFITypeTuple::get_fields() const {
	return fields;
}

uint32_t CFFITypeTuple::size() const {
	return fields.size();
}

String CFFITypeTuple::to_string() const {
	PackedStringArray types_str;
	for (int i = 0; i < fields.size(); ++i) {
		types_str.append(fields[i]->get_name());
	}
	return String("(%s)") % String(", ").join(types_str);
}

ffi_type **CFFITypeTuple::get_element_types() {
	if (ffi_fields.is_empty()) {
		bool needs_padding;
		for (int i = 0; i < fields.size(); ++i) {
			ffi_type& t = fields[i]->get_ffi_type();
			// filter-out zero-sized arrays
			if (t.size != 0 || t.type != FFI_TYPE_STRUCT) {
				ffi_fields.push_back(&t);
			}
		}
		ffi_fields.push_back(nullptr);
	}
	return ffi_fields.ptr();
}

CFFITypeTuple CFFITypeTuple::from_array(const Array& array, const CFFIScope *type_scope) {
	CFFITypeVector fields;
	fields.resize(array.size());
	for (int64_t i = 0; i < array.size(); i++) {
		auto& var = array[i];
		Ref<CFFIType> field_type = CFFIType::from_variant(var, type_scope);
		ERR_FAIL_COND_V_EDMSG(field_type == nullptr, CFFITypeTuple(), String("Invalid type: %s") % var.stringify());
		fields[i] = field_type;
	}
	return CFFITypeTuple(std::move(fields));
}

}