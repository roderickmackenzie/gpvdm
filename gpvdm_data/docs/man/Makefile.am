HELP2MAN=help2man

lib=-L../../
inc=-I../../include/

all: gpvdm.pdf
	echo "Docs built"

gpvdm.pdf: *.tex gpvdm.bib
	pdflatex gpvdm
	bibtex gpvdm
	pdflatex gpvdm
clean:
	rm *.pdf -f
	rm core -rf
	rm gui -rf


