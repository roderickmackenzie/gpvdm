# 
# General-purpose Photovoltaic Device Model gpvdm.com- a drift diffusion
# base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
# The model can simulate OLEDs, Perovskite cells, and OFETs.
# 
# Copyright (C) 2012-2019 Roderick C. I. MacKenzie info at gpvdm dot com
# 
# https://www.gpvdm.com
# 
# 
# This program is free software; you can redistribute it and/or modify it
# under the terms and conditions of the GNU Lesser General Public License,
# version 2.1, as published by the Free Software Foundation.
# 
# This program is distributed in the hope it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
# 


AC_INIT([gpvdm_core], [5.3])
AC_CONFIG_SRCDIR([./src/main.c])
AC_CANONICAL_TARGET
AC_CANONICAL_HOST
#AM_GNU_GETTEXT([external])

AC_MSG_NOTICE([$target])
AC_MSG_NOTICE([$target_alias])
AC_MSG_NOTICE([$target_cpu])
AC_MSG_NOTICE([$target_vendor])
AC_MSG_NOTICE([$target_os])

AC_SUBST(WARN, ["-Wall -Wno-format-overflow"])

AC_ARG_ENABLE(nodevicelib, "used to disable the device lib",
[AM_CONDITIONAL([INCLUDE_DEVICELIB],[false])],
[AM_CONDITIONAL([INCLUDE_DEVICELIB],[true])]
)

AC_ARG_ENABLE(nocluster, "used to disable the cluster",
[AM_CONDITIONAL([INCLUDE_CLUSTER],[false])],
[AM_CONDITIONAL([INCLUDE_CLUSTER],[true])]
)


AC_ARG_ENABLE(noplots, "used to disable plot files",
[AM_CONDITIONAL([INCLUDE_PLOTS],[false])],
[AM_CONDITIONAL([INCLUDE_PLOTS],[true])]
)


AC_ARG_ENABLE(noman, "used to disable man page generation",
[AM_CONDITIONAL([INCLUDE_MAN],[false])],
[AM_CONDITIONAL([INCLUDE_MAN],[true])]
)


AC_ARG_ENABLE(useopencl, "Use this to enable OpenCL for the FDTD code",
[
AC_SUBST(OPEN_CL_LINK, "-lOpenCL")
AC_SUBST(OPEN_CL_COMPILE, "-D use_open_cl")
],
[
AC_SUBST(OPEN_CL_LINK, "")
AC_SUBST(OPEN_CL_COMPILE, "")
]
)

AC_SUBST(WIN_ICON, "" )
AC_SUBST(WIN_RES_FILE, "" )
AC_SUBST(PNG_LIB,[""])
AC_SUBST(PNG_HEADER,[""])
#This is a case statment the default is to compile for linux
AS_CASE([$target_os],
  [mingw32], [
], [

AC_MSG_NOTICE([*****************************])
AC_MSG_NOTICE([*Setting up config for linux*])
AC_MSG_NOTICE([*****************************])
AC_SUBST(GPVDM_DIR, "gpvdm" )
AC_SUBST(LINK_FLAG, "")
AC_CHECK_LIB(zip, [main], [], [AC_MSG_ERROR([libzip is needed but not found.])])

AC_CHECK_HEADERS([umfpack.h],[],[
AC_CHECK_HEADERS([suitesparse/umfpack.h],[],[
AC_MSG_ERROR([suitesparse/umfpack.h or umfpack.h is needed but not found])
#AC_CHECK_HEADERS([gsl/gsl_multimin.h],[],[AC_MSG_ERROR([gsl/gsl_multimin.h is needed but not found])])
#AC_SUBST(LINK_GSL_BLAS, ["-lgsl -lgslcblas"])

])
])

AC_SUBST(SLIB_EXT, "a")
AC_SUBST(DLIB_EXT, "so")
AC_SUBST(EXE_EXT, "")


AC_SUBST(DLL_LINKER, ["`dpkg-buildflags --get LDFLAGS` -Wl,-soname,plugin.so"])
AC_SUBST(DLL_COMPILE, ["-fPIC"])
AC_SUBST(DLL_DEBUG, ["-Wall -g "])
AC_SUBST(SRC_DEBUG, [" -g "])

#-g -fsanitize=address -fno-omit-frame-pointer
# -fsanitize=address -fno-omit-frame-pointer

AC_ARG_ENABLE(hpc, "used to turn on HPC compatability",
  [
AC_MSG_NOTICE([*****************************])
AC_MSG_NOTICE([*Setting up for HPC         *])
AC_MSG_NOTICE([*****************************])

AC_SUBST(COMPILE_FLAG, " -g -fPIC " )
AC_SUBST(LIBS, " -lumfpack -lm -rdynamic -export-dynamic -ldl -lzip -lz -lpng -L/home/steve/gpvdm/cluster_libs/")
#-lgsl -lgslcblas
#-lmatheval -g -L/home/steve/rm/build/libmatheval-1.1.11/lib/.libs/

],[

	AC_SUBST(COMPILE_FLAG, " -g -fPIC " )
	AC_SUBST(LIBS, "-lumfpack  -lm -rdynamic -export-dynamic -ldl -lzip -lz -lpng -g `pkg-config --libs dbus-1` ")
	AC_SUBST(INTERNET_LIB,["-lcurl"])
 	#-I/usr/lib/x86_64-linux-gnu/openmpi/include/ -lmpi -ldmumps for mumps
	#-lgsl -lgslcblas
	
]
)


]
)


AC_PROG_RANLIB
AM_INIT_AUTOMAKE([-Wno-portability])
AC_PROG_CC

AC_ARG_ENABLE(hpc, "used to turn on HPC compatability",
  [ ],[

	AM_PATH_PYTHON([3.0], [], [AC_MSG_ERROR([You need python installed])])
	
	AC_CHECK_PROG([PKG_HELP2MAN], help2man, help2man,no)
	if test "$PKG_HELP2MAN" = "no"; then
		AC_MSG_ERROR([help2man is not installed.])
	fi

	AC_CHECK_PROG([PKG_XGETTEXT], xgettext, xgettext,no)
	if test "$PKG_XGETTEXT" = "no"; then
		AC_MSG_ERROR([xgettext is not installed.])
	fi

]
)

AC_CHECK_HEADERS([zip.h],[],[AC_MSG_ERROR([zip.h is needed but not found])])

AC_CHECK_PROG([PKG_CONFIG], pkg-config, pkg-config,no)
if test "$PKG_CONFIG" = "no"; then
	AC_MSG_ERROR([pkg-config is not installed.])
fi




#i?86
#x86_64

m4_include([make_files.m4])
m4_include([local_link.m4])
m4_include([gpvdm_core_lib.m4])
m4_include([includes.m4])


m4_include([ar.m4])

AC_CHECK_HEADERS(math.h)
AC_FUNC_MALLOC
AC_CHECK_FUNCS([sqrt])


#AC_CONFIG_FILES

m4_include([config_files.m4])

AC_OUTPUT

AC_MSG_NOTICE([gpvdm_core build finished. (gpvdm_build_finished)])
