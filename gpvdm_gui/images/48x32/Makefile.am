SOURCEDIR = ../flags

SOURCES = $(wildcard $(SOURCEDIR)/*.svg)
BUILDDIR = ./

OBJECTS = $(patsubst $(SOURCEDIR)/%.svg,$(BUILDDIR)/%.png,$(SOURCES))

all: $(OBJECTS)
	echo "Images 48x32"

$(OBJECTS) : %.png : $(SOURCEDIR)/%.svg
	@IMAGE_CONVERT_FLAG@ $@

clean:
	rm *.png -f

