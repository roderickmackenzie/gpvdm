OBJS=plugin.o fdtd.o

CFLAGS+= -Wall -g
lib=-L../../
inc=-I../../include/

all: $(OBJS)
	$(CC) -shared $(OBJS) $(lib) @DLL_DEBUG@ -lgpvdm_core  -L../../ -Wall -o plugin.@DLIB_EXT@
	mv plugin.@DLIB_EXT@ ../fdtd.@DLIB_EXT@

.c.o : 
	$(CC) -Wall -g $(inc) @DLL_COMPILE@ @COMPILE_FLAG@ -Wno-error=unused-but-set-variable -Wno-error=unused-variable -c $< -o $@

clean:
	rm *.o *.dll *.so *.a -f
