inc=-I../include/
OBJS=i.o i_mem.o i_math.o math_xy_calculus.o math_xy_load_save.o math_xy_peak.o math_xy_dft.o math_xy_delta.o math_xy_smooth.o math_interpolate.o

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

all: $(OBJS)
	$(AR) cr libgpvdm_i.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libi/libgpvdm_i.@SLIB_EXT@ ../libgpvdm_i.@SLIB_EXT@
#-cvq

.c.o:
	$(CC) -c $(CPPFLAGS) $(COMPILE_FLAG) $(inc) $< -o $@  

clean:
	rm *.o *.dll *.so *.a *.lib -f
