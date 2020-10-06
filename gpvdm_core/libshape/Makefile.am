inc=-I../include/
OBJS=shape.o

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

all: $(OBJS)
	$(AR) cr libgpvdm_shape.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libshape/libgpvdm_shape.@SLIB_EXT@ ../libgpvdm_shape.@SLIB_EXT@
#-cvq

.c.o:
	$(CC) -c $(CPPFLAGS) $(COMPILE_FLAG) $(inc) $< -o $@  

clean:
	rm *.o *.dll *.so *.a *.lib -f
