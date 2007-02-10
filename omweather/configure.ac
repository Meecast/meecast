AC_INIT(Makefile.am)
AM_INIT_AUTOMAKE(omweather, 0.15.1)
AC_CONFIG_HEADERS(src/config.h)
# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_CC
AC_PROG_LIBTOOL
# Checks for libraries.

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h memory.h netdb.h netinet/in.h stdlib.h string.h sys/param.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_TIME
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([clock_gettime gethostbyname gettimeofday memset strcasecmp strchr strdup strerror strncasecmp strstr])

PKG_CHECK_MODULES(GTK, gtk+-2.0)
AC_SUBST(GTK_LIBS)
AC_SUBST(GTK_CFLAGS)

PKG_CHECK_MODULES(OSSO, libosso >= 1)
AC_SUBST(OSSO_LIBS)
AC_SUBST(OSSO_CFLAGS)

PKG_CHECK_MODULES(LIBXML2, libxml-2.0 >= 2.6.16)
AC_SUBST(LIBXML2_LIBS)
AC_SUBST(LIBXML2_CFLAGS)

PKG_CHECK_MODULES(GNOME_VFS, gnome-vfs-2.0 >= 2.8.4.11 gnome-vfs-module-2.0 >= 2.8.4.11)
AC_SUBST(GNOME_VFS_LIBS)
AC_SUBST(GNOME_VFS_CFLAGS)

PKG_CHECK_MODULES(GCONF, gconf-2.0 >= 2.6.2)
AC_SUBST(GCONF_LIBS)
AC_SUBST(GCONF_CFLAGS)


# Help file install directory
helpfile_engbdir=$datadir/osso-help/en_US
AC_SUBST(helpfile_engbdir)

# Config files
AC_CONFIG_FILES([Makefile
		src/lib/Makefile
		src/Makefile
		data/Makefile])
# Produce output files
AC_OUTPUT

