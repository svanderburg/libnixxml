lib_LTLIBRARIES = libnixxml-data.la
include_HEADERS = nixxml-ptrarray.h nixxml-xmlhashtable.h nixxml-xmlhashtable-scanner.h nixxml-ds.h

libnixxml_data_la_SOURCES = nixxml-ptrarray.c nixxml-xmlhashtable.c nixxml-xmlhashtable-scanner.c nixxml-ds.c
libnixxml_data_la_CFLAGS = $(LIBXML2_CFLAGS) -I../libnixxml
libnixxml_data_la_LIBADD = ../libnixxml/libnixxml.la
