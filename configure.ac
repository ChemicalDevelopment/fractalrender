###  configure.ac -- Autoconf configuration script
#
#    Copyright 2016-2017 ChemicalDevelopment
#
#    This file is part of the fractalrender project
#
#    FractalRender source code, as well as any other resources in this project are
#  free software; you are free to redistribute it and/or modify them under
#  the terms of the GNU General Public License; either version 3 of the
#  license, or any later version.
#
#    These programs are hopefully useful and reliable, but it is understood
#  that these are provided WITHOUT ANY WARRANTY, or MERCHANTABILITY or FITNESS
#  FOR A PARTICULAR PURPOSE. See the GPLv3 or email at
#  <info@chemicaldevelopment.us> for more info on this.
#
#    Here is a copy of the GPL v3, which this software is licensed under. You
#  can also find a copy at http://www.gnu.org/licenses/.
###


AC_PREREQ([2.55])
AC_INIT([fractalrender], [0.4.0], [group@chemicaldevelopment.us])
AM_INIT_AUTOMAKE([])

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_SRCDIR([src/fractalrender.h])
AC_CONFIG_HEADERS([config.h])


AC_LANG(C)
AC_PROG_CC
LT_INIT

AC_CHECK_HEADERS([stdlib.h stdio.h stdbool.h string.h math.h sys/time.h sys/stat.h time.h unistd.h assert.h stdarg.h getopt.h ctype.h dlfcn.h])

AC_CHECK_LIB(m, log)
AC_CHECK_LIB(dl, dlopen)


AC_ARG_WITH([png],
    [AS_HELP_STRING([--with-png],
    [use png @<:@default=check@:>@])],
    [],
    [with_png=check]
)

if test "x$with_png" != xno; then
    AS_IF([test "x$with_png" != xyes && test "x$with_png" != xcheck], [
        LDFLAGS="$LDFLAGS -L$with_png/lib"
        CFLAGS="$CFLAGS -I$with_png/include"
    ])

    AC_CHECK_LIB([png], [main], [
        AC_CHECK_HEADERS([png.h])
        AC_DEFINE([HAVE_PNG], [1], [Define if you have libpng])
        export with_png=yes
        AC_MSG_CHECKING([libpng])
        AC_MSG_RESULT([yes])

    ], [
        AC_MSG_CHECKING([libpng])
        AC_MSG_RESULT([no])
        if test "x$with_png" != xcheck; then
            AC_MSG_FAILURE([--with-png was given, but test for libpng failed])
        fi
        export with_$1=no
    ])
fi


AM_CONDITIONAL([HAVE_PNG], [test x$with_png != xno])




AC_CHECK_SIZEOF([double])
AC_CHECK_SIZEOF([unsigned char])
AC_CHECK_SIZEOF([unsigned short])
AC_CHECK_SIZEOF([unsigned int])
AC_CHECK_SIZEOF([unsigned long])


# Checks for library functions.
#AC_FUNC_MALLOC
#AC_FUNC_REALLOC

# todo, add tests
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT


