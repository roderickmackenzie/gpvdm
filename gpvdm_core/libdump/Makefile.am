CFLAGS=-Wall -g

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

inc=-I../include/

OBJS=dump_zxy.o dump_zxy_charge.o dump.o dump_config.o dump_ctrl.o dynamic.o dynamic_init.o dynamic_free.o dump_energy_slice.o dump_map.o dat_file.o dat_file_load.o dump_contacts.o dynamic_append.o dynamic_dump_j.o dump_zxy_interfaces.o dump_zx.o dynamic_dump_thermal.o dat_file_add_info.o dat_file_init.o snapshots.o dat_file_add_zxy_float.o dat_file_add_zxyt_long_double.o dat_file_add_zxy_double.o dat_file_add_zxy.o dat_file_write_zxy_snapshot_as_slices.o dump_singlet.o

all: $(OBJS)
	$(AR) rcs libgpvdm_dump.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libdump/libgpvdm_dump.@SLIB_EXT@ ../libgpvdm_dump.@SLIB_EXT@
#-cvq

.c.o :
	$(CC) -c $(lib_type) $(CPPFLAGS) $(COMPILE_FLAG) -Wall $< -o $@  $(inc)

clean:
	rm *.o *.dll *.so *.a *.lib -f
