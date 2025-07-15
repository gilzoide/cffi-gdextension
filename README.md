# CFFI GDExtension
[![Build](https://github.com/gilzoide/cffi-gdextension/actions/workflows/.build.yml/badge.svg?branch=main)](https://github.com/gilzoide/cffi-gdextension/actions/workflows/.build.yml)

WIP [libffi](https://github.com/libffi/libffi) bindings for [Godot 4.1+](https://godotengine.org/).

> **Warning**
>
> Misuse may easily crash the Godot editor and built games/apps, use with caution!


## Features
- Supports Windows, Linux, macOS, iOS and Android platforms, including running in the Godot editor
- Custom ".ffilibrary" file format, which are INI files with a `[libraries]` section that defines paths to native libraries separately per platform/architecture, using the exact [same format as ".gdextension" files]((https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/gdextension_cpp_example.html#using-the-gdextension-module)).
  They are imported as [CFFILibrary](addons/cffi/cffi_library.gd) resources.
  + TODO: support the `[dependencies]` section
- Editor export plugin that bundles the correct native library in builds, based on the configurations from ".ffilibrary" files
- Supports built-in types like `int` and `float`, pointer types like `const char *` and struct types
  + TODO: arrays, union, enums, function pointers
- Scoped type definitions, so that different types can be defined with the same name in different libraries.
- Instantiate FFI types using `CFFIType.alloc`.
  The returned `CFFIOwnedValue` is RefCounted and releases the memory automatically whenever it gets destroyed.
- Get/set struct fields by name from `CFFIPointer`s
- Construct `String`s and `PackedByteArray`s from `CFFIPointer`s with a single method call
- Use Dictionaries as literal struct values, for example when calling a function


## Basic usage
1. Enable the `cffi` editor plugin in `Project -> Project Settings... -> Plugins` tab.
2. Build your native DLLs and add them to your project
3. Create a ".ffilibrary" file and define the paths of the native libraries per platform/architecture inside the `[libraries]` section, exactly like you would do for [.gdextension files](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/gdextension_cpp_example.html#using-the-gdextension-module).
   This file will be imported as a [CFFILibrary](addons/cffi/cffi_library.gd) Resource.
4. Call `CFFILibrary.open()` to open a `CFFILibraryHandle`
5. Get native functions using `CFFILibraryHandle.get_function` and invoke them with `invoke` or `invokev`.
6. Mess with native data using `CFFIPointer`s, hopefully not crashing your app
7. Enjoy 🍾


## Show me some code!
```c
// native_plugin.c
typedef struct ExampleStruct {
    int a;
    int b;
} ExampleStruct;

const char *get_message() {
    return "Hello world!";
}

int get_a(ExampleStruct s) {
    return s.a;
}

int my_strlen(const char *s) {
    int len = 0;
    while (*s) {
        len++;
        s++;
    }
    return len;
}
```

```ini
# native_plugin.ffilibrary
[libraries]
windows.x86_64 = "res://native_plugin_windows_x86_64.dll"
windows.x86 = "res://native_plugin_windows_x86.dll"
linux.x86_64 = "res://native_plugin_linux_x86_64.so"
linux.x86 = "res://native_plugin_linux_x86.so"
macos = "res://native_plugin_macos.dylib"
```

```gdscript
# script.gd
extends Node

# Using ".ffilibrary" files is the preferred way of bundling FFI libraries in projects.
# You can also use `CFFI.open` to load libraries by name or path directly instead.
static var native_plugin_dll = load("res://native_plugin.ffilibrary").open()
static var ExampleStruct = native_plugin_dll.define_struct("ExampleStruct", {
    "a": "int",
    "b": "int",
})
static var get_message = native_plugin_dll.get_function("get_message", "const char *")
static var get_a = native_plugin_dll.get_function("get_a", "int", ["ExampleStruct"])
static var my_strlen = native_plugin_dll.get_function("my_strlen", "int", ["const char *"])


func _ready():
    var message_ptr: CFFIPointer = get_message.invoke()
    var message = message_ptr.get_string_from_utf8()
    print(message)  # "Hello world!"

    var example_struct = ExampleStruct.alloc()
    example_struct.a = 42
    var a = get_a.invoke(example_struct)
    assert(a == example_struct.a)
    assert(a == 42)

    # Strings are passed as null-terminated buffers to pointer arguments
    var message_length = my_strlen.invoke("Hello World!")
    assert(message_length == 12)
```

There is also an example [native_plugin.ffilibrary](test/native_plugin/native_plugin.ffilibrary) in the "test/native_plugin" folder showing how to setup a native library resource.
The script [Test.gd](test/Test.gd) shows how to use the FFI to call the native library functions. Refer to [plugin.c](test/native_plugin/plugin.c) for the native function implementations in this sample.


## TODO
- Accept a pointer when expecting a value, just dereference it automagically
  + Accept a value when expecting pointer? Could be useful in function calls, but not when setting struct fields
- Support defining and using unions
- Support FFI array types, both fixed and variable length
- Support function pointers
- Support variadic native functions (the `variadic` flag is there, but the `ffi_cif` is not being prep'd correctly)
- Support `[dependencies]` section in ".ffilibrary" files
- Define enum values?
  Enum constants can easily be defined as constants in GDScript directly, so this isn't a requirement.
  Would be nice for self documenting function arguments/return types, though.


## Open-source libraries
CFFI GDExtension uses the following open-source libraries:
- [libffi](https://github.com/libffi/libffi) ([License](https://github.com/libffi/libffi/blob/master/LICENSE))
- [godot-cpp](https://github.com/godotengine/godot-cpp) ([License](https://github.com/godotengine/godot-cpp/blob/master/LICENSE.md))