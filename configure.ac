AC_INIT([liblu], [0.1], [andrew@acooke.org])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])
AM_PROG_AR
LT_INIT
AC_PROG_CC_C99
dnl following obsolete, but required by check
AM_PROG_CC_C_O
PKG_CHECK_MODULES([CHECK], [check >= 0.9.0])
AC_SEARCH_LIBS([pow], [m], [], [AC_MSG_ERROR([unable to find the pow() function])])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile lib/Makefile tests/Makefile])
AC_OUTPUT
