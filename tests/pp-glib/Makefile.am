TESTS = convert-xml.sh test-identical.sh

check_PROGRAMS = pp-glib

noinst_HEADERS = pretty-print-glib.h
pp_glib_SOURCES = pretty-print-glib.c main.c
pp_glib_LDADD = ../../src/libnixxml/libnixxml.la ../../src/libnixxml-glib/libnixxml-glib.la
pp_glib_CFLAGS = $(GLIB2_CFLAGS) $(LIBXML2_CFLAGS) -I../../src/libnixxml -I../../src/libnixxml-glib

EXTRA_DIST = convert-xml.sh.in test-identical.sh test-ast.nix setup.sh
