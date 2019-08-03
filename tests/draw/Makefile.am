check_PROGRAMS = draw compare-draw

TESTS = convert-xml.sh draw-xml.sh draw-nix.sh draw-png.sh draw-invalid.sh draw-wrong.sh check-equal.sh

noinst_HEADERS = dimensions.h drawcommand.h drawarray.h drawspec.h figure.h figurestable.h metatable.h tagsarray.h
COMMON_SRCS = dimensions.c drawcommand.c drawarray.c drawspec.c figure.c figurestable.c metatable.c tagsarray.c

draw_SOURCES = $(COMMON_SRCS) draw.c
draw_LDADD = $(GDLIB_LIBS) ../../src/libnixxml/libnixxml.la ../../src/libnixxml-data/libnixxml-data.la
draw_CFLAGS = $(LIBXML2_CFLAGS) $(GDLIB_CFLAGS) -I../../src/libnixxml -I../../src/libnixxml-data

compare_draw_SOURCES = $(COMMON_SRCS) compare-draw.c
compare_draw_LDADD = $(GDLIB_LIBS) ../../src/libnixxml/libnixxml.la ../../src/libnixxml-data/libnixxml-data.la
compare_draw_CFLAGS = $(LIBXML2_CFLAGS) $(GDLIB_CFLAGS) -I../../src/libnixxml -I../../src/libnixxml-data

EXTRA_DIST = test.nix convert-xml.sh.in draw-xml.sh draw-nix.sh draw-png.sh draw-invalid.sh draw-wrong.sh check-equal.sh
