inc=-I../include/ -I/usr/include/suitesparse/
OBJS=2d_long_double.o memory.o memory_basic.o zxy_long_double.o matrix.o matrix_free.o matrix_cpy.o zxy_long_double_complex.o zy_long_double.o zxy_int.o light_l_long_double.o light_y_long_double.o light_zxyl_long_double.o light_zxyl_float.o light_zxyl_long_double_complex.o light_zxy_long_double.o zx_long_double.o light_zxy_p_object.o heat_zxy_long_double.o zx_epitaxy_int.o zx_int.o 3d_int.o 4d_long_double.o srh_long_double.o zy_int.o matrix_solver_memory.o 1d.o 2d.o 3d.o 4d.o zxy_p_object.o zx_layer_p_object.o light_zxyl_double.o light_zxyl_float_complex.o light_zxyl_float.o light_zxy_float.o

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

all: $(OBJS)
	$(AR) cr libgpvdm_memory.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libmemory/libgpvdm_memory.@SLIB_EXT@ ../libgpvdm_memory.@SLIB_EXT@
#-cvq

.c.o:
	$(CC) -c $(CPPFLAGS) $(COMPILE_FLAG) $(inc) $< -o $@  

clean:
	rm *.o *.dll *.so *.a *.lib -f
