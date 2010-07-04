dnl
dnl $Id: gcc-specs.m4,v 1.2 2001-09-27 11:59:11 chris Exp $
dnl
dnl Check whether the target compiler accepts -specs
dnl

AC_DEFUN(RTEMS_GCC_SPECS,
[AC_REQUIRE([RTEMS_PROG_CC])
AC_CACHE_CHECK(whether $CC_FOR_TARGET accepts -specs,rtems_cv_gcc_specs,
[
rtems_cv_gcc_specs=no
if test "$rtems_cv_prog_gcc" = "yes"; then
  touch confspec
  echo 'void f(){}' >conftest.c
  if test -z "`${CC_FOR_TARGET} -specs confspec -c conftest.c 2>&1`";then
    rtems_cv_gcc_specs=yes
  fi
fi
rm -f confspec conftest*
])])
