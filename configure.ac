dnl Process this file with autoconf to produce a configure script.
AC_INIT([libfec],[3.0.0])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([makefile])

dnl Need canonical target for OS detection
AC_CANONICAL_TARGET

SO_NAME=3
VERSION=3.0.0
AC_SUBST(SO_NAME, [3])
AC_SUBST(VERSION)

dnl Set installation directories
AC_PREFIX_DEFAULT([/usr/local])
AC_SUBST(prefix)
AC_SUBST(exec_prefix)
AC_SUBST(libdir)
AC_SUBST(includedir)
AC_SUBST(mandir)
AC_SUBST(datarootdir)

dnl Checks for programs.
AC_PROG_CC([clang gcc cc])
if test "$GCC" != "yes"; then
    AC_MSG_ERROR([Need GNU-compatible C compiler (clang or gcc)])
fi

AC_CHECK_PROGS([AR], [ar])
AC_PROG_RANLIB

dnl Checks for libraries.
AC_CHECK_LIB([c], [malloc])

dnl Check for pkg-config first
AC_CHECK_PROG([PKG_CONFIG], [pkg-config], [yes], [no])

dnl Check for VOLK library
if test "$PKG_CONFIG" = "yes"; then
    PKG_CHECK_MODULES([VOLK], [volk >= 2.0], [
        AC_DEFINE([HAVE_VOLK], [1], [Define to 1 if VOLK is available])
        AC_MSG_NOTICE([VOLK found, enabling VOLK acceleration])
        VOLK_FOUND=yes
    ], [
        AC_MSG_WARN([VOLK not found. Viterbi decoder will use portable implementation])
        VOLK_FOUND=no
        VOLK_CFLAGS=""
        VOLK_LIBS=""
    ])
else
    AC_MSG_WARN([pkg-config not found. Cannot check for VOLK. Using portable implementation])
    VOLK_FOUND=no
    VOLK_CFLAGS=""
    VOLK_LIBS=""
fi

AC_SUBST(VOLK_CFLAGS)
AC_SUBST(VOLK_LIBS)
AC_SUBST(VOLK_FOUND)

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

dnl Summary
AC_MSG_NOTICE([
Configuration summary:
  Target: $target
  VOLK support: $VOLK_FOUND
  VOLK CFLAGS: $VOLK_CFLAGS
  VOLK LIBS: $VOLK_LIBS
  Shared library: $SH_LIB
])

AC_OUTPUT