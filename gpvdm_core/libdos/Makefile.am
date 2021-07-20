AROPT=

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

inc=-I../include/
OBJS=dos_an.o dos.o gendosfdgaus.o checksum.o hash.o md5.o dos_free.o dos_trap.o dos_config_load.o dos_init.o dos_mem_free.o dos_cpy.o dos_cache.o dos_ion.o

all: $(OBJS)
	$(AR) rcs libgpvdm_dos.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libdos/libgpvdm_dos.@SLIB_EXT@ ../libgpvdm_dos.@SLIB_EXT@
#-cvq

.c.o:
	$(CC) -c $(lib_type) $(CPPFLAGS) -D dos_bin $(COMPILE_FLAG) -Wall $< -o $@  $(inc)

clean:
	rm *.o *.dll *.so *.a *.lib -f

