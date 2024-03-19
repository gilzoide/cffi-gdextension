@tool
extends EditorExportPlugin


func _export_file(path, type, features):
	if path.begins_with("res://addons/cffi/plugin"):
		skip()
	elif path.ends_with(".ffilibrary"):
		var libpath = load(path).find_library_path(features)
		if libpath:
			add_shared_object(libpath, features, "")


func _get_name():
	return "CFFILibrary"
