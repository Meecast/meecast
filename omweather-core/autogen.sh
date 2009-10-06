#!/bin/sh

set -x
glib-gettextize --copy --force
libtoolize --automake --copy --force
aclocal
#intltoolize --automake --copy --force
autoconf --force
autoheader --force
automake --add-missing --copy --force-missing --foreign
    