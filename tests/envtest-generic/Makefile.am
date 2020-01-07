check_PROGRAMS = envtest

TESTS = test-envtest.sh

envtest_SOURCES =  main.c
envtest_LDADD = ../../src/libnixxml/libnixxml.la ../../src/libnixxml-data/libnixxml-data.la
envtest_CFLAGS = $(LIBXML2_CFLAGS) -I../../src/libnixxml -I../../src/libnixxml-data

EXTRA_DIST = test-envtest.sh check-envvars.sh
