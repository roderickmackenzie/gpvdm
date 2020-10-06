inc=-I../include/
OBJS=simplex_contract.o simplex_init.o simplex_shrink.o simplex_dump.o simplex_reflect.o simplex_utils.o simplex_expand.o  simplex_run.o

all: $(OBJS)
#$(CC) -shared $(OBJS) -o libgpvdm_simplex.@DLIB_EXT@
#ln -s -f ./libsimplex/libgpvdm_simplex.@DLIB_EXT@ ../libgpvdm_simplex.@DLIB_EXT@
	$(AR) cr libgpvdm_simplex.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libsimplex/libgpvdm_simplex.@SLIB_EXT@ ../libgpvdm_simplex.@SLIB_EXT@
#-cvq

.c.o:
	$(CC) -c $(CPPFLAGS) @COMPILE_FLAG@ $(inc) $< -o $@ 

clean:
	rm *.o *.dll *.so *.a *.lib -f
