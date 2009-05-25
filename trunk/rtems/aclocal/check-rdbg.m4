dnl $Id: check-rdbg.m4,v 1.2 2001-09-27 11:59:11 chris Exp $
dnl
AC_DEFUN(RTEMS_CHECK_RDBG,
[dnl
AC_REQUIRE([RTEMS_TOP])dnl
AC_REQUIRE([RTEMS_CHECK_CPU])dnl
AC_REQUIRE([RTEMS_CHECK_NETWORKING])dnl
AC_REQUIRE([RTEMS_ENABLE_RDBG])dnl
AC_CACHE_CHECK([whether BSP supports librdbg],
  rtems_cv_HAS_RDBG,
  [
    if test -d "$srcdir/${RTEMS_TOPdir}/c/src/librdbg/src/${RTEMS_CPU}/${$1}";
    then
      rtems_cv_HAS_RDBG="yes" ;
    elif test -d "$srcdir/${RTEMS_TOPdir}/c/src/librdbg/src/${RTEMS_CPU}/any"; 
    then
      rtems_cv_HAS_RDBG="yes" ;
    else
      rtems_cv_HAS_RDBG="no";
    fi
  ])
HAS_RDBG="$rtems_cv_HAS_RDBG"
AC_SUBST(HAS_RDBG)
])
