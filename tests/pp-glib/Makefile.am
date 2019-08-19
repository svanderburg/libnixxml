TESTS = convert-xml-simple.sh test-identical-simple.sh convert-xml-verbose.sh test-identical-verbose.sh

check_PROGRAMS = pp-glib

noinst_HEADERS = pretty-print-glib.h
pp_glib_SOURCES = pretty-print-glib.c main.c
pp_glib_LDADD = ../../src/libnixxml/libnixxml.la ../../src/libnixxml-glib/libnixxml-glib.la
pp_glib_CFLAGS = $(GLIB2_CFLAGS) $(LIBXML2_CFLAGS) -I../../src/libnixxml -I../../src/libnixxml-glib

EXTRA_DIST = convert-xml-simple.sh.in test-identical-simple.sh convert-xml-verbose.sh.in test-identical-verbose.sh test-ast.nix setup.sh
