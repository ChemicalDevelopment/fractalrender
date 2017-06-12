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
AC_INIT([FractalRender], [0.1.0], [group@chemicaldevelopment.us])
AM_INIT_AUTOMAKE()

AC_CONFIG_SRCDIR([src/fractalrender.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h stdio.h stdbool.h string.h math.h sys/time.h time.h unistd.h])

# Checks for libraries.

AC_CHECK_LIB(m, log)

UNAME_S=$(uname)


AC_CHECK_SIZEOF([unsigned char])
AC_CHECK_SIZEOF([unsigned short])
AC_CHECK_SIZEOF([unsigned int])
AC_CHECK_SIZEOF([unsigned long])
AC_CHECK_SIZEOF([unsigned long long])



# always use C engine

AC_DEFINE([USE_ENGINE_C], [], [Use C engine])


# these are all required

LIBS="-lcargs -lpng -lm $LIBS"


use_opencl=false
use_mpc=false

m4_define([FR_WITH],[
  AC_ARG_WITH([$1],
    [AS_HELP_STRING([--with-$1],
    [use $1 @<:@default=check@:>@])],
    [],
    [with_$1=check]
  )
])

m4_define([FR_CHECK],[
  AS_IF([test "x${with_$1}" != xno],
    [
      AC_CHECK_LIB([$1], [main],
      [

        AC_CHECK_HEADERS([$1.h])
        AC_DEFINE_UNQUOTED([HAVE_$2], [1],
                  [Define if you have $1])
        LIBS="-l$1 $LIBS"

        AS_IF([test "x${with_$1}" != xyes], [
          AS_IF([test "x${with_$1}" != xno], [
            AS_IF([test "x${with_$1}" != xcheck], [
              LDFLAGS="$LDFLAGS -L${with_$1}/lib"
              CFLAGS="$CFLAGS -I${with_$1}/include"
            ])
          ])
        ])
      ],
      [
       
        if test "x${with_$1}" != xcheck; then
          AC_MSG_FAILURE(
            [--with-$1 was given, but test for $1 failed])
        fi
      ], [])]
  )
])


FR_WITH([cargs])
FR_WITH([gmp])
FR_WITH([mpfr])
FR_WITH([mpc])
FR_WITH([png])
FR_WITH([mpi])
FR_WITH([opencl])


FR_CHECK([cargs], [CARGS])
FR_CHECK([gmp], [GMP])
FR_CHECK([mpfr], [MPFR])
FR_CHECK([mpc], [MPC])
FR_CHECK([png], [PNG])
FR_CHECK([mpi], [MPI])

#FR_CHECK([OpenCL], [OPENCL])

# specially handle OpenCL

AS_IF([test "x$with_opencl" != xno],
  [
    AC_CHECK_LIB([OpenCL], [main],
    [
      AC_CHECK_HEADERS([OpenCL/opencl.h])
      AC_CHECK_HEADERS([CL/cl.h])
      AC_DEFINE_UNQUOTED([HAVE_OPENCL], [1],
                [Define if you have OpenCL])
      LIBS="-lOpenCL $LIBS"

      AS_IF([test "x$with_opencl" != xyes], [
        AS_IF([test "x$with_opencl" != xno], [
          AS_IF([test "x$with_opencl" != xcheck], [
            LDFLAGS="$LDFLAGS -L$with_opencl/lib"
            CFLAGS="$CFLAGS -I$with_opencl/include"
          ])
        ])
      ])
    ],
    [
      
      if test "x$with_opencl" != xcheck; then
        AC_MSG_FAILURE(
          [--with-opencl was given, but test for OpenCL failed])
      fi
    ], [])]
)



AM_CONDITIONAL([HAVE_GMP], [test x$with_gmp != xno])
AM_CONDITIONAL([HAVE_MPFR], [test x$with_mpfr != xno])
AM_CONDITIONAL([HAVE_MPC], [test x$with_mpc != xno])
AM_CONDITIONAL([HAVE_OPENCL], [test x$with_opencl != xno])

AM_CONDITIONAL([HAVE_PNG], [test x$with_png != xno])




# Checks for library functions.
#AC_FUNC_MALLOC
#AC_FUNC_REALLOC


# todo, add tests
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

