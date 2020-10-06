inc=-I../include/ -I/usr/include/suitesparse/ #last line is for debian compatibility
OBJS=advmath.o rand.o vec.o triangle.o triangles.o object.o vectors.o basic_math.o objects.o triangles_flags.o triangles_min_max.o triangles_roughness.o triangles_reduce.o triangles_reduce_nodes.o

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

all: $(OBJS)
	$(AR) cr libgpvdm_basicmath.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libbasicmath/libgpvdm_basicmath.@SLIB_EXT@ ../libgpvdm_basicmath.@SLIB_EXT@
#-cvq

.c.o:
	$(CC) -c $(CPPFLAGS) $(COMPILE_FLAG) $(inc) -O5 $< -o $@  

clean:
	rm *.o *.dll *.so *.a *.lib -f
