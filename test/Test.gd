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
static var get_example_struct = NativePlugin.get_function("get_example_struct", "ExampleStruct")


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
