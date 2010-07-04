dnl $Id: check-posix.m4,v 1.2 2001-09-27 11:59:11 chris Exp $
dnl
AC_DEFUN(RTEMS_CHECK_POSIX_API,
[dnl
AC_REQUIRE([RTEMS_ENV_RTEMSBSP])dnl
AC_REQUIRE([RTEMS_CHECK_CPU])dnl
AC_REQUIRE([RTEMS_ENABLE_POSIX])dnl

AC_CACHE_CHECK([whether BSP supports libposix],
  rtems_cv_HAS_POSIX_API,
  [dnl
    case "$RTEMS_CPU" in
    unix*)
      rtems_cv_HAS_POSIX_API="no"
      ;;
    *)
      if test "${RTEMS_HAS_POSIX_API}" = "yes"; then
        rtems_cv_HAS_POSIX_API="yes";
      else
        rtems_cv_HAS_POSIX_API="disabled";
      fi
      ;;
    esac])
if test "$rtems_cv_HAS_POSIX_API" = "yes"; then
  HAS_POSIX_API="yes";
else
  HAS_POSIX_API="no";
fi
AC_SUBST(HAS_POSIX_API)dnl
])
