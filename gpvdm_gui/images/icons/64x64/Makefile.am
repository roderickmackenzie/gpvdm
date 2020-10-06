SOURCEDIR = ../scalable

SOURCES = $(wildcard $(SOURCEDIR)/*.svg)
BUILDDIR = ./

OBJECTS = $(patsubst $(SOURCEDIR)/%.svg,$(BUILDDIR)/%.png,$(SOURCES))

all: $(OBJECTS)
	echo "Images 64x64"

$(OBJECTS) : %.png : $(SOURCEDIR)/%.svg
	@IMAGE_CONVERT_64@ $@

clean:
	rm *.png -f
