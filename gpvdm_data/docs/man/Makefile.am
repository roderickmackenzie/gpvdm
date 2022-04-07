HELP2MAN=help2man
#latex2html gpvdm  -html_version 5,math
# -shell-escape is for minted
lib=-L../../
inc=-I../../include/

all: gpvdm.pdf
	echo "Docs built"

gpvdm.pdf: *.tex gpvdm.bib
	pdflatex -shell-escape gpvdm
	makeindex gpvdm
	bibtex gpvdm
	pdflatex -shell-escape  gpvdm
	

clean:
	rm *.pdf -f
	rm core -rf
	rm gui -rf


