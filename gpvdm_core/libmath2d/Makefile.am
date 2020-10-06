inc=-I../include/
OBJS=data2d.o math_2d_free.o math_2d_init.o math_2d_malloc.o math_2d_png_image.o math_2d_dump.o math_2d_to_trianges_simple.o math_2d_to_trianges_front.o math_2d_min_max.o math_2d_math.o

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

all: $(OBJS)
	$(AR) cr libgpvdm_math2d.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libi/libgpvdm_math2d.@SLIB_EXT@ ../libgpvdm_math2d.@SLIB_EXT@
#-cvq

.c.o:
	$(CC) -c $(CPPFLAGS) $(COMPILE_FLAG) $(inc) $< -o $@  

clean:
	rm *.o *.dll *.so *.a *.lib -f
