AC_INIT([gpvdm_gui], [5.3])
AC_CONFIG_SRCDIR([./gui/gpvdm.py])
AC_CANONICAL_TARGET
AC_CANONICAL_HOST
#AM_GNU_GETTEXT([external])

AC_MSG_NOTICE([$target])
AC_MSG_NOTICE([$target_alias])
AC_MSG_NOTICE([$target_cpu])
AC_MSG_NOTICE([$target_vendor])
AC_MSG_NOTICE([$target_os])

AM_INIT_AUTOMAKE([-Wno-portability])

AC_ARG_ENABLE(noman, "used to disable man page generation",
[AM_CONDITIONAL([INCLUDE_MAN],[false])],
[AM_CONDITIONAL([INCLUDE_MAN],[true])]
)

AC_ARG_ENABLE(nodesktop, "used to disable desktop files for free desktop intergration",
[AM_CONDITIONAL([INCLUDE_DESKTOP],[false])],
[AM_CONDITIONAL([INCLUDE_DESKTOP],[true])]
)

AC_ARG_ENABLE(noimages, "used to disable compilation of images",
[AM_CONDITIONAL([INCLUDE_IMAGES],[false])],
[AM_CONDITIONAL([INCLUDE_IMAGES],[true])]
)

AC_ARG_ENABLE(nohtml, "used to disable compilation of html",
[AM_CONDITIONAL([INCLUDE_HTML],[false])],
[AM_CONDITIONAL([INCLUDE_HTML],[true])]
)

AC_ARG_ENABLE(novideo, "used to disable compilation of video",
[AM_CONDITIONAL([INCLUDE_VIDEO],[false])],
[AM_CONDITIONAL([INCLUDE_VIDEO],[true])]
)
 
AC_SUBST(INSTALL_LANG,"")
AC_SUBST(WIN_ICON, "" )
AC_SUBST(WIN_RES_FILE, "" )
#This is a case statment the default is to compile for linux
AS_CASE([$target_os],
  [mingw32], [
], [

AC_MSG_NOTICE([*****************************])
AC_MSG_NOTICE([*Setting up config for linux*])
AC_MSG_NOTICE([*****************************])
AC_SUBST(GPVDM_DIR, "gpvdm" )

AC_ARG_ENABLE(hpc, "used to turn on HPC compatability",
  [
AC_MSG_NOTICE([*****************************])
AC_MSG_NOTICE([*Setting up for HPC         *])
AC_MSG_NOTICE([*****************************])


],[

AC_MSG_NOTICE([Default])
	
]
)


]
)


AC_PROG_RANLIB
AC_PROG_CC


AM_PATH_PYTHON([3.0], [], [AC_MSG_ERROR([You need python installed])])

AC_CHECK_PROG([PKG_HELP2MAN], help2man, help2man,no)
if test "$PKG_HELP2MAN" = "no"; then
	AC_MSG_ERROR([help2man is not installed.])
fi

AC_CHECK_PROG([PKG_XGETTEXT], xgettext, xgettext,no)
if test "$PKG_XGETTEXT" = "no"; then
	AC_MSG_ERROR([xgettext is not installed.])
fi

AC_CHECK_PROG([PKG_RSGV], rsvg-convert, yes,no)
if test "$PKG_RSGV" = "yes"; then
	AC_SUBST(IMAGE_CONVERT_16, "rsvg-convert -w 16 -h 16 $< > ")
	AC_SUBST(IMAGE_CONVERT_32, "rsvg-convert -w 32 -h 32 $< > ")
	AC_SUBST(IMAGE_CONVERT_48, "rsvg-convert -w 48 -h 48 $< > ")
	AC_SUBST(IMAGE_CONVERT_64, "rsvg-convert -w 64 -h 64 $< > ")
	AC_SUBST(IMAGE_CONVERT_128, "rsvg-convert -w 128 -h 128 $< > ")
	AC_SUBST(IMAGE_CONVERT_256, "rsvg-convert -w 256 -h 256 $< > ")
	AC_SUBST(IMAGE_CONVERT_512, "rsvg-convert -w 512 -h 512 $< > ")
	AC_SUBST(IMAGE_CONVERT_FLAG, "rsvg-convert -w 48 -h 32 $< > ")
else
	AC_MSG_WARN([rsvg-convert is not installed, I'm going to look for ImageMagick instead.])
	
	AC_CHECK_PROG([PKG_IMAGEMAGICK], convert, yes,no)
	if test "$PKG_IMAGEMAGICK" = "yes"; then
		AC_SUBST(IMAGE_CONVERT_16, "convert -background white -alpha remove -flatten -alpha off -size 16x16 $< ")
		AC_SUBST(IMAGE_CONVERT_32, "convert -background white -alpha remove -flatten -alpha off -size 32x32 $< ")
		AC_SUBST(IMAGE_CONVERT_64, "convert -background white -alpha remove -flatten -alpha off -size 64x64  $< ")
		AC_SUBST(IMAGE_CONVERT_FLAG, "convert -background white -alpha remove -flatten -alpha off -size 48x32 $< ")
	else
		AC_MSG_ERROR([ImageMagick not installed.])
	fi
fi


AC_CHECK_PROG([PKG_CONFIG], pkg-config, pkg-config,no)
if test "$PKG_CONFIG" = "no"; then
	AC_MSG_ERROR([pkg-config is not installed.])
fi


m4_include([make_files.m4])
m4_include([local_link.m4])

m4_include([ar.m4])


m4_include([config_files.m4])

AC_OUTPUT

AC_MSG_NOTICE([gpvdm_gui build finished (gpvdm_build_finished).])
