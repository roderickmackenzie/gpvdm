SOURCEDIR = ../scalable

SOURCES = $(wildcard $(SOURCEDIR)/*.svg)
BUILDDIR = ./

OBJECTS = $(patsubst $(SOURCEDIR)/%.svg,$(BUILDDIR)/%.png,$(SOURCES))

all: $(OBJECTS)
	echo "Images 128x128"

$(OBJECTS) : %.png : $(SOURCEDIR)/%.svg
	@IMAGE_CONVERT_128@ $@

clean:
	rm *.png -f
