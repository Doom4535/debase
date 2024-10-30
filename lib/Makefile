BUILDDIRS = $(addsuffix /$(BUILDDIR), $(LIBS))
BUILDDIR = build-$(PLATFORM)
LIBS = #libgit2 #ncurses
ARCHS ?= x86_64 arm64

ifeq ($(shell uname -s), Darwin)
	PLATFORM = mac
else
	PLATFORM = linux
endif

# LIBGIT2.BUILDDIR = $(LIBGIT2)/$(BUILDDIR)
# NCURSES.BUILDDIR = $(NCURSES)/$(BUILDDIR)

# LIBGIT2.TMP = $(LIBGIT2).tmp
# NCURSES.TMP = $(NCURSES).tmp

all: $(addsuffix /$(BUILDDIR), $(LIBS))

# all: libgit2/build-Mac
# all: ncurses/build-Mac
# all: $(addsuffix /$(BUILDDIR), $(LIBS))
# all: $(LIBGIT2) $(NCURSES)

# Clean: execute `make clean` for each lib, and then delete $(BUILDDIRS)
CLEANLIBS := $(addsuffix .clean, $(LIBS))

clean: $(CLEANLIBS)
	rm -Rf $(BUILDDIRS)

$(CLEANLIBS):
	$(MAKE) -C $(basename $@) clean || true

# clean:
# 	$(foreach lib, $(LIBS), $(MAKE) -C $(lib) clean || echo hello)
# 	rm -Rf $(BUILDDIRS)
