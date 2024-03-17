extends Node

static var NativePlugin = preload("res://native_plugin/native_plugin.ffilibrary").open()
static var get_answer = NativePlugin.get_function("get_answer", "int")
static var double_int = NativePlugin.get_function("double_int", "int", ["int"])
static var double_float = NativePlugin.get_function("double_float", "float", ["float"])
static var get_message = NativePlugin.get_function("get_message", "const char *")


# Called when the node enters the scene tree for the first time.
func _ready():
	print(get_answer.invoke())
	print(double_int.invoke(31))
	print(double_float.invoke(2.51))
	var message: CFFIPointer = get_message.invoke()
	print(message.get_string_from_utf8())
	
	var native_int = CFFI["float"].alloc()
	print(native_int)
