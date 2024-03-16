extends Resource
class_name CFFILibrary

@export var libraries: Dictionary


func find_library_path(tags := PackedStringArray()) -> String:
	if tags.is_empty():
		return _find_library_path(func(tag): return OS.has_feature(tag))
	else:
		return _find_library_path(func(tag): return tags.has(tag))


func open(tags := PackedStringArray()) -> CFFILibraryHandle:
	var library_path = find_library_path(tags)
	return CFFI.open(library_path)


func _find_library_path(has_tag: Callable) -> String:
	var best_matching_tags := PackedStringArray()
	var best_library_path := ""
	for key in libraries:
		var lib_tags: PackedStringArray = (key as String).split(".")
		var all_tags_met = true
		for tag in lib_tags:
			if not has_tag.call(tag.strip_edges()):
				all_tags_met = false
				break
		if all_tags_met and lib_tags.size() > best_matching_tags.size():
			best_library_path = libraries[key]
			best_matching_tags = lib_tags
	if not best_library_path.is_empty() and best_library_path.is_relative_path():
		best_library_path = resource_path.get_base_dir().path_join(best_library_path)
	return best_library_path
