from os import path


env = SConscript("lib/godot-cpp/SConstruct").Clone()

# Add support for generating compilation database files
env.Tool("compilation_db")
compiledb = env.CompilationDatabase("compile_commands.json")
env.Alias("compiledb", compiledb)

# Setup variant build dir for each setup
build_dir = f"build/{env["suffix"][1:]}"
env["build_dir"] = build_dir
env.VariantDir(build_dir, "src", duplicate=False)

# libffi stuff
env.Tool("libffi", toolpath=["tools"])

env.Append(CXXFLAGS=[
    # Shared Library suffix, for dlopen/LoadLibrary
    f'-DSHLIBSUFFIX=\\"{env["SHLIBSUFFIX"]}\\"',
    # Avoid C++ not being able to use type `ffi_type` at some points
    "-D_ffi_type=ffi_type",
    # Avoid warnings about __has_trivial_* from godot-cpp templates
    "-Wno-deprecated-builtins",
])

# Build GDExtension
sources = [
    f"{build_dir}/{path.basename(str(cpp))}"
    for cpp in Glob("src/*.cpp")
]
if env["target"] in ["editor", "template_debug"]:
    doc_data = env.GodotCPPDocData("src/generated/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
    sources.append(doc_data)
library = env.SharedLibrary(
    f"addons/cffi/build/libcffi{env["suffix"]}{env["SHLIBSUFFIX"]}",
    source=sources,
)

Default(library)
