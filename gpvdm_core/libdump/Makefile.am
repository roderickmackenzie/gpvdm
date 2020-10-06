CFLAGS=-Wall -g

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

inc=-I../include/

OBJS=dump_zxy.o dump_zxy_charge.o dump.o dump_snapshot_utils.o dump_config.o dump_ctrl.o dynamic.o dynamic_init.o dynamic_free.o dump_energy_slice.o dump_map.o dat_file.o dat_file_load.o dump_contacts.o cache.o dynamic_append.o dynamic_dump_j.o dump_zxy_interfaces.o dump_zx.o dat_file_zxyl_light.o

all: $(OBJS)
	$(AR) rcs libgpvdm_dump.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libdump/libgpvdm_dump.@SLIB_EXT@ ../libgpvdm_dump.@SLIB_EXT@
#-cvq

.c.o :
	$(CC) -c $(lib_type) $(CPPFLAGS) $(COMPILE_FLAG) $< -o $@  $(inc)

clean:
	rm *.o *.dll *.so *.a *.lib -f
