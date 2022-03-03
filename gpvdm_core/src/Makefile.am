OBJS=  server.o sim_run.o pos.o config.o anal.o run_electrical_dll.o sim.o check_and_gen_dos.o clean_sim_dir.o
# -pg  -lefence  -lefence 
#echo 428000 > /proc/sys/vm/max_map_count  for when efence runs out of memroy 
AROPT=

inc=-I../include/ -L../

all: main.c $(OBJS)
	$(CC) -shared -o ../libgpvdm_core.@DLIB_EXT@ @GPVDM_CORE_LIB_FILES@ -L/home/rod/windll/compiled_dlls2/ @INTERNET_LIB@ -lzip @PNG_LIB@
	@WIN_ICON@
	$(CC) main.c $(OBJS) -o ../gpvdm_core@EXE_EXT@ @WIN_RES_FILE@ $(inc) @COMPILE_FLAG@ -Wl,--whole-archive @LOCAL_LINK@ -Wl,--no-whole-archive @LDFLAGS@ @LIBS@ @OPEN_CL_LINK@ @INTERNET_LIB@ -lpthread -pg @SRC_DEBUG@ 
	ln -s -f gpvdm_core@EXE_EXT@ ../go@EXE_EXT@
	ln -s -f ./gpvdm_core/gpvdm_core@EXE_EXT@ ../../go@EXE_EXT@
	@echo "$(DESTDIR)$(bindir)" > bindir
	@echo "$(DESTDIR)$(datadir)" > datadir
	@echo "$(DESTDIR)$(datarootdir)" > datarootdir

	rm ../../libgpvdm_core.@DLIB_EXT@ -f
	ln -s ./gpvdm_core/libgpvdm_core.@DLIB_EXT@ ../../libgpvdm_core.@DLIB_EXT@
	@echo "(gpvdm_build_finished)"
	
.c.o : 
	$(CC) -c $(DEFINE_FLAGS) $(inc) $(CPPFLAGS) $(warn) @COMPILE_FLAG@ $< -o $@

clean:
	rm *.o *.a *.dll *.exe -f
