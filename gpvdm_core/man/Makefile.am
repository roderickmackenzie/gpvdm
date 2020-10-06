HELP2MAN=help2man

lib=-L../../
inc=-I../../include/

all: gpvdm_core.1
	echo "Man page built"



gpvdm_core.1: ../gpvdm_core@EXE_EXT@
	cd ..; $(HELP2MAN) --output=./man/gpvdm_core.1 --name="A solar cell device model" ./gpvdm_core@EXE_EXT@

clean:
	rm *.1 -f

