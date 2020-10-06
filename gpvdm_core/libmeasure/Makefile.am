COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

inc=-I../include/
OBJS=exp.o measure.o

all: $(OBJS)
	$(AR) rcs libgpvdm_measure.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libmeasure/libgpvdm_measure.@SLIB_EXT@ ../libgpvdm_measure.@SLIB_EXT@
#-cvq

.c.o:
	$(CC) -c $(lib_type) $(CPPFLAGS) $(COMPILE_FLAG) $< -o $@  $(inc)

clean:
	rm *.o *.dll *.so *.a *.lib -f
