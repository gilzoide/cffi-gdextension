extends Node


static var types = [
	CFFI["bool"],
	CFFI["int"],
	CFFI["long"],
	CFFI["float"],
	CFFI["double"],
]


func test_array_size() -> bool:
	for t in types:
		for i in range(20):
			var array_t = CFFIArrayType.from(t, i)
			assert(array_t.size == i * t.size)
			assert(array_t.alignment == t.alignment)
	return true


func test_struct_with_zero_sized_array() -> bool:
	var struct_int_data = CFFI.define_struct("test_struct_with_zero_sized_array_int", {
		"length": "int32_t",
		"data": "int32_t[0]",  # same alignment of length
	})
	assert(struct_int_data.offset_of("length") == 0)
	assert(struct_int_data.offset_of("data") == CFFI["int32_t"].size)
	
	var struct_long_data = CFFI.define_struct("test_struct_with_zero_sized_array_long", {
		"length": "int32_t",
		"data": "int64_t[0]",  # bigger alignment than length
	})
	assert(struct_long_data.offset_of("length") == 0)
	assert(struct_long_data.offset_of("data") == CFFI["int64_t"].size)
	return true
