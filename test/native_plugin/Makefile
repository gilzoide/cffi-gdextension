LINKFLAGS += -shared

libnative_plugin.dylib: plugin.c
	$(CC) -o $@ $< $(CFLAGS) $(LINKFLAGS)