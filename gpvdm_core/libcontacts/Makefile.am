inc=-I../include/
OBJS=contacts.o contacts_load.o contact.o contacts_cpy.o contacts_free.o

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

all: $(OBJS)
	$(AR) cr libgpvdm_contacts.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libcontacts/libgpvdm_contacts.@SLIB_EXT@ ../libgpvdm_contacts.@SLIB_EXT@
#-cvq

.c.o:
	$(CC) -c $(CPPFLAGS) $(COMPILE_FLAG) $(inc) $< -o $@  

clean:
	rm *.o *.dll *.so *.a *.lib -f
