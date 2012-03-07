AC_PREREQ(2.59)
AC_INIT(meecasttizen, 0.1)
AM_INIT_AUTOMAKE(meecasttizen, 0.1)
AC_CONFIG_HEADER([src/config.h:src/config.h.in])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CANONICAL_HOST
AC_GNU_SOURCE
dnl Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_LN_S
AC_PROG_RANLIB
AC_PROG_LIBTOOL
dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])
dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_STRUCT_TM
dnl Checks for library functions.
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([strrchr])
AC_CHECK_FUNCS([strstr])
AC_FUNC_STRFTIME
dnl glibc2 needs this
AC_DEFINE([_XOPEN_SOURCE], [1], [glibc2 needs this])
dnl check packages
PKG_CHECK_MODULES(GTK, [glib-2.0])
AC_SUBST(GTK_LIBS)
AC_SUBST(GTK_CFLAGS)

PKG_CHECK_MODULES(LIBXML2, [libxml-2.0 >= 2.6.16])
AC_SUBST(LIBXML2_LIBS)
AC_SUBST(LIBXML2_CFLAGS)

PKG_CHECK_MODULES(SQLITE, [sqlite3])
AC_SUBST(SQLITE_LIBS)
AC_SUBST(SQLITE_CFLAGS)

PKG_CHECK_MODULES(LIBCURL, [libcurl])
AC_SUBST(LIBCURL_LIBS)
AC_SUBST(LIBCURL_CFLAGS)

dnl Config files
AC_CONFIG_FILES([Makefile
		src/Makefile])
dnl Produce output files
AC_OUTPUT
