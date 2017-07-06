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
AC_INIT([fractalrender], [0.4.4], [group@chemicaldevelopment.us])

AC_CONFIG_MACRO_DIRS([m4 custom_m4])

AM_INIT_AUTOMAKE([])


AC_CONFIG_SRCDIR([src/fractalrender.h])
AC_CONFIG_HEADERS([config.h])


AC_LANG(C)
AC_PROG_CC
PKG_PROG_PKG_CONFIG
LT_INIT([disable-static])

AC_CHECK_HEADERS([stdlib.h stdio.h stdbool.h string.h math.h sys/time.h sys/stat.h time.h unistd.h assert.h stdarg.h getopt.h ctype.h dlfcn.h])

AC_CHECK_LIB(m, log)
AC_CHECK_LIB(dl, dlopen)



AC_ARG_WITH([gmp],
    [AS_HELP_STRING([--with-gmp],
    [use gmp @<:@default=check@:>@])],
    [],
    [with_gmp=check]
)

if test "x$with_gmp" != xno; then
    AS_IF([test "x$with_gmp" != xyes && test "x$with_gmp" != xcheck], [
        LDFLAGS="$LDFLAGS -L$with_gmp/lib"
        CFLAGS="$CFLAGS -I$with_gmp/include"
    ])

    AC_CHECK_LIB([gmp], [main], [
        AC_CHECK_HEADERS([gmp.h])
        AC_DEFINE([HAVE_GMP], [1], [Define if you have bmp])
        export with_bmp=yes
        AC_MSG_CHECKING([libgmp])
        AC_MSG_RESULT([yes])

    ], [
        AC_MSG_CHECKING([libgmp])
        AC_MSG_RESULT([no])
        if test "x$with_gmp" != xcheck; then
            AC_MSG_FAILURE([--with-gmp was given, but test for libgmp failed])
        fi
        export with_gmp=no
    ])
fi


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
        export with_png=no
    ])
fi


AC_ARG_WITH([cuda],
    [AS_HELP_STRING([--with-cuda],
    [use cuda @<:@default=check@:>@])],
    [],
    [with_cuda=check]
)



cuda_prefix="$with_cuda"

# Setting the prefix to the default if only --with-cuda was given
if test "$cuda_prefix" == "yes"; then
	cuda_prefix="/usr/local/cuda"
fi

if test "$cuda_prefix" == "check"; then
	cuda_prefix="/usr/local/cuda"
fi


if test "x${with_cuda}" != xno; then
  AS_IF([test "x${with_cuda}" != xyes && test "x${with_cuda}" != xcheck], [
    LDFLAGS="$LDFLAGS -L${with_cuda}/lib$SUFFIX -Wl,-rpath,${with_cuda}/lib$SUFFIX"
    CFLAGS="$CFLAGS -I${with_cuda}/include"
  ])
  AS_IF([test "x${with_cuda}" == xcheck], [
    LDFLAGS="$LDFLAGS -L/usr/local/cuda/lib$SUFFIX -Wl,-rpath,${with_cuda}/lib$SUFFIX"
    CFLAGS="$CFLAGS -I/usr/local/cuda/include"
  ])
fi


VALID_CUDA=no

# Checking for nvcc
AC_MSG_CHECKING([nvcc in $cuda_prefix/bin])
if test -x "$cuda_prefix/bin/nvcc"; then
	AC_MSG_RESULT([found])
	AC_DEFINE_UNQUOTED([NVCC_PATH], ["$cuda_prefix/bin/nvcc"], [Path to nvcc binary])
	# We need to add the CUDA search directories for header and lib searches

	CUDA_CFLAGS=""
  AC_DEFINE([HAVE_CUDA], [], [Have cuda support])

	# Saving the current flags
	ax_save_CFLAGS="${CFLAGS}"
	ax_save_LDFLAGS="${LDFLAGS}"

	# Announcing the new variables
	AC_SUBST([CUDA_CFLAGS])
	AC_SUBST([CUDA_LDFLAGS])
	AC_SUBST([NVCC],[$cuda_prefix/bin/nvcc])
  AC_CHECK_FILE([$cuda_prefix/lib64],[lib64_found=yes],[lib64_found=no])
  AC_CHECK_FILE([$cuda_prefix/lib32],[lib32_found=yes],[lib32_found=no])
  AC_CHECK_FILE([$cuda_prefix/lib],[lib_found=yes],[lib_found=no])
	if test "x$lib64_found" = xyes ; then
			AC_SUBST([CUDA_LIBDIR],[$cuda_prefix/lib64])
      CUDA_LIBDIR=$cuda_prefix/lib64
      VALID_CUDA=yes
	else
    if test "x$lib_found" = xyes ; then
  			AC_SUBST([CUDA_LIBDIR],[$cuda_prefix/lib])
        CUDA_LIBDIR=$cuda_prefix/lib
        VALID_CUDA=yes
  	else
      if test "x$lib32_found" = xyes ; then
    			AC_SUBST([CUDA_LIBDIR],[$cuda_prefix/lib32])
          CUDA_LIBDIR=$cuda_prefix/lib32
          VALID_CUDA=yes
    	else
        AC_MSG_WARN([Couldn't find cuda lib directory])
        VALID_CUDA=no
      fi
    fi
  fi

	if test "x$VALID_CUDA" != xno ; then
    echo $CUDA_LIBDIR
		CUDA_CFLAGS="$CUDA_CFLAGS -I$cuda_prefix/include/"
		CFLAGS="$CUDA_CFLAGS $CFLAGS"
		CUDA_LDFLAGS="-L$CUDA_LIBDIR"
		LDFLAGS="$CUDA_LDFLAGS $LDFLAGS -Wl,-rpath,$CUDA_LIBDIR"

		# And the header and the lib
		AC_CHECK_HEADERS([cuda.h cuda_runtime.h helper_functions.h helper_cuda.h])
		if test "x$VALID_CUDA" != "xno" ; then
			AC_CHECK_LIB([cuda], [cuInit], [VALID_CUDA=yes], AC_MSG_WARN([Couldn't find libcuda]
			VALID_CUDA=no))
		fi
  else
    # Returning to the original flags
    CFLAGS="${ax_save_CFLAGS} -I$cuda_prefix/include"
    LDFLAGS="${ax_save_LDFLAGS} -L$cuda_prefix/lib$SUFFIX -Wl,-rpath,$cuda_prefix/lib$SUFFIX"
	fi
else
	AC_MSG_RESULT([not found!])
	AC_MSG_WARN([nvcc was not found in $cuda_prefix/bin])
	VALID_CUDA=no
fi

if test "x$with_cuda" = xyes && test x$VALID_CUDA = xyes ; then
	AC_MSG_NOTICE([Building with CUDA bindings])
elif test "x$with_cuda" = xyes && test x$VALID_CUDA = xno ; then
	AC_MSG_ERROR([Cannot build CUDA bindings. Check errors])
fi



AC_ARG_WITH([opencl],
    [AS_HELP_STRING([--with-opencl],
    [use opencl @<:@default=check@:>@])],
    [],
    [with_opencl=check]
)


AS_IF([test "x${with_opencl}" != xno], [
    AC_CHECK_HEADERS([OpenCL/opencl.h CL/cl.h OpenCL/cl.h])

    OPENCL_LIBS="$LIBS -lOpenCL"
], [
    OPENCL_LIBS="$LIBS"
])

AS_IF([test "x${with_opencl}" != xno && test "x${with_opencl}" != xcheck && test "x${with_opencl}" != xyes], [
  OPENCL_CFLAGS="$CFLAGS -I${with_opencl}"
  OPENCL_LDFLAGS="$LDFLAGS -L${with_opencl}"
], [
  OPENCL_CFLAGS="$CFLAGS"
  OPENCL_LDFLAGS="$LDFLAGS"
])


AS_IF([test "x${with_opencl}" != xno], [
  AC_MSG_CHECKING([checking for -lOpenCL])
  AS_IF([ ${CC} $OPENCL_CFLAGS $OPENCL_LDFLAGS test_opencl.c $OPENCL_LIBS 2> /dev/null ] ,[
    AC_MSG_RESULT([yes])
    AC_DEFINE([HAVE_OPENCL], [1], [Use OPENCL])
  ], [
    AC_MSG_RESULT([no])
    AC_MSG_CHECKING([checking for -framework OpenCL])
    OPENCL_LIBS="$LIBS -framework OpenCL"
      AS_IF([ ${CC} $OPENCL_CFLAGS $OPENCL_LDFLAGS test_opencl.c $OPENCL_LIBS 2> /dev/null ] ,[
      AC_MSG_RESULT([yes])
      AC_DEFINE([HAVE_OPENCL], [1], [Use OPENCL])
    ],
    [
      AC_MSG_RESULT([no])
      export with_opencl=no

      AS_IF([test "x${with_opencl}" != xcheck], [
        AC_MSG_ERROR([--with-opencl was specified, but OpenCL not found])
      ], [
        AC_MSG_WARN([Check for OpenCL failed, so not using it])
      ])
    ])
  ])
  ], [
])


AC_ARG_WITH([sdl2],
    [AS_HELP_STRING([--with-sdl2],
    [use sdl2 @<:@default=check@:>@])],
    [],
    [with_sdl2=check]
)


if test "x$with_sdl2" != xno; then
    AS_IF([test "x$with_sdl2" != xyes && test "x$with_sdl2" != xcheck], [
        LDFLAGS="$LDFLAGS -L$with_sdl2/lib"
        CFLAGS="$CFLAGS -I$with_sdl2/include"
    ])

    SDL_VERSION=2.0.0
    AM_PATH_SDL2([$SDL_VERSION],[
            AC_MSG_CHECKING([sdl2])
            AC_MSG_RESULT([yes])
            export with_sdl2=yes
        ], [
            AC_MSG_CHECKING([sdl2])
            AC_MSG_RESULT([no])
            if test "x$with_sdl2" != xcheck; then
                AC_MSG_FAILURE([--with-sdl2 was given, but test for libsdl failed])
            fi
            export with_sdl2=no
        ]
    )
fi






AC_SUBST([OPENCL_LIBS], [$OPENCL_LIBS])
AC_SUBST([OPENCL_CFLAGS], [$OPENCL_CFLAGS])
AC_SUBST([OPENCL_LDFLAGS], [$OPENCL_LDFLAGS])
AC_SUBST([SDL_CFLAGS], [$SDL_CFLAGS])
AC_SUBST([SDL_LDFLAGS], [$SDL_LIBS])


AM_CONDITIONAL([HAVE_GMP], [test x$with_gmp != xno])
AM_CONDITIONAL([HAVE_PNG], [test x$with_png != xno])
AM_CONDITIONAL([HAVE_OPENCL], [test x$with_opencl != xno])
AM_CONDITIONAL([HAVE_CUDA], [test x$VALID_CUDA == xyes])
AM_CONDITIONAL([HAVE_SDL], [test x$with_sdl != xno])




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
