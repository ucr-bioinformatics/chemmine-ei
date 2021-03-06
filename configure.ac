#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([ei], [1.0])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])


# Checks for programs.
AC_PROG_RANLIB
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

AC_ARG_WITH([eir],AS_HELP_STRING([--with-eir],[Build and install EI R extension]))
AS_IF([ test "x$with_eir" = "xyes"],[
	AC_PATH_PROG([R_PATH],[R])
	if test  -z "$ac_cv_path_R_PATH"; then
		echo "could not find R binary"
		exit 1
	fi
	: ${R_HOME=`R RHOME`}
	if test -z "${R_HOME}"; then
	 echo "could not determine R_HOME"
	 exit 1
	fi
])
AM_CONDITIONAL([BUILD_EIR], [test "x$with_eir" = "xyes"])


AC_ARG_WITH([chemminerpp],AS_HELP_STRING([--with-chemminerpp],[Build and install ChemmineRpp R extension]))
AS_IF([ test "x$with_chemminerpp" = "xyes"],[
	AC_PATH_PROG([R_PATH],[R])
	if test  -z "$ac_cv_path_R_PATH"; then
		echo "could not find R binary"
		exit 1
	fi
	: ${R_HOME=`R RHOME`}
	if test -z "${R_HOME}"; then
	 echo "could not determine R_HOME"
	 exit 1
	fi
])
AM_CONDITIONAL([BUILD_CHEMMINER], [test "x$with_chemminerpp" = "xyes"])


AM_PATH_PYTHON([2.5])
AX_PKG_SWIG([],[],[AC_MSG_FAILURE])
AX_SWIG_ENABLE_CXX
AX_SWIG_PYTHON

# Checks for libraries.
AC_ARG_WITH([openbabel],AS_HELP_STRING([--without-openbabel],[Build without OpenBabel]))
AS_IF([test "x$with_openbabel" != "xno"],[
	AC_LANG_PUSH([C++])
		 #AC_CHECK_HEADERS([openbabel/obconversion.h openbable/mol.h openbable/obiter.h],[],[AC_MSG_FAILURE])
		 PKG_CHECK_MODULES([OPENBABEL],[openbabel-2.0])
		 AC_CHECK_LIB([openbabel],[acos],[:],[AC_MSG_FAILURE])
	AC_LANG_POP([C++])
])
AM_CONDITIONAL([HAVE_OPENBABEL], [test "x$with_openbabel" != "xno"])

AC_CHECK_LIB([f2c], [main],[:],[AC_MSG_FAILURE],[-static])
AC_CHECK_LIB([m], [pow],[],[AC_MSG_FAILURE])
AC_CHECK_LIB([readline], [rl_complete],[],[AC_MSG_FAILURE])
AC_CHECK_LIB([db], [db_create],[],[AC_MSG_FAILURE])

AC_ARG_WITH([lshkit],AS_HELP_STRING([--without-lshkit],[Build without lshkit]))
AS_IF([test "x$with_lshkit" != "xno"],[
	AX_BOOST_BASE
	AX_BOOST_PROGRAM_OPTIONS
	AX_BOOST_THREAD

	AC_LANG_PUSH([C++])
	AC_CHECK_HEADERS([boost/threadpool.hpp],[],[AC_MSG_FAILURE])
	AC_CHECK_HEADERS([gsl/gsl_integration.h],[],[AC_MSG_FAILURE])
	AC_CHECK_HEADERS([gsl/gsl_multifit.h],[],[AC_MSG_FAILURE])
	AC_LANG_POP([C++])

	AC_CHECK_LIB([gslcblas],[cblas_ctpmv],[],[AC_MSG_FAILURE])
	AC_CHECK_LIB([gsl],[gsl_matrix_set],[],[AC_MSG_FAILURE],[-lgslcblas])
])
AM_CONDITIONAL([BUILD_LSHKIT], [test "x$with_lshkit" != "xno"])


# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h stdint.h stdlib.h string.h sys/time.h unistd.h])
AC_CHECK_HEADERS([db.h],[],[AC_MSG_FAILURE])


# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([floor ftruncate gettimeofday memset pow sqrt])

AC_CONFIG_FILES([Makefile
					  man/Makefile
					  src/Makefile
                 src/atompair/Makefile
                 src/atompair/python/Makefile
                 src/atompair/R/Makefile
                 src/embedder/Makefile
                 src/euclidean-search/Makefile
                 src/lshkit/Makefile
                 src/lshkit/src/Makefile
                 src/lshkit/tools/Makefile
					  src/eutils/Makefile
					  src/ei_fmcs/Makefile
					  src/ei_R/Makefile
                 src/matrix/Makefile])


AC_OUTPUT
