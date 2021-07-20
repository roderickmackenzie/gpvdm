AUTOMAKE_OPTIONS = foreign
EXTRA_DIST = config.rpath m4/ChangeLog  materials
SUBDIRS = @BUILD_DIRS@

bin_PROGRAMS=gpvdm_core
gpvdm_core_SOURCES=



inp_templatedir = $(pkgdatadir)/inp_template
inp_template_DATA = inp_template/*.inp

inp_template_shapedir = $(pkgdatadir)/inp_template/shape/
inp_template_shape_DATA = inp_template/shape/*.inp inp_template/shape/*.py inp_template/shape/*.png

if INCLUDE_DEVICELIB
device_libdir = $(pkgdatadir)/device_lib
device_lib_DATA=device_lib/*.gpvdm

#device_lib_maeckel_et_aldir = $(pkgdatadir)/device_lib/maeckel_et_al/
#device_lib_maeckel_et_al_DATA=device_lib/maeckel_et_al/*.gpvdm
endif

if INCLUDE_CLUSTER
clusterdir = $(pkgdatadir)/cluster_
cluster_DATA=cluster_/*.zip
endif

if INCLUDE_PLOTS
plotdir = $(pkgdatadir)/plot
plot_DATA=plot/*.plot
endif

pluginsdir = $(pkglibdir)/plugins
plugins_DATA=plugins/*.@DLIB_EXT@

dist_doc_DATA = README.md LICENSE.txt


if INCLUDE_MAN
man1_MANS = man/gpvdm_core.1
endif


clean-local:
	rm *.dat *.o ./plugins/*.so ./plugins/*.dll  *.log *.a  *.lib *.exe *.chk *.oplot -f
	rm autom4te.cache -rf
	rm gpvdm_core -f
	rm gpvdm_core.@DLIB_EXT@ -f
	find . -type f -name '.~lock*' -delete
	
	
gpvdm_core$(EXEEXT):
	@printf ""


install-data-hook:
	echo "lang"
	cp libgpvdm_core.@DLIB_EXT@ $(DESTDIR)$(bindir)/
	@echo "(gpvdm_build_finished)"

uninstall-hook:
	-rm $(DESTDIR)$(pkgdatadir) -rf
	-rm $(DESTDIR)$(bindir)/gpvdm_core

ACLOCAL_AMFLAGS = -I m4
