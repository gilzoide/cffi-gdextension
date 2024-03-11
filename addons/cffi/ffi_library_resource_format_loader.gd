@tool
extends ResourceFormatLoader
class_name FFILibraryResourceFormatLoader

const LIBRARIES_SECTION = "libraries"


func _get_recognized_extensions():
	return ["ffilibrary"]


func _handles_type(type):
	return type == "FFILibrary"


func _load(path, original_path, use_sub_threads, cache_mode):
	var config = ConfigFile.new()
	var res = config.load(original_path)
	if res != OK:
		return res
	if not config.has_section(LIBRARIES_SECTION):
		return ERR_INVALID_DATA
	var libraries = {}
	for key in config.get_section_keys(LIBRARIES_SECTION):
		libraries[key] = config.get_value(LIBRARIES_SECTION, key)
	var resource = FFILibrary.new()
	resource.libraries = libraries
	return resource
