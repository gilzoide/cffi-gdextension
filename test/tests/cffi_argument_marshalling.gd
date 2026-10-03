extends Node

## Regression tests for converting GDScript values into native call arguments.
##
## The second test used to take the whole process down rather than fail: a
## rejected argument left an empty value tuple, and `ffi_call` dereferenced its
## null argument vector.


static func _check(condition: bool, message: String) -> bool:
	if not condition:
		printerr("      ", message)
	return condition


## libc, for the tests that need a real native function with a pointer parameter.
##
## Returns null on a platform this doesn't know, in which case the caller skips.
static func _libc() -> CFFILibraryHandle:
	match OS.get_name():
		"macOS", "iOS":
			return CFFI.open("/usr/lib/libSystem.B.dylib")
		"Linux", "FreeBSD", "NetBSD", "OpenBSD", "BSD":
			return CFFI.open("libc.so.6")
		"Windows":
			return CFFI.open("msvcrt.dll")
	return null


## An argument that cannot be converted must fail the call and return null.
## `invokev` is the Array-taking entry point, which reports and returns instead of
## raising a script error at the call site.
func test_unconvertible_argument_errors_instead_of_crashing() -> bool:
	var libc := _libc()
	if libc == null:
		print("      (no libc on this platform, skipping)")
		return true
	var memset = libc.get_function("memset", "void *", ["void *", "int", "size_t"])
	# A float is not a pointer.
	var result = memset.invokev([1.5, 0, 4])
	return _check(result == null, "an unconvertible argument should return null")


## A span is a pointer plus a length, and the length is irrelevant to a `T *`
## parameter, so passing the base address is the right thing to do. Array-typed
## struct fields already accept spans; pointer parameters now do too.
##
## This is the vararg (`invoke`) path as well: before the crash fix the rejected
## argument produced an empty tuple and ffi_call dereferenced a null vector, so
## this test took the process down instead of failing.
func test_span_is_accepted_where_a_pointer_is_declared() -> bool:
	var libc := _libc()
	if libc == null:
		print("      (no libc on this platform, skipping)")
		return true
	var memset = libc.get_function("memset", "void *", ["void *", "int", "size_t"])
	var data = CFFI["uint8_t"].alloc_array(8)
	memset.invoke(data, 0xAB, 8)
	var ok := true
	ok = _check(data.get_value(0) == 0xAB,
		"memset through a span should fill the buffer, got %d" % data.get_value(0)) and ok
	ok = _check(data.get_value(7) == 0xAB,
		"the whole span should be filled, got %d" % data.get_value(7)) and ok
	return ok
