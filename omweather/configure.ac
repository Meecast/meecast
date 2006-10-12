AC_INIT(Makefile.am)
AM_INIT_AUTOMAKE(omweather, 0.12)

AC_PROG_CC
AC_CONFIG_HEADERS(src/config.h)
AC_PROG_INSTALL
AC_PROG_LIBTOOL

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

# Produce output files
AC_OUTPUT( Makefile src/lib/Makefile src/Makefile  data/Makefile )

