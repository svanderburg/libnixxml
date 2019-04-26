AC_PREREQ(2.61)
AC_INIT([libnixxml], m4_esyscmd([echo -n $(cat ./version)$VERSION_SUFFIX]))
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([1.9 tar-pax -Wall -Werror -Wno-extra-portability])

AC_ARG_WITH([gd], AS_HELP_STRING([--with-gd], [Build with libgd support for the testcases]))

# Checks for installed programs
AC_DEFUN([NEED_PROG],
[
AC_PATH_PROG($1, $2)
if test -z "$$1"; then
    AC_MSG_ERROR([$2 is required])
fi
])

# Checks for programs
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AM_PROG_CC_C_O
NEED_PROG(getopt, getopt)
NEED_PROG(xsltproc, xsltproc)

# Checks for libxml2 libraries
LIBXML2_REQUIRED=2.5.10
PKG_CHECK_MODULES(LIBXML2, libxml-2.0 >= $LIBXML2_REQUIRED)
AC_SUBST(LIBXML2_CFLAGS)
AC_SUBST(LIBXML2_LIBS)

# Checks for gdlib libraries
GDLIB_REQUIRED=2.2.0
AS_IF([test "x$with_gd" = "xyes"], [
    PKG_CHECK_MODULES(GDLIB, gdlib >= $GDLIB_REQUIRED)
    AC_SUBST(GDLIB_CFLAGS)
    AC_SUBST(GDLIB_LIBS)
])

# Output
AC_CONFIG_FILES([
Makefile
src/libnixxml.pc
src/Makefile
src/libnixxml/Makefile
src/libnixxml-data/Makefile
src/prettyprint/Makefile
scripts/Makefile
scripts/nixexpr2xml
xsl/Makefile
tests/Makefile
tests/draw/Makefile
tests/draw/convert-xml.sh
], [
chmod +x tests/draw/convert-xml.sh
])
AC_OUTPUT
