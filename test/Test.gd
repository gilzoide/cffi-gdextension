extends Node


# Called when the node enters the scene tree for the first time.
func _ready():
	var lib = preload("res://native_plugin/native_plugin.ffilibrary").open()
	print(lib)
	var f = lib.define_function("get_answer", "int", [])
	print(f)
