inc=-I../include/ -I/usr/include/suitesparse/ #last line is for debian compatibility
OBJS=log.o util.o inp.o cal_path.o newton_interface.o timer.o gui_hooks.o startstop.o hard_limit.o epitaxy_materials.o errors.o plot.o  initial.o newton_update.o solver_interface.o util_str.o list.o zip_buffer.o complex_solver_interface.o util_units.o

COMPILE_FLAG=@COMPILE_FLAG@
LINK_FLAG=@LINK_FLAG@

all: $(OBJS)
	$(AR) cr libgpvdm_lib.@SLIB_EXT@ $(OBJS)
	ln -s -f ./lib/libgpvdm_lib.@SLIB_EXT@ ../libgpvdm_lib.@SLIB_EXT@

.c.o:
	$(CC) -c $(CPPFLAGS) $(COMPILE_FLAG) $(inc) @I_DBUS@ $< -o $@  

clean:
	rm *.o *.dll *.so *.a *.lib -f
