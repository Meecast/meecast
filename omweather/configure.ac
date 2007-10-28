AC_PREREQ(2.59)
AC_INIT(omweather, 0.19.1)
AM_INIT_AUTOMAKE(omweather, 0.19.1)
AC_CONFIG_HEADER([src/config.h:src/config.h.in])
AC_CONFIG_SRCDIR([src/weather-home.c])
AC_CANONICAL_HOST
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
AC_PROG_INTLTOOL([0.23])	
# Checks for libraries.

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h memory.h netdb.h netinet/in.h stdlib.h string.h sys/param.h sys/socket.h sys/time.h unistd.h])
AC_CHECK_HEADERS([libintl.h])
AC_CHECK_HEADERS([wchar.h])
AC_CHECK_HEADERS([limits.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_TIME
AC_STRUCT_TM
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([clock_gettime gethostbyname gettimeofday memset strcasecmp strchr strdup strerror strncasecmp strstr setlocale])

# Localisation
localedir=`pkg-config osso-af-settings --variable=localedir`
GETTEXT_PACKAGE=$PACKAGE
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], "${GETTEXT_PACKAGE}", [Name of gettext package])
AC_DEFINE_UNQUOTED([LOCALEDIR], "${localedir}", [Path to store localization files])
ALL_LINGUAS="ru_RU fr_FR fi_FI"
AM_GLIB_GNU_GETTEXT

PKG_CHECK_MODULES(GTK, [gtk+-2.0])
AC_SUBST(GTK_LIBS)
AC_SUBST(GTK_CFLAGS)

PKG_CHECK_MODULES(OSSO, [libosso >= 1])
AC_SUBST(OSSO_LIBS)
AC_SUBST(OSSO_CFLAGS)

PKG_CHECK_MODULES(LIBXML2, [libxml-2.0 >= 2.6.16])
AC_SUBST(LIBXML2_LIBS)
AC_SUBST(LIBXML2_CFLAGS)

PKG_CHECK_MODULES(LIBCURL, libcurl)
AC_SUBST(LIBCURL_LIBS)
AC_SUBST(LIBCURL_CFLAGS)

PKG_CHECK_MODULES(GNOME_VFS, [gnome-vfs-2.0 >= 2.8.4.11 gnome-vfs-module-2.0 >= 2.8.4.11])
AC_SUBST(GNOME_VFS_LIBS)
AC_SUBST(GNOME_VFS_CFLAGS)

PKG_CHECK_MODULES(GCONF, [gconf-2.0 >= 2.6.2])
AC_SUBST(GCONF_LIBS)
AC_SUBST(GCONF_CFLAGS)

PKG_CHECK_MODULES(DESKTOP, hildon-desktop)
AC_SUBST(DESKTOP_LIBS)
AC_SUBST(DESKTOP_CFLAGS)

PKG_CHECK_MODULES(HILDON, hildon-1 >= 0.9.9, \
                  AC_DEFINE(HILDON, 1, [Version of hildon libraries]), \
                  [AC_DEFINE(HILDON, 0, [Version of hildon libraries]) \
                  PKG_CHECK_MODULES(HILDON_LIBS, hildon-libs >= 0.12.0)])
AC_SUBST(HILDON_CFLAGS)
AC_SUBST(HILDON_LIBS)

AC_ARG_ENABLE(conic, [ --enable-conic=[yes|no] ], [use_conic=$enableval ])
if test "$use_conic" != "no"; then
PKG_CHECK_MODULES(CONIC, [conic],
    [
            use_conic="yes"
    ],
    [
        use_conic="no"
        echo 'No New Connectivity CONIC Library!!!!.'
    ]
)
if test "$use_conic" = "yes"; then
AC_SUBST(CONIC_CFLAGS)
AC_SUBST(CONIC_LIBS)
        AC_DEFINE(USE_CONIC, 1,"Set to 1 if enable-conic is yes")
fi
fi

# Help file install directory
helpfile_engbdir=$datadir/osso-help/en_US
AC_SUBST(helpfile_engbdir)

# Checks for library functions.
AC_CHECK_FUNCS([setlocale])

# Config files
AC_CONFIG_FILES([Makefile
		src/Makefile
		po/Makefile.in
		data/Makefile])
# Produce output files
AC_OUTPUT

