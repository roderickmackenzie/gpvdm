inc=-I../include/
OBJS=server2.o server2_config.o

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

all: $(OBJS)
	$(AR) rcs libgpvdm_server.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libserver/libgpvdm_server.@SLIB_EXT@ ../libgpvdm_server.@SLIB_EXT@
#-cvq

.c.o:
	$(CC) -c $(lib_type) $(COMPILE_FLAG) $< -o $@  $(inc)

clean:
	rm *.o *.dll *.so *.a *.lib -f
