OBJS=plugin.o newton.o
inc=-I../../include/
lib=-L../../
CFLAG=-Wall -g

all: $(OBJS)
	$(CC) -shared $(OBJS) $(lib) @DLL_DEBUG@ -lgpvdm_core  -L../../ -Wall -o plugin.@DLIB_EXT@
	mv plugin.@DLIB_EXT@ ../newton_norm.@DLIB_EXT@

.c.o : 
	$(CC) -Wall -g $(inc) $(CPPFLAGS) -Wno-unused-variable @DLL_COMPILE@ @COMPILE_FLAG@ -c $< -o $@


clean:
	rm *.o *.dll *.so -f
