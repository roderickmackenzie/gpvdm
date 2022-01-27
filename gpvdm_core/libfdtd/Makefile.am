CFLAGS=-Wall -g
AROPT=

COMPILE_FLAG=@COMPILE_FLAG@ @OPEN_CL_COMPILE@
LINK_FLAG=@LINK_FLAG@

inc=-I../include/

OBJS=fdtd.o fdtd_config.o fdtd_lambda.o fdtd_init.o fdtd_mesh.o fdtd_power.o fdtd_mem.o fdtd_util.o opencl_init.o fdtd_kernel.o opencl_mem.o fdtd_opencl_util.o fdtd_solve.o fdtd_dump.o fdtd_excitation.o

all: $(OBJS)
	$(AR) rcs libgpvdm_fdtd.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libfdtd/libgpvdm_fdtd.@SLIB_EXT@ ../libgpvdm_fdtd.@SLIB_EXT@
#-cvq

.c.o : 
	$(CC) -c $(lib_type) $(CFLAGS) $(COMPILE_FLAG) $< -o $@  $(inc)

clean:
	rm *.o *.dll *.so *.a *.lib -f
