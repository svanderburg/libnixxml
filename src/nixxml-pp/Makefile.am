bin_PROGRAMS = nixxml-pp

noinst_HEADERS = pretty-print.h
nixxml_pp_SOURCES = pretty-print.c main.c
nixxml_pp_LDADD = ../../src/libnixxml/libnixxml.la ../../src/libnixxml-data/libnixxml-data.la
nixxml_pp_CFLAGS = $(LIBXML2_CFLAGS) -I../../src/libnixxml -I../../src/libnixxml-data
