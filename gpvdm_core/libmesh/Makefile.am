CFLAGS=-Wall -g
AROPT=

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

inc=-I../include/
OBJS=time.o mesh.o dim.o remesh.o mesh_obj.o dim_light.o dim_heat.o dim_zx_epitaxy.o

all: $(OBJS)
	$(AR) rcs libgpvdm_mesh.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libmesh/libgpvdm_mesh.@SLIB_EXT@ ../libgpvdm_mesh.@SLIB_EXT@
#-cvq

.c.o : 
	$(CC) -c $(lib_type) $(CFLAGS) $(COMPILE_FLAG) $< -o $@  $(inc)

clean:
	rm *.o *.dll *.so *.a *.lib -f
