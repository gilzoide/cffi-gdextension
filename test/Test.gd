extends Node

static var NativePlugin = preload("res://native_plugin/native_plugin.ffilibrary").open()
static var get_answer = NativePlugin.get_function("get_answer", "int")
static var double_int = NativePlugin.get_function("double_int", "int", ["int32_t"])
static var double_float = NativePlugin.get_function("double_float", "float", ["float"])


# Called when the node enters the scene tree for the first time.
func _ready():
	# print(get_answer.invoke())
	print(double_int.invoke([13]))
	pass
