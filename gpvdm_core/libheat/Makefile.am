CFLAGS=-Wall -g
AROPT=

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

inc=-I../include/

OBJS=heat_config.o heat_free.o heat_malloc.o heat_cpy.o heat_utils.o hydrodynamic_solve.o heat_dump.o heat_init.o heat_mesh.o hydrodynamic_mesh_check.o  hydrodynamic_update_heat.o heat_solve.o lattice_solve.o heat_set_initial_distribution.o heat_material.o heat_stats.o


all: $(OBJS)
	$(AR) rcs libgpvdm_heat.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libheat/libgpvdm_heat.@SLIB_EXT@ ../libgpvdm_heat.@SLIB_EXT@
#-cvq

.c.o : 
	$(CC) -c $(lib_type) $(CFLAGS) $(COMPILE_FLAG) $< -o $@  $(inc)

clean:
	rm *.o *.dll *.so *.a *.lib -f
