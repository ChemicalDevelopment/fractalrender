###  configure.ac -- Autoconf configuration script
#
#    Copyright 2016-2017 ChemicalDevelopment
#
#    This file is part of the FractalRender project
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
AC_INIT([FractalRender], [0.3.2], [group@chemicaldevelopment.us])
AM_INIT_AUTOMAKE([subdir-objects])

#AC_CONFIG_MACRO_DIRS([m4])

AC_CONFIG_SRCDIR([src/fractalrender.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_LANG(C)
AC_PROG_CC
AC_PROG_RANLIB


# Checks for header files.
AC_CHECK_HEADERS([stdlib.h stdio.h stdbool.h string.h math.h sys/time.h sys/stat.h time.h unistd.h])

# Checks for libraries.

AC_CHECK_LIB(m, log)

UNAME_S=$(uname)


AC_CHECK_SIZEOF([double])
AC_CHECK_SIZEOF([unsigned char])
AC_CHECK_SIZEOF([unsigned short])
AC_CHECK_SIZEOF([unsigned int])
AC_CHECK_SIZEOF([unsigned long])
AC_CHECK_SIZEOF([unsigned long long])


ARCH=$(uname -m)
if [[ $ARCH == "x86_64" ]];
then
  SUFFIX="64"
else
  SUFFIX=""
fi


# always use C engine

AC_DEFINE([USE_ENGINE_C], [], [Use C engine])


# these are all required

LIBS="$LIBS"

use_opencl=false

m4_define([FR_WITH],[
  AC_ARG_WITH([$1],
    [AS_HELP_STRING([--with-$1],
    [use $1 @<:@default=check@:>@])],
    [],
    [with_$1=check]
  )
])

m4_define([FR_CHECK],[
  if test "x${with_$1}" != xno; then
    AS_IF([test "x${with_$1}" != xyes && test "x${with_$1}" != xcheck], [
      LDFLAGS="$LDFLAGS -L${with_$1}/lib"
      CFLAGS="$CFLAGS -I${with_$1}/include"
    ])

    AC_CHECK_LIB([$1], [main], [
      AC_CHECK_HEADERS([$1.h])
      AC_DEFINE_UNQUOTED([HAVE_$2], [1], [Define if you have $1])
      LIBS="-l$1 $LIBS"
      export with_$1=yes
    ], [
    if test "x${with_$1}" != xcheck; then
      AC_MSG_FAILURE([--with-$1 was given, but test for $1 failed])
    fi
    export with_$1=no
    ])
  fi
])


FR_WITH([cargs])
FR_CHECK([cargs], [CARGS])



FR_WITH([gmp])
FR_CHECK([gmp], [GMP])

FR_WITH([png])
FR_CHECK([png], [PNG])

AC_ARG_WITH([mpi],
  [AS_HELP_STRING([--with-mpi],
  [use mpi @<:@default=no@:>@])],
  [],
  [with_mpi=no]
)
FR_CHECK([mpi], [MPI])


FR_WITH([cuda])


##### CUDA


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



##### /CUDA






# specially handle OpenCL
#AX_LIB_OPENCL()


FR_WITH([opencl])

AC_CHECK_HEADERS([OpenCL/opencl.h CL/cl.h OpenCL/cl.h])

BEFORE_CL_LIBS="$LIBS"

AS_IF([test "x${with_opencl}" != xno], [
  LIBS="$BEFORE_CL_LIBS -lOpenCL"
], [

])

AS_IF([test "x${with_opencl}" != xno], [
  AC_MSG_CHECKING([checking for -lOpenCL])
  AS_IF([ ${CC} $CFLAGS $LDFLAGS test_opencl.c  $LIBS 2> /dev/null ] ,[
    AC_MSG_RESULT([yes])
    AC_DEFINE([HAVE_OPENCL], [1], [Use OPENCL])
  ], [
    AC_MSG_RESULT([no])
    AC_MSG_CHECKING([checking for -framework OpenCL])
    LIBS="$BEFORE_CL_LIBS -framework OpenCL"
      AS_IF([ ${CC} $CFLAGS $LDFLAGS test_opencl.c $LIBS 2> /dev/null ] ,[
      AC_MSG_RESULT([yes])
      AC_DEFINE([HAVE_OPENCL], [1], [Use OPENCL])
    ],
    [
      AC_MSG_RESULT([no])
      AS_IF([test "x${with_opencl}" != xcheck], [
        AC_MSG_ERROR([--with-opencl was specified, but OpenCL not found])
      ], [
        AC_MSG_WARN([Check for OpenCL failed, so not using it])
      ])
    ])
  ])
  ], [
])

echo $VALID_CUDA

AM_CONDITIONAL([HAVE_CUDA_ENGINE], [test x$VALID_CUDA == xyes])
AM_CONDITIONAL([HAVE_CARGS], [test x$with_cargs != xno])
AM_CONDITIONAL([HAVE_MPI], [test x$with_mpi != xno])
AM_CONDITIONAL([HAVE_GMP], [test x$with_gmp != xno])
AM_CONDITIONAL([HAVE_OPENCL], [test x$with_opencl != xno])
AM_CONDITIONAL([HAVE_CUDA], [test x$with_cuda != xno])
AM_CONDITIONAL([HAVE_PNG], [test x$with_png != xno])

# Checks for library functions.
#AC_FUNC_MALLOC
#AC_FUNC_REALLOC

# todo, add tests
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

AS_IF([test x$with_png == xno], [
    AC_MSG_WARN([Not compiled with PNG support, so FractalRender cannot create .png images! You can only create .raw files])
  ]
)

AS_IF([test x$with_cargs == xno], [
    AC_MSG_ERROR([Not compiled with CARGS, and cargs is needed. Please use --with-cargs])
  ]
)
