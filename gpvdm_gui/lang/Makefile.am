OBJS= ./zh_CN/LC_MESSAGES/gpvdm.mo ./pt_BR/LC_MESSAGES/gpvdm.mo ./hi_IN/LC_MESSAGES/gpvdm.mo ./el_GR/LC_MESSAGES/gpvdm.mo ./pt_PT/LC_MESSAGES/gpvdm.mo
#./de_DE/LC_MESSAGES/gpvdm.mo  ./fa_IR/LC_MESSAGES/gpvdm.mo ./ru_RU/LC_MESSAGES/gpvdm.mo ./hi_IN/LC_MESSAGES/gpvdm.mo ./fr_FR/LC_MESSAGES/gpvdm.mo
# ./fr_FR/LC_MESSAGES/gpvdm.mo
all: $(OBJS)
	xgettext --language=Python --keyword=_ --output=gpvdm.pot `find ../../ \( -name "*.py" -or -name "*.c" \)|grep -v pub` --msgid-bugs-address=roderick.mackenzie@nottingham.ac.uk  --copyright-holder="Roderick C. I. MacKenzie" --package-name=gpvdm --package-version=5.0

./%/LC_MESSAGES/gpvdm.mo : %.po
	mkdir $(basename $<)
	mkdir $(basename $<)/LC_MESSAGES
	cp $(basename $<).bib $(basename $<)/LC_MESSAGES/ref.bib
	msgfmt  $< -o $(basename $<)/LC_MESSAGES/gpvdm.mo


clean:
	rm de_DE zh_CN fa_IR ru_RU hi_IN fr_FR pt_BR hi_IN el_GR pt_PT -rf
	rm *.mo -f
	rm gpvdm.pot -f
