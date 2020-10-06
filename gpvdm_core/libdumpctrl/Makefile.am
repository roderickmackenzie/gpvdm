CFLAGS=-Wall -g

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

inc=-I../include/

OBJS=dump_file.o

all: $(OBJS)
	$(AR) rcs libgpvdm_dumpctrl.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libdumpctrl/libgpvdm_dumpctrl.@SLIB_EXT@ ../libgpvdm_dumpctrl.@SLIB_EXT@
#-cvq

.c.o :
	$(CC) -c $(lib_type) $(CPPFLAGS) $(COMPILE_FLAG) $< -o $@  $(inc)

clean:
	rm *.o *.dll *.so *.a *.lib -f
