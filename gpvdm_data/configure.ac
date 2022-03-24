AC_INIT([gpvdm_data], [5.3])
AC_CONFIG_SRCDIR([./docs/man/gpvdm.tex])
AC_CANONICAL_TARGET
AC_CANONICAL_HOST
#AM_GNU_GETTEXT([external])

#AC_CONFIG_FILES
m4_include([config_files.m4])


AC_ARG_ENABLE(nodocs, "used to disable compilation of docs",
[AM_CONDITIONAL([INCLUDE_DOCS],[false])],
[AM_CONDITIONAL([INCLUDE_DOCS],[true])]
)
AC_MSG_NOTICE([$target])
AC_MSG_NOTICE([$target_alias])
AC_MSG_NOTICE([$target_cpu])
AC_MSG_NOTICE([$target_vendor])
AC_MSG_NOTICE([$target_os])

AM_INIT_AUTOMAKE([-Wno-portability])


#This is a case statment the default is to compile for linux
AS_CASE([$target_os],
  [mingw32], [
], [

AC_MSG_NOTICE([*****************************])
AC_MSG_NOTICE([*Setting up config for linux*])
AC_MSG_NOTICE([*****************************])
AC_SUBST(GPVDM_DIR, "gpvdm" )


]
)


AC_PROG_RANLIB
AC_PROG_CC


AM_PATH_PYTHON([3.0], [], [AC_MSG_ERROR([You need python installed])])

AC_CHECK_PROG([PKG_HELP2MAN], help2man, help2man,no)
if test "$PKG_HELP2MAN" = "no"; then
	AC_MSG_ERROR([help2man is not installed.])
fi

AC_CHECK_PROG([PKG_PDFLATEX], pdflatex, pdflatex,no)
if test "$PKG_PDFLATEX" = "no"; then
	AC_MSG_WARN([pdflatex is not installed.])
	AC_SUBST(DOC_DIR, "" )
else
	AC_SUBST(DOC_DIR, "docs/man" )
fi

AC_CHECK_PROG([PKG_BIBTEX], bibtex, bibtex,no)
if test "$PKG_PDFLATEX" = "no"; then
	AC_MSG_WARN([bibtex is not installed.])
fi


m4_include([make_files.m4])
m4_include([local_link.m4])

m4_include([ar.m4])

AC_OUTPUT

AC_MSG_NOTICE([gpvdm_data build finished. (gpvdm_build_finished)])
