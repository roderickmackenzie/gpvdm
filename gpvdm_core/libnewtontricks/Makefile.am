AROPT=

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

inc=-I../include/
OBJS=perovskite.o externalv.o misc.o load_save.o newton_voc.o hash_dir.o state_vector.o newton_state.o newton_state_complex.o

all: $(OBJS)
	$(AR) rcs libgpvdm_newtontricks.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libnewtontricks/libgpvdm_newtontricks.@SLIB_EXT@ ../libgpvdm_newtontricks.@SLIB_EXT@
#-cvq

.c.o:
	$(CC) -c $(lib_type) $(CPPFLAGS) $(COMPILE_FLAG) $< -o $@  $(inc)

clean:
	rm *.o *.dll *.so *.a *.lib -f

