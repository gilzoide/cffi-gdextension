extends Label

static var NativePlugin: CFFILibraryHandle = load("res://native_plugin/native_plugin.ffilibrary").open()
static var ExampleStruct = NativePlugin.define_struct("ExampleStruct", {
	"a": "int",
	"b": "int",
})
static var get_answer = NativePlugin.get_function("get_answer", "int")
static var double_int = NativePlugin.get_function("double_int", "int", ["int"])
static var double_float = NativePlugin.get_function("double_float", "float", ["float"])
static var get_message = NativePlugin.get_function("get_message", "const char *")
static var fill_message = NativePlugin.get_function("fill_message", "void", ["char *", "int"])
static var get_example_struct = NativePlugin.get_function("get_example_struct", "ExampleStruct")
static var get_global_example_struct = NativePlugin.get_function("get_global_example_struct", "ExampleStruct*")
static var get_global_example_struct_ptr = NativePlugin.get_function("get_global_example_struct_ptr", "void", ["ExampleStruct**"])
static var example_struct_get_a = NativePlugin.get_function("example_struct_get_a", "int", ["ExampleStruct"])
static var example_struct_pointer_get_a = NativePlugin.get_function("example_struct_pointer_get_a", "int", ["const ExampleStruct *"])
static var str_length = NativePlugin.get_function("str_length", "int", ["const char *", "int"])

static var global_int = NativePlugin.get_global("global_int", "int")


func _ready():
	print(get_answer.invoke())
	print(double_int.invoke(31))
	print(double_float.invoke(2.51))
	var message: CFFIPointer = get_message.invoke()
	var message_str = message.get_string_from_utf8()
	print(message_str)
	text = message_str
	
	var example_struct: CFFIOwnedValue = get_example_struct.invoke()
	var new_example_struct: CFFIOwnedValue = ExampleStruct.alloc()
	printt(example_struct, new_example_struct)
	new_example_struct.set_value(example_struct)
	printt(example_struct, new_example_struct)

	assert(example_struct.a == example_struct_get_a.invoke(example_struct))
	assert(example_struct.a == example_struct_pointer_get_a.invoke(example_struct))

	var buffer = PackedByteArray()
	buffer.resize(128)
	fill_message.invoke(buffer, buffer.size())
	print(buffer.get_string_from_utf8())

	var hello_length = str_length.invoke("Hello beautiful people!", 100)
	print(hello_length)
	
	var charp = CFFI.find_type("char")
	var hi_msg = charp.alloc_array(3, false)
	hi_msg.set_value(ord('h'), 0)
	hi_msg.set_value(ord('i'), 1)
	hi_msg.set_value(0, 2)
	printt(hi_msg.get_string_from_ascii())
	
	var global_example_struct_ptr: CFFIPointer = get_global_example_struct.invoke()
	assert(global_example_struct_ptr != null)
	
	var example_struct_ptr = NativePlugin["ExampleStruct*"].alloc()
	get_global_example_struct_ptr.invoke(example_struct_ptr)
	assert(example_struct_ptr.get_value().get_address() == global_example_struct_ptr.get_address())
	
	# Global variable
	assert(global_int != null)
	assert(global_int.get_value() == 0)
	global_int.set_value(42)
	assert(NativePlugin.get_global("global_int", "int").get_value() == 42)
