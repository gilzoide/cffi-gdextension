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
	assert(struct_int_data.offset_of("data") == struct_int_data.size)
	
	var struct_long_data = CFFI.define_struct("test_struct_with_zero_sized_array_long", {
		"length": "int32_t",
		"data": "int64_t[0]",  # bigger alignment than length in most archs
	})
	assert(struct_long_data.offset_of("length") == 0)
	assert(struct_long_data.offset_of("data") >= CFFI["int32_t"].size)
	assert(struct_long_data.offset_of("data") == struct_long_data.size)
	
	var struct_instance = struct_long_data.alloc()
	assert(struct_instance.data is CFFIPointer)
	return true


func test_struct_with_sized_array() -> bool:
	var struct = CFFI.define_struct("test_struct_with_sized_array", {
		"chars": "char[5]",
		"i": "int",
	})
	assert(struct.offset_of("i") == 8)
	
	var struct_instance = struct.alloc()
	assert(struct_instance.chars is CFFISpan)
	assert(struct_instance.chars.length == 5)
	return true
