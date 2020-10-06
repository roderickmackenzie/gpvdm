

all: 
	@echo "[Desktop Entry]" > gpvdm.desktop
	@echo "Name=gpvdm" >> gpvdm.desktop
	@echo "Icon=$(pkgdatadir)/images/application-gpvdm.svg" >> gpvdm.desktop
	@echo "Type=Application" >> gpvdm.desktop
	@echo "MimeType=application/gpvdm;" >> gpvdm.desktop
	@echo "Exec=$(DESTDIR)$(bindir)/gpvdm %F" >> gpvdm.desktop
	@echo "Terminal=true" >> gpvdm.desktop
	@echo "Categories=GTK;GNOME;Science;" >> gpvdm.desktop
	@echo


clean:
	rm gpvdm.desktop -f

install-desktop:
	mkdir -p $(DESTDIR)$(datadir)/applications
	cp gpvdm.desktop $(DESTDIR)$(datadir)/applications/gpvdm.desktop

uninstall:
	pwd
	-rm $(DESTDIR)$(datadir)/icons/hicolor/scalable/mimetypes/simulation-gpvdm.svg
	-rm $(DESTDIR)$(datadir)/mime/packages/gpvdm-gpvdm.xml
	-rm $(DESTDIR)$(datadir)/applications/gpvdm.desktop
