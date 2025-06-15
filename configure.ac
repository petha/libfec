dnl Process this file with autoconf to produce a configure script.
AC_INIT([libfec],[3.0.0])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([makefile])

SO_NAME=3
VERSION=3.0.0
AC_SUBST(SO_NAME, [3])
AC_SUBST(VERSION)

dnl Checks for programs.
AC_PROG_CC([clang gcc cc])
if test "$GCC" != "yes"; then
    AC_MSG_ERROR([Need GNU-compatible C compiler (clang or gcc)])
fi

AC_CHECK_PROGS([AR], [ar])

AC_PROG_RANLIB

dnl Checks for libraries.
AC_CHECK_LIB([c], [malloc])

dnl Checks for header files.
AC_CHECK_HEADERS([getopt.h stdio.h stdlib.h memory.h string.h])

if test "$ac_cv_header_stdio_h" != "yes"; then
    AC_MSG_ERROR([Need stdio.h!])
fi
if test "$ac_cv_header_stdlib_h" != "yes"; then
    AC_MSG_ERROR([Need stdlib.h!])
fi
if test "$ac_cv_header_memory_h" != "yes"; then
    AC_MSG_ERROR([Need memory.h!])
fi
if test "$ac_cv_header_string_h" != "yes"; then
    AC_MSG_ERROR([Need string.h!])
fi

AC_CANONICAL_TARGET

ARCH_OPTION="-O3 -march=native -mtune=native -ffast-math -funroll-all-loops"
AC_SUBST(ARCH_OPTION, ["-O3 -march=native -mtune=native -ffast-math -funroll-all-loops"])

MLIBS=""

case $target_os in
darwin*)
    SH_LIB=libfec.dylib
    REBIND=""
    ;;
*)
    SH_LIB=libfec.so
    REBIND=ldconfig
    ;;
esac

AC_SUBST(SH_LIB)
AC_SUBST(REBIND)
AC_SUBST(MLIBS)

dnl Checks for library functions.
AC_CHECK_FUNCS(getopt_long memset memmove)

AC_OUTPUT