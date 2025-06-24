from os import path


env = SConscript("lib/godot-cpp/SConstruct").Clone()

# Add support for generating compilation database files
env.Tool("compilation_db")
compiledb = env.CompilationDatabase("compile_commands.json")
env.Alias("compiledb", compiledb)

# Setup variant build dir for each setup
def remove_prefix(s, prefix):
    return s[len(prefix):] if s.startswith(prefix) else s

build_dir = "build/{}".format(remove_prefix(env["suffix"], "."))
env.VariantDir(build_dir, 'src', duplicate=False)

# libffi stuff
target_triple_map = {
    "windows": {
        "x86_64": "x86_64-windows",
        "x86_32": "i686-windows",
    },
    "linux": {
        "x86_64": "x86_64-linux",
        "x86_32": "i686-linux",
        "arm64": "arm64-linux",
    },
    "macos": {
        "universal": "arm64-macos",
        "arm64": "arm64-macos",
        "x86_64": "x86_64-macos",
    },
    "ios": {
        "universal": "arm64-ios",
        "arm64": "arm64-ios",
    },
    "android": {
        "arm64": "arm64-android",
        "arm32": "armv7-android",
        "x86_64": "x86_64-android",
        "x86_32": "i686-android",
    },
}

CC = env["CXX"].replace("clang++", "clang").replace("g++", "gcc").replace("c++", "cc")
CXX = env["CXX"]
CFLAGS = env["CCFLAGS"]
target_triple = target_triple_map[env["platform"]][env["arch"]]
ffi_output = f"{build_dir}/libffi"
ffi_autogen = env.Command(
    f"lib/libffi/configure",
    [],
    "cd lib/libffi && ./autogen.sh"
)
ffi_configure = env.Command(
    f"{ffi_output}/Makefile",
    ffi_autogen,
    (
        f"mkdir -p {ffi_output}"
        f" && cd {ffi_output}"
        f" && ../../../lib/libffi/configure --disable-shared"
            f" --host {target_triple}"
            f" \"CC={CC}\""
            f" \"CXX={CXX}\""
            f" \"CFLAGS={CFLAGS}\""
    )
)
ffi_staticlib = env.Command(
    f"{ffi_output}/.libs/libffi.a",
    ffi_configure,
    f"make -C {ffi_output}"
)
ffi_h = env.SideEffect(f"{ffi_output}/include/ffi.h", ffi_configure)
env.Append(
    CPPPATH=f"{ffi_output}/include",
    LIBS=[ffi_staticlib],
)
# avoid caching single files in CI
env.NoCache([ffi_autogen, ffi_configure, ffi_staticlib])

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
    "addons/cffi/build/libcffi{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)
env.Depends(library, ffi_h)

Default(library)
