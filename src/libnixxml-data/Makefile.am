lib_LTLIBRARIES = libnixxml-data.la
include_HEADERS = nixxml-ptrarray.h nixxml-xmlhashtable.h

libnixxml_data_la_SOURCES = nixxml-ptrarray.c nixxml-xmlhashtable.c
libnixxml_data_la_CFLAGS = $(LIBXML2_CFLAGS) -I../libnixxml
libnixxml_data_la_LIBADD = ../libnixxml/libnixxml.la
