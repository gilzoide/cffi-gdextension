env = SConscript("lib/godot-cpp/SConstruct").Clone()

# Add support for generating compilation database files
env.Tool("compilation_db")
compiledb = env.CompilationDatabase("compile_commands.json")
env.Alias("compiledb", compiledb)

# Compile with debugging symbols
if ARGUMENTS.get("debugging_symbols") == 'true':
    if "-O2" in env["CCFLAGS"]:
        env["CCFLAGS"].remove("-O2")
    env.Append(CCFLAGS=["-g", "-O0"])

# Setup variant build dir for each setup
def remove_prefix(s, prefix):
    return s[len(prefix):] if s.startswith(prefix) else s

build_dir = "build/{}".format(remove_prefix(env["suffix"], "."))
VariantDir(build_dir, 'src', duplicate=False)

# Run autoconf
if env['platform'] == 'macos':
    env.Append(CFLAGS=["-arch", "x86_64", "-arch", "arm64"])
ffi_autogen = env.Command(f"lib/libffi/configure", [], "cd lib/libffi && ./autogen.sh")
ffi_output = f"{build_dir}/libffi"
ffi_configure = env.Command(f"{ffi_output}/Makefile", ffi_autogen, f"mkdir -p {ffi_output} && cd {ffi_output} && ../../../lib/libffi/configure --disable-shared \"CFLAGS={env['CFLAGS']}\"")
ffi_staticlib = env.Command(f"{ffi_output}/.libs/libffi.a", ffi_configure, f"make -C {ffi_output}")
env.Append(
    CPPPATH=f"{ffi_output}/include",
    LIBS=ffi_staticlib,
)

# Build Lua GDExtension
sources = Glob("src/*.cpp")
library = env.SharedLibrary(
    "addons/cffi/build/libcffi{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)

Default(library)
