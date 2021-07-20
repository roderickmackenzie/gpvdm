OBJS=plugin.o pulse.o

lib=-L../../ -L$(HOME)/windll/compiled_dlls/
inc=-I../../include/

all: $(OBJS)
	$(CC) -shared $(OBJS) $(lib) @DLL_DEBUG@ -lgpvdm_core  -L../../ -o plugin.@DLIB_EXT@ @WARN@
	mv plugin.@DLIB_EXT@ ../time_domain.@DLIB_EXT@
#$(CC) -shared $(OBJS) @DLL_LINKER@ $(lib) @DLL_DEBUG@ -o plugin.@DLIB_EXT@
.c.o : 
	$(CC) -g $(inc) @DLL_COMPILE@ @COMPILE_FLAG@ @WARN@ -c $< -o $@

clean:
	rm *.o *.dll *.so -f

