CFLAGS=-Wall -g
AROPT=

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

inc=-I../include/ -I/usr/include/suitesparse/
OBJS=circuit.o circuit_memory.o circuit_load.o circuit_dump.o cal_photogen.o cal_resistance.o circuit_build.o circuit_util.o circuit_transfer_to_electrical.o nodes.o links.o component.o circuit_init.o circuit_cpy.o circuit_spm.o circuit_dump_snapshot.o

all: $(OBJS)
	$(AR) rcs libgpvdm_circuit.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libcircuit/libgpvdm_circuit.@SLIB_EXT@ ../libgpvdm_circuit.@SLIB_EXT@
#-cvq

.c.o : 
	$(CC) -c $(lib_type) $(CFLAGS) $(COMPILE_FLAG) $< -o $@  $(inc)

clean:
	rm *.o *.dll *.so *.a *.lib -f
