GITHUB_CLI ?= gh
RUN_ID ?=

download-latest-build:
	$(eval TMPDIR := $(shell mktemp -d))
	$(GITHUB_CLI) run download $(RUN_ID) -n cffi-gdextension -D $(TMPDIR)
	cp $(TMPDIR)/addons/cffi/build/libcffi* addons/cffi/build
	$(RM) -r $(TMPDIR)