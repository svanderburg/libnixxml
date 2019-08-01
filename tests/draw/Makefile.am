check_PROGRAMS = draw

TESTS = convert-xml.sh draw-xml.sh draw-nix.sh draw-png.sh draw-invalid.sh draw-wrong.sh

noinst_HEADERS = dimensions.h drawcommand.h drawarray.h drawspec.h figure.h figurestable.h metatable.h tagsarray.h

draw_SOURCES = dimensions.c drawcommand.c drawarray.c drawspec.c figure.c figurestable.c metatable.c tagsarray.c main.c
draw_LDADD = $(GDLIB_LIBS) ../../src/libnixxml/libnixxml.la ../../src/libnixxml-data/libnixxml-data.la
draw_CFLAGS = $(LIBXML2_CFLAGS) $(GDLIB_CFLAGS) -I../../src/libnixxml -I../../src/libnixxml-data

EXTRA_DIST = test.nix convert-xml.sh.in draw-xml.sh draw-nix.sh draw-png.sh draw-invalid.sh draw-wrong.sh
