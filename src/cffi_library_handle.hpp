#ifndef __CFFI_LIBRARY_HPP__
#define __CFFI_LIBRARY_HPP__

#include "cffi_scope.hpp"

using namespace godot;

namespace cffi {

class CFFIFunction;
class CFFIPointer;

/**
 * Native dynamic library handle, where native functions live.
 */
class CFFILibraryHandle : public CFFIScope {
	GDCLASS(CFFILibraryHandle, CFFIScope);
public:
	/**
	 * Necessary to define a Godot class.
	 * @warning Never use this constructor.
	 */
	CFFILibraryHandle();
	virtual ~CFFILibraryHandle();

	/**
	 * Get the function named `name` from the library.
	 *
	 * `name` should be a symbol exported by the native library.
	 * Users are responsible for passing the correct return type and array of argument types.
	 * @warning Passing the wrong types and calling the returned function may crash your application and/or Godot editor.
	 */
	Ref<CFFIFunction> get_function(const String& name, const Variant& return_type, const Array& argument_types, bool is_variadic = false) const;

	/**
	 * Get a pointer to the global variable named `name` from the library.
	 *
	 * `name` should be a symbol exported by the native library.
	 * Users are responsible for passing the correct type.
	 * @warning Passing the wrong type and dereferencing the returned pointer may crash your application and/or Godot editor.
	 */
	Ref<CFFIPointer> get_global(const String& name, const Variant& type);

	/**
	 * Opens a dynamic library by its name or path.
	 *
	 * `name` may be the name of the library, for example "cffi", or a file path like "res://some_dir/cffi.so".
	 * Paths starting with `res://` reference files inside the project in editor.
	 * In built games, the directory part is ignored and the library is searched for in the executable path.
	 *
	 * @return The open library handle or `null` if the library could not be opened.
	 */
	static Ref<CFFILibraryHandle> open(const String& name);

protected:
	static void _bind_methods();
	String _to_string() const;

	void *library_handle;

private:
	CFFILibraryHandle(void *library_handle);
};

}

#endif  // __CFFI_LIBRARY_HPP__