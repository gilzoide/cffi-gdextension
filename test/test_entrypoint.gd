extends SceneTree

const TEST_DIR = "res://tests"

func _process(_delta) -> bool:
	var error_count = 0

	for gdscript in DirAccess.get_files_at(TEST_DIR):
		if not gdscript.ends_with(".gd"):
			continue
		print("> ", gdscript, ":")
		var file_name = str(TEST_DIR, "/", gdscript)
		var obj = load(file_name).new()
		if obj is Node:
			root.add_child(obj)
		for method in obj.get_method_list():
			var method_name = method.name
			if method_name.begins_with("test"):
				# optional per-test setup
				if obj.has_method("_setup"):
					obj._setup()
				# actual test
				if not obj.call(method_name):
					error_count += 1
					printerr("  ! ", method_name)
				else:
					print("  ✓ ", method_name)
		if obj is Node:
			obj.queue_free()
	
	print("\nFailed tests: ", error_count)
	quit(0 if error_count == 0 else -1)
	return true
