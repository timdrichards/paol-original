#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([paol-threads], [2.01], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([main.cpp])
AM_INIT_AUTOMAKE([1.10 no-define])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lPvAPI':
AC_CHECK_LIB([PvAPI], [PvCameraCount])
# FIXME: Replace `main' with a function in `-lSampleCapture':
AC_CHECK_LIB([SampleCapture], [main])
# FIXME: Replace `main' with a function in `-laa':
AC_CHECK_LIB([aa], [main])
# FIXME: Replace `main' with a function in `-lboost_regex':
AC_CHECK_LIB([boost_regex], [main])
# FIXME: Replace `main' with a function in `-lboost_thread':
AC_CHECK_LIB([boost_thread], [main])
# FIXME: Replace `main' with a function in `-lc':
AC_CHECK_LIB([c], [main])
# FIXME: Replace `main' with a function in `-lgphoto2':
AC_CHECK_LIB([gphoto2], [main])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])
# FIXME: Replace `main' with a function in `-lrt':
AC_CHECK_LIB([rt], [main])
# FIXME: Replace `main' with a function in `-ltiff':
AC_CHECK_LIB([tiff], [main])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h stdint.h stdlib.h string.h strings.h sys/ioctl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([inet_ntoa memset mkdir sqrt strcasecmp strchr strdup strtol])


AC_CONFIG_FILES([Makefile])
AC_OUTPUT
