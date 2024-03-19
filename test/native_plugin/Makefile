LINKFLAGS += -shared

libnative_plugin%.dylib: plugin.c
	$(CC) -o $@ $< $(CFLAGS) $(LINKFLAGS)

libnative_plugin%.so: plugin.c
	$(CC) -o $@ $< $(CFLAGS) $(LINKFLAGS)

macos: CFLAGS += -arch arm64 -arch x86_64
macos: libnative_plugin_macos.dylib


android-arm64: CC = $(wildcard $(ANDROID_NDK_ROOT)/toolchains/llvm/prebuilt/*/bin/aarch64-linux-android21-clang)
android-arm64: libnative_plugin_android_arm64.so
android-arm32: CC = $(wildcard $(ANDROID_NDK_ROOT)/toolchains/llvm/prebuilt/*/bin/armv7a-linux-androideabi19-clang)
android-arm32: libnative_plugin_android_arm32.so
android-x86_64: CC = $(wildcard $(ANDROID_NDK_ROOT)/toolchains/llvm/prebuilt/*/bin/x86_64-linux-android21-clang)
android-x86_64: libnative_plugin_android_x86_64.so
android-x86: CC = $(wildcard $(ANDROID_NDK_ROOT)/toolchains/llvm/prebuilt/*/bin/i686-linux-android19-clang)
android-x86: libnative_plugin_android_x86.so