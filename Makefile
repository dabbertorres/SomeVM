PROJECTS := libSomeVM SomeVM SomeLang

# compile settings for all projects
export GLOBAL_CXX       := g++
export GLOBAL_CFLAGS    := --std=c++17 -Wall -Wextra
export GLOBAL_RLS_FLAGS := -O2
export GLOBAL_DBG_FLAGS := -DDEBUG -O0 -g

WORKING_DIR    := $(realpath .)
OUT_DIR_NAME   := /build
export OUT_DIR := $(addsuffix $(OUT_DIR_NAME), $(WORKING_DIR))

.PHONY: release debug clean

release:
	@mkdir -p $(OUT_DIR)/$@
	@$(MAKE) -C libSomeVM $@
	@$(MAKE) -C SomeVM $@
	@$(MAKE) -C SomeLang $@

debug:
	@mkdir -p $(OUT_DIR)/$@
	@$(MAKE) -C libSomeVM $@
	@$(MAKE) -C SomeVM $@
	@$(MAKE) -C SomeLang $@

clean:
	@rm -rf $(OUT_DIR)
	@$(MAKE) -C libSomeVM $@
	@$(MAKE) -C SomeVM $@
	@$(MAKE) -C SomeLang $@

