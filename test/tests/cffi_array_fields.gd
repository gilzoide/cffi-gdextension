extends Node

## Regression tests for array and span handling in structs.
##
## Each test pins down behaviour that was wrong before; all four fail on the
## unpatched `feature/array-types` branch.


static func _check(condition: bool, message: String) -> bool:
	if not condition:
		printerr("      ", message)
	return condition


## A span made from a sized array field must describe the ELEMENT type, so that
## indexing strides by sizeof(element). Spanning the array type instead strides
## by sizeof(T[N]), which reads and writes past the end of the field.
func test_sized_array_field_span_strides_by_element() -> bool:
	var inner = CFFI.define_struct("fix_inner", {
		"a": "int32_t",
		"b": "int32_t",
	})
	var outer = CFFI.define_struct("fix_outer", {
		"n": "uint32_t",
		"ints": "int32_t[3]",
		"items": "fix_inner[2]",
	})
	var ok := true

	var instance = outer.alloc()
	var ints: CFFISpan = instance.ints
	ok = _check(ints is CFFISpan, "ints should be a CFFISpan") and ok
	ok = _check(ints.length == 3, "ints.length should be 3, got %d" % ints.length) and ok
	ok = _check(ints.element_type.size == 4,
		"ints element size should be 4, got %d" % ints.element_type.size) and ok
	ok = _check(ints.size_bytes == 12,
		"ints should span 12 bytes, got %d" % ints.size_bytes) and ok
	ok = _check(ints.get_pointer(1).address - ints.get_pointer(0).address == 4,
		"ints should stride by 4 bytes") and ok

	for i in 3:
		ints.set_value(i, i + 1)
	ok = _check(ints.to_int32_array() == PackedInt32Array([1, 2, 3]),
		"ints should read back as [1, 2, 3], got %s" % [ints.to_int32_array()]) and ok
	# n sits immediately before ints: an over-striding span would have hit it.
	ok = _check(instance.n == 0, "ints must not write outside itself, n = %d" % instance.n) and ok

	var items: CFFISpan = instance.items
	ok = _check(items.element_type.name == "fix_inner",
		"items element type should be fix_inner, got %s" % items.element_type.name) and ok
	ok = _check(items.element_type.size == inner.size, "items element size should be 8") and ok
	ok = _check(items.size_bytes == 16,
		"items should span 16 bytes, got %d" % items.size_bytes) and ok
	ok = _check(items.get_pointer(1).address - items.get_pointer(0).address == 8,
		"items should stride by 8 bytes") and ok
	items.get_pointer(1).get_field("b").set_value(42)
	ok = _check(items.get_pointer(1).b == 42, "items[1] should be addressable through the span") and ok
	return ok


## A zero-sized array field is a flexible array member: its element count lives
## somewhere else, so the field must behave like a `T *`. A pointer typed as
## `T[0]` has size 0 and cannot be indexed or spanned at all.
##
## `storage` here stands in for the buffer a caller would put behind the flexible
## member: the two fields deliberately share an offset.
func test_zero_sized_array_field_is_element_pointer() -> bool:
	var frame = CFFI.define_struct("fix_frame", {
		"a": "int32_t",
		"b": "int32_t",
		"f": "float[0]",
		"storage": "float[4]",
	})
	var ok := true
	ok = _check(frame.offset_of("f") == 8, "f should start at 8, got %d" % frame.offset_of("f")) and ok
	ok = _check(frame.offset_of("f") == frame.offset_of("storage"),
		"f should begin where the caller's storage does") and ok

	var instance = frame.alloc()
	ok = _check(instance.f is CFFIPointer, "f should be a CFFIPointer") and ok
	ok = _check(instance.f.element_type.size == 4,
		"f element size should be 4, got %d" % instance.f.element_type.size) and ok
	ok = _check(instance.f.offset_by(1).address - instance.f.address == 4,
		"f should stride by 4 bytes") and ok

	var span: CFFISpan = CFFISpan.from(instance.f, 4)
	ok = _check(span.size_bytes == 16,
		"a span over f should be 16 bytes, got %d" % span.size_bytes) and ok
	span.set_value(0, 1.5)
	span.set_value(3, 4.5)
	# Read it back through the sibling field, which is the same memory.
	var storage: CFFISpan = instance.storage
	ok = _check(storage.to_float32_array() == PackedFloat32Array([1.5, 0.0, 0.0, 4.5]),
		"writes through f should land in the struct, storage = %s"
		% [storage.to_float32_array()]) and ok
	return ok


## A field declared after a zero-sized array must report its own type. The FFI
## gives a zero-sized field no slot, so it shares the following field's, and
## using that same index into the declaration-ordered field list gave every
## later field the flexible member's type.
func test_field_after_zero_sized_array_keeps_its_type() -> bool:
	var struct = CFFI.define_struct("fix_after_flex", {
		"n": "uint32_t",
		"flex": "float[0]",
		"after": "uint32_t",
	})
	var ok := true
	ok = _check(struct.type_of("after").name == "uint32",
		"after should be uint32, got %s" % struct.type_of("after").name) and ok
	ok = _check(struct.type_of("after").size == 4,
		"after should be 4 bytes, got %d" % struct.type_of("after").size) and ok

	var instance = struct.alloc()
	instance.after = 0x5A5A
	ok = _check(instance.after == 0x5A5A,
		"after should round-trip through the struct, got %d" % instance.after) and ok
	ok = _check(instance.get_field("after") != null, "after should be reachable as a field") and ok
	return ok


## A property access is resolved against the struct's fields first, so asking for
## one of the pointer's own properties must not be reported as an unknown field.
##
## The values here were correct even before the fix; what the fix removes is the
## `ERROR: Unknown field: "address"` that every such access printed.
func test_pointer_own_properties_resolve() -> bool:
	var struct = CFFI.define_struct("fix_props", {"a": "int32_t"})
	var instance = struct.alloc()
	var ok := true
	ok = _check(instance.element_type is CFFIStructType,
		"element_type should resolve to the struct type") and ok
	ok = _check(instance.element_type.name == "fix_props",
		"element_type.name should be fix_props, got %s" % instance.element_type.name) and ok
	ok = _check(instance.address != 0, "address should resolve") and ok
	ok = _check(instance.get_field("address") == null,
		"get_field on a name that is not a field should return null, not error") and ok
	return ok


## `to_byte_array` is the byte view of the span, so it is `length * element_size`
## bytes long. It used to be resized to `length` and copy `length` bytes.
func test_span_to_byte_array_is_byte_sized() -> bool:
	var floats = CFFI["float"].alloc_array(4)
	floats.set_value(3, 2.0)
	var bytes: PackedByteArray = floats.to_byte_array()
	var ok := true
	ok = _check(bytes.size() == 16,
		"4 floats should be 16 bytes, got %d" % bytes.size()) and ok
	ok = _check(bytes.slice(12, 16) != PackedByteArray([0, 0, 0, 0]),
		"the last element should be included in the byte view") and ok

	var empty: CFFISpan = CFFISpan.from(floats.data, 0)
	ok = _check(empty.to_byte_array().size() == 0, "an empty span should give an empty array") and ok
	return ok
