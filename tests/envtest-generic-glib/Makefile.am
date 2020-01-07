check_PROGRAMS = envtest

TESTS = test-envtest.sh

envtest_SOURCES =  main.c
envtest_LDADD = ../../src/libnixxml/libnixxml.la ../../src/libnixxml-glib/libnixxml-glib.la
envtest_CFLAGS = $(LIBXML2_CFLAGS) $(GLIB2_CFLAGS) -I../../src/libnixxml -I../../src/libnixxml-glib

EXTRA_DIST = test-envtest.sh check-envvars.sh
