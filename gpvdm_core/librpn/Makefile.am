inc=-I../include/
OBJS=edge.o eval.o function.o opp.o rpn.o stack.o vars.o

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

all: $(OBJS)
	$(AR) cr libgpvdm_rpn.@SLIB_EXT@ $(OBJS)
	ln -s -f ./librpn/libgpvdm_rpn.@SLIB_EXT@ ../libgpvdm_rpn.@SLIB_EXT@
#-cvq

.c.o:
	$(CC) -c $(CPPFLAGS) $(COMPILE_FLAG) $(inc) $< -o $@  

clean:
	rm *.o *.dll *.so *.a *.lib -f
