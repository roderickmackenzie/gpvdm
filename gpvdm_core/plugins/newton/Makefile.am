OBJS=plugin.o newton.o update.o offset.o memory.o error.o
lib=-L../../
inc=-I../../include/

all: $(OBJS)
	$(CC) -shared $(OBJS) $(lib) @DLL_DEBUG@ -lgpvdm_core  -L../../ -Wall -o plugin.@DLIB_EXT@
	mv plugin.@DLIB_EXT@ ../newton.@DLIB_EXT@
#$(CC) -shared $(OBJS) @DLL_LINKER@ $(lib) @DLL_DEBUG@ -o plugin.@DLIB_EXT@

.c.o : 
	$(CC) -Wall -g $(inc) $(CPPFLAGS) @DLL_COMPILE@ @COMPILE_FLAG@ -c $< -o $@

clean:
	rm *.o *.dll *.so -f
