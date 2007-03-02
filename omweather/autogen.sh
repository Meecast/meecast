#!/bin/sh

set -x
glib-gettextize --copy --force
libtoolize --automake --copy --force
aclocal
intltoolize --automake
autoconf --force
autoheader --force
automake --add-missing --copy --force-missing --foreign
