AC_INIT([zhavam], [1.3], [joseluis.nunez@selenitas.es])
AM_INIT_AUTOMAKE
# AM_INIT_AUTOMAKE([1.9.6 -Wall -Werror dist-bzip2])
AC_PROG_CC
# Compiling sources with per-target flags requires AM_PROG_CC_C_O
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AM_PATH_GTK_3_0([3.22.1],,AC_MSG_ERROR([Gtk+ 3.22.1 or higher required.]))

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT