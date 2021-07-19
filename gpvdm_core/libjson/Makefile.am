CFLAGS=-Wall -g
OBJS=json.o json_search.o json_free.o json_cpy.o json_chk_sum.o json_init.o json_dump.o json_string.o

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

inc=-I../include/

all: $(OBJS)
	$(AR) cr libgpvdm_json.@SLIB_EXT@ $(OBJS)
	ln -s -f ./lib/libgpvdm_json.@SLIB_EXT@ ../libgpvdm_json.@SLIB_EXT@

.c.o:
	$(CC) -c $(CPPFLAGS) $(lib_type) $(CFLAGS) $(COMPILE_FLAG) $(inc) $< -o $@  

clean:
	rm *.o *.dll *.so *.a *.lib -f
