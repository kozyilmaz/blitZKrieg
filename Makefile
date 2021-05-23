
subdir-y = \
	fixedpoint

include Makefile.lib

# $(call patchme,DIRECTORY_TO_BE_PATCHED)
define patchme
	@if [ -d "$(1)" ] && [ -d "patches" ] && [ ! -f ".patched" ]; then \
		$(foreach F, $(wildcard $(shell find patches -type f \( -name "*.patch" \) | sort)), patch -d $(1) -p1 < $(F); echo $(F) >> .patched;) \
	fi
endef

patch-deps:
	@echo "  PATCH      deps"
	$(call patchme,.)

libmakefile-check:
	@echo "  CHECK      libmakefile"
	@git fetch https://github.com/alperakcan/libmakefile.git master
	@$(TOPDIR)/scripts/git-subtree-check.sh -r deps/libmakefile

libmakefile-update:
	@echo "  UPDATE     libmakefile"
ifeq ($(shell git remote -v |grep '\<libmakefile-remote\>'),)
	@git remote add libmakefile-remote https://github.com/alperakcan/libmakefile.git
else
	@git remote set-url libmakefile-remote https://github.com/alperakcan/libmakefile.git
endif
	@git subtree pull --prefix=deps/libmakefile/ --squash libmakefile-remote master

r128-build:
	@echo "  COMPILE    r128"
	@make -C deps/r128/test

r128-clean:
	@echo "  CLEAN      r128"
	@rm -rf deps/r128/test/test

r128-test:
	@echo "  TEST       r128"
	@./deps/r128/test/test
