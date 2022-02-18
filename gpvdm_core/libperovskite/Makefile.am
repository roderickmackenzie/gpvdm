AROPT=

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

inc=-I../include/
OBJS=perovskite.o perovskite_build_matrix.o perovskite_load.o

all: $(OBJS)
	$(AR) rcs libgpvdm_perovskite.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libperovskite/libgpvdm_perovskite.@SLIB_EXT@ ../libgpvdm_perovskite.@SLIB_EXT@
#-cvq

.c.o:
	$(CC) -c $(lib_type) $(CPPFLAGS) $(COMPILE_FLAG) $< -o $@  $(inc)

clean:
	rm *.o *.dll *.so *.a *.lib -f

