AUTOMAKE_OPTIONS = foreign
EXTRA_DIST = config.rpath m4/ChangeLog 
SUBDIRS = @BUILD_DIRS@ @DOC_DIR@

dist_doc_DATA = README.md LICENSE.md

if INCLUDE_DOCS
docsdir = $(docdir)
docs_DATA = docs/man/gpvdm.pdf
endif


clean-local:
	echo "clean"
	@echo "(gpvdm_build_finished)"

all:
	@echo "(gpvdm_build_finished)"

install-data-hook:
	mkdir -p $(DESTDIR)$(datadir)/gpvdm_data
	cp materials $(DESTDIR)$(datadir)/gpvdm_data/materials -rf
	cp spectra $(DESTDIR)$(datadir)/gpvdm_data/spectra -rf
	cp cie_color $(DESTDIR)$(datadir)/gpvdm_data/cie_color -rf
	cp emission $(DESTDIR)$(datadir)/gpvdm_data/emission -rf
	cp atmosphere $(DESTDIR)$(datadir)/gpvdm_data/atmosphere -rf
	cp shape $(DESTDIR)$(datadir)/gpvdm_data/shape -rf
	cp filters $(DESTDIR)$(datadir)/gpvdm_data/filters -rf
	cp components $(DESTDIR)$(datadir)/gpvdm_data/components -rf
	find ./ -name "*.mo" -exec cp --parents {} $(DESTDIR)$(datadir)/gpvdm_data/ ";"
	@echo "(gpvdm_build_finished)"

uninstall-hook:
	echo ""

ACLOCAL_AMFLAGS = -I m4
