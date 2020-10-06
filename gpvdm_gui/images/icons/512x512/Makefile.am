SOURCEDIR = ../scalable

SOURCES = $(wildcard $(SOURCEDIR)/*.svg)
BUILDDIR = ./

OBJECTS = $(patsubst $(SOURCEDIR)/%.svg,$(BUILDDIR)/%.png,$(SOURCES))

all: $(OBJECTS)
	echo "Images 512x512"

$(OBJECTS) : %.png : $(SOURCEDIR)/%.svg
	@IMAGE_CONVERT_512@ $@

clean:
	rm *.png -f
