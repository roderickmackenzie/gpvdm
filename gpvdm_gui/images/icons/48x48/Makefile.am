SOURCEDIR = ../scalable

SOURCES = $(wildcard $(SOURCEDIR)/*.svg)
BUILDDIR = ./

OBJECTS = $(patsubst $(SOURCEDIR)/%.svg,$(BUILDDIR)/%.png,$(SOURCES))

all: $(OBJECTS)
	echo "Images 48x48"

$(OBJECTS) : %.png : $(SOURCEDIR)/%.svg
	@IMAGE_CONVERT_48@ $@

clean:
	rm *.png -f
