HELP2MAN=help2man
#latex2html gpvdm  -html_version 5,math
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


