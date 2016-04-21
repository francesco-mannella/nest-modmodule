# Automake file for external dynamic modules for NEST
#
# Hans Ekkehard Plesser, April 2008
# Automake file for the Developer Module
#
# libmodmodule is built as a normal, installable library.
# It will be installed to $prefix/lib by make install.
#
# Headers from this directory are not to be installed upon
# make install. They are therefore included in _SOURCES.


# 1. Exchange "mod" in "modmodule" with the name of your model below
#    (ten locations).
# 2. Add all .cpp and .h files from your code as *_SOURCES. Header files
#    are given only so that they will be included in the tarball if you
#    run "make dist" on your module.
# 3. The modmodule* stuff creates a module that can be loaded at runtime.
#    It is called modmodule.so.
# 4. The libmodmodule* stuff creates a library against which NEST can be
#    linked.

libdir= @libdir@/nest

# We need to install the module header for static linking on BlueGene
include_HEADERS = modmodule.h

# All other source files
source_files=  modmodule.cpp \
               modulatory_connection.cpp \
               modulatory_connection.h \
               d1_connection.h

if BUILD_DYNAMIC_USER_MODULES
  lib_LTLIBRARIES= libmodmodule.la modmodule.la
  modmodule_la_CXXFLAGS= @AM_CXXFLAGS@
  modmodule_la_SOURCES=  $(source_files)
  modmodule_la_LDFLAGS=  -module
else
  lib_LTLIBRARIES= libmodmodule.la
endif

libmodmodule_la_CXXFLAGS= @AM_CXXFLAGS@ -DLINKED_MODULE
libmodmodule_la_SOURCES=  $(source_files)

MAKEFLAGS= @MAKE_FLAGS@

AM_CPPFLAGS= @NEST_CPPFLAGS@ \
             @INCLTDL@

.PHONY: install-slidoc

pkgdatadir=@datadir@/nest

nobase_pkgdata_DATA=\
	sli/modmodule-init.sli

install-slidoc:
	NESTRCFILENAME=/dev/null $(DESTDIR)$(NEST_PREFIX)/bin/sli --userargs="@HELPDIRS@" $(NEST_PREFIX)/share/nest/sli/install-help.sli

install-data-hook: install-exec install-slidoc

EXTRA_DIST= sli
