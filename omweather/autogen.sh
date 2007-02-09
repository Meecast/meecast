#!/bin/sh

#set -x
#aclocal-1.7
#autoconf
#libtoolize
#automake-1.7 --add-missing --foreign

set -x
glib-gettextize --copy --force
libtoolize --automake --copy --force
intltoolize --automake --copy --force
aclocal
autoconf --force
autoheader --force
automake --add-missing --copy --force-missing --foreign
