if have_glib
    libnixxml_glib = libnixxml-glib
    libnixxml_glib_pc = libnixxml-glib.pc
endif

SUBDIRS = libnixxml libnixxml-data nixxml-pp $(libnixxml_glib)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libnixxml.pc libnixxml-data.pc $(libnixxml_glib_pc)

EXTRA_DIST = libnixxml.pc.in libnixxml-data.pc.in libnixxml-glib.pc.in
