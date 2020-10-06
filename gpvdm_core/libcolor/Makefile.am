CFLAGS=-Wall -g
AROPT=

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

inc=-I../include/

OBJS=wavelength_to_rgb.o cie.o

all: $(OBJS)
	$(AR) rcs libgpvdm_color.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libcolor/libgpvdm_color.@SLIB_EXT@ ../libgpvdm_color.@SLIB_EXT@
#-cvq

.c.o : 
	$(CC) -c $(lib_type) $(CFLAGS) $(COMPILE_FLAG) $< -o $@  $(inc)

clean:
	rm *.o *.dll *.so *.a *.lib -f
