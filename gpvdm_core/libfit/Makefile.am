inc=-I../include/
OBJS=fit.o fit_config.o fit_constraints.o fit_error.o fit_run.o fit_duplicate.o fit_sync.o fit_dump.o my_f.o fit_newton.o fit_build.o fit_vars.o fit_simplex.o fit_init.o fit_free.o fit_patch.o fit_cmp_sim_and_exp.o fit_plot.o

all: $(OBJS)
	$(AR) cr libgpvdm_fit.@SLIB_EXT@ $(OBJS)
	ln -s -f ./libfit/libgpvdm_fit.@SLIB_EXT@ ../libgpvdm_fit.@SLIB_EXT@

#-cvq

.c.o:
	$(CC) -c $(CPPFLAGS) @COMPILE_FLAG@ $(inc) -Wall $< -o $@

clean:
	rm *.o *.dll *.so *.a *.lib -f
