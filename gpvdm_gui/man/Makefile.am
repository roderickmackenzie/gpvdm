HELP2MAN=help2man

lib=-L../../
inc=-I../../include/

all: gpvdm_gui.1
	echo "Man page built"


gpvdm_gui.1: ../gpvdm_gui
	cd ..; $(HELP2MAN) --output=./man/gpvdm_gui.1 --name="GUI for gpvdm_core, a solar cell device model" ./gpvdm_gui

clean:
	rm *.1 -f

