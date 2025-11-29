# Changelog
## [Unreleased](https://github.com/gilzoide/cffi-gdextension/compare/0.1.0...HEAD)
### Added
- Support for accessing global variables with `CFFILibraryHandle.get_global`
- `CFFIType.alloc_array` to allocate an array of values
- `CFFIPointer.duplicate_array` to duplicate more than one element from a pointer as an `CFFIOwnedValue`
- `CFFIPointer.get_address` method to get the base address of a pointer.
  Useful to compare pointers by value.
- Support for passing any packed array (other than PackedStringArray) as pointer to native functions.


## [0.1.0](https://github.com/gilzoide/cffi-gdextension/releases/tag/0.1.0)
### Added
- `CFFI` singleton that represents the global type scope and is the entrypoint for opening native libraries
- `CFFILibraryHandle` class representing native libraries, where native functions live
- Custom ".ffilibrary" file format, which are INI files with a `[libraries]` section that defines paths to native libraries separately per platform/architecture, using the exact [same format as ".gdextension" files]((https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/gdextension_cpp_example.html#using-the-gdextension-module)).
  They are imported as [CFFILibrary](addons/cffi/cffi_library.gd) resources.
- Editor export plugin that bundles the correct native library in builds, based on the configurations from ".ffilibrary" files
- Support for built-in types like `int` and `float`, pointer types like `const char *` and struct types
- Instantiate FFI types using `CFFIType.alloc`.
  The returned `CFFIOwnedValue` is RefCounted and releases the memory automatically whenever it gets destroyed.
- Get/set struct fields by name from `CFFIPointer`s
- Construct `String`s and `PackedByteArray`s from `CFFIPointer`s with a single method call
- Use Dictionaries as literal struct values, for example when calling a function