from os import path


TARGET_TRIPLE_MAP = {
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


def exists(env):
    return True


def generate(env):
    ffi_autogen = env.Command(
        "lib/libffi/configure",
        [],
        "cd lib/libffi && ./autogen.sh"
    )
    env.NoCache(ffi_autogen)

    build_dir = env['build_dir']

    # macOS universal special case: build x86_64 and arm64 separately, then `lipo` them together
    if env["platform"] == "macos" and env["arch"] == "universal":
        env_x86_64 = env.Clone()
        _remove_options(env_x86_64["CCFLAGS"], "-arch", "x86_64", "-arch", "arm64")
        _remove_options(env_x86_64["LINKFLAGS"], "-arch", "x86_64", "-arch", "arm64")
        env_x86_64.Append(
            CCFLAGS=["-arch", "x86_64"],
            LINKFLAGS=["-arch", "x86_64"],
        )
        libffi_x86_64, _ = _generate_libffi(env_x86_64, f"{build_dir}/libffi/x86_64", ffi_autogen, TARGET_TRIPLE_MAP["macos"]["x86_64"])
        
        env_arm64 = env.Clone()
        _remove_options(env_arm64["CCFLAGS"], "-arch", "x86_64", "-arch", "arm64")
        _remove_options(env_arm64["LINKFLAGS"], "-arch", "x86_64", "-arch", "arm64")
        env_arm64.Append(
            CCFLAGS=["-arch", "arm64"],
            LINKFLAGS=["-arch", "arm64"],
        )
        libffi_arm64, ffi_h_arm64 = _generate_libffi(env_arm64, f"{build_dir}/libffi/arm64", ffi_autogen, TARGET_TRIPLE_MAP["macos"]["arm64"])

        libffi = _lipo(
            env,
            target=f"{build_dir}/libffi/libffi.a",
            source=[libffi_x86_64, libffi_arm64],
        )
        ffi_h = ffi_h_arm64
    else:
        libffi, ffi_h = _generate_libffi(env, f"{build_dir}/libffi", ffi_autogen, TARGET_TRIPLE_MAP[env["platform"]][env["arch"]])

    env.Append(CPPPATH=path.dirname(str(ffi_h[0])))
    env.Append(LIBS=libffi)


def _generate_libffi(env, build_dir, ffi_autogen, target_triple):
    ffi_configure = env.Command(
        f"{build_dir}/Makefile",
        ffi_autogen,
        (
            f"mkdir -p {build_dir}"
            f" && cd {build_dir}"
            f" && {path.relpath('lib/libffi/configure', build_dir)} --disable-shared"
                f" --host {target_triple}"
                f" \"CC={env["CC"]}\""
                f" \"CXX={env["CXX"]}\""
                f" \"CFLAGS={env["CCFLAGS"]}\""
        )
    )
    ffi_h = env.SideEffect(f"{build_dir}/include/ffi.h", ffi_configure)
    ffi_staticlib = env.Command(
        f"{build_dir}/.libs/libffi.a",
        ffi_configure,
        f"make -C {build_dir}"
    )
    env.NoCache([ffi_configure, ffi_staticlib])
    return ffi_staticlib, ffi_h


def _lipo(env, target, source):
    return env.Command(
        target,
        source,
        action=f"lipo $SOURCES -create -output $TARGET",
    )


def _remove_options(lst, *options) -> bool:
    removed_something = False
    for opt in options:
        if opt in lst:
            lst.remove(opt)
            removed_something = True
    return removed_something