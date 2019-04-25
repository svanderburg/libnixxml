bin_PROGRAMS = draw

noinst_HEADERS = dimensions.h drawcommand.h drawspec.h figure.h

draw_SOURCES = dimensions.c drawcommand.c drawspec.c figure.c main.c
draw_LDADD = $(GDLIB_LIBS) ../../src/libnixxml/libnixxml.la ../../src/libnixxml-data/libnixxml-data.la
draw_CFLAGS = $(LIBXML2_CFLAGS) $(GDLIB_CFLAGS) -I../../src/libnixxml -I../../src/libnixxml-data
