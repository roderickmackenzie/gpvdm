// 
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// The model can simulate OLEDs, Perovskite cells, and OFETs.
// 
// Copyright 2008-2022 Roderick C. I. MacKenzie https://www.gpvdm.com
// r.c.i.mackenzie at googlemail.com
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
// SOFTWARE.
// 

/** @file main.c
@brief main... everything begins here.
*/

#include <enabled_libs.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>


#include <time.h>
#include <unistd.h>

#include <util.h>

#include <sim.h>
#include <dos.h>
#include <server.h>
#include <light_interface.h>
#include <dump.h>
#include <inp.h>
#include <gui_hooks.h>
#include <timer.h>
#include <rand.h>
#include <hard_limit.h>
#include <cal_path.h>
#include <lang.h>
#include <log.h>
#include <device.h>
#include <fit.h>
#include <advmath.h>
#include <plot.h>
#include <assert.h>
#include <rpn.h>
#include <color.h>


	#include <sys/prctl.h>
	#include <sys/types.h>

//<strip>
#include <lock.h>
//</strip>

#include <signal.h>

#include <json.h>
#include <device.h>
#include <device_fun.h>
#include <solver_interface.h>

static int unused __attribute__((unused));

#include <stdio.h>
//#include <execinfo.h>
/*void print_trace(void) {
    char **strings;
    size_t i, size;
    enum Constexpr { MAX_SIZE = 1024 };
    void *array[MAX_SIZE];
    size = backtrace(array, MAX_SIZE);
    strings = backtrace_symbols(array, size);
    for (i = 0; i < size; i++)
        printf("%s\n", strings[i]);
    puts("");
    free(strings);
}

void segfault_sigaction(int signal, siginfo_t *si, void *arg)
{
    printf("Caught segfault at address %p\n", si->si_addr);
	print_trace();
    exit(0);
}*/



int main (int argc, char *argv[])
{
/*struct sigaction sa;

memset(&sa, 0, sizeof(struct sigaction));
sigemptyset(&sa.sa_mask);
sa.sa_sigaction = segfault_sigaction;
sa.sa_flags   = SA_SIGINFO;

sigaction(SIGSEGV, &sa, NULL);*/

struct json j;

struct json_obj *obj;
set_ewe_lock_file("","");
	prctl(PR_SET_PDEATHSIG, SIGKILL);


//setlocale(LC_ALL,"");
//bindtextdomain("gpvdm","./lang/");
//textdomain("gpvdm");
//wchar_t wide[1000];
//int i=mbstowcs(wide, _("Hole generation rate"), 1000);
//exit(0);
int run=FALSE;
struct simulation sim;
strcpy(sim.server.lock_file,"");
sim_init(&sim);

/*json_init(&j);

struct json_obj *json_obj;
json_load(&sim,&j,"sim.json");
json_obj=&(j.obj);//json_obj_find_by_path(&sim,&(j.obj), "circuit.circuit_diagram.segment3");

struct json_obj *next_obj;
next_obj=(struct json_obj* )json_obj->objs;

json_dump_obj(&(j.obj));
json_free(&j);
exit(0);*/

/*struct rpn rpn_cal;
rpn_init(&sim,&rpn_cal);
rpn_add_var(&sim,&rpn_cal,"a",1.0);
rpn_add_var(&sim,&rpn_cal,"b",2.0);
double value1=rpn_evaluate(&sim,&rpn_cal,"(a>b)*100.0");
printf("rodeval: %le\n",value1);
return 0;*/

if (scanarg( argv,argc,"--gui")==TRUE)
{
	sim.gui=TRUE;
}

int log_level=0;
set_logging_level(&sim,log_level_screen);
cal_path(&sim);





char *b=NULL;
char *c=NULL;
	if (scanarg( argv,argc,"--lang")==TRUE)
	{
		setlocale(LC_ALL,get_arg_plusone( argv,argc,"--lang"));
		c=textdomain("gpvdm");
		b=bindtextdomain("gpvdm","lang");

	}else
	{
		setlocale(LC_ALL,"");
		setlocale(LC_NUMERIC, "C");
		c=textdomain("gpvdm");
		b=bindtextdomain("gpvdm","lang");
	}


if (scanarg( argv,argc,"--html")==TRUE)
{
	sim.html=TRUE;
	printf_log(&sim,"<meta charset=\"utf-8\">\n");
}else
{
	sim.html=FALSE;
}

if (scanarg( argv,argc,"--help")==TRUE)
{
	printf_log(&sim,"gpvdm_core - General-purpose Photovoltaic Device Model\n");
	printf_log(&sim,"%s\n",_("Copyright (C) 2009-2022 Roderick C. I. MacKenzie, Releced the MIT License"));
	printf_log(&sim,"\n");
	printf_log(&sim,"Usage: gpvdm_core [%s]\n",_("options"));
	printf_log(&sim,"\n");
	printf_log(&sim,"%s:\n",_("Options"));
	printf_log(&sim,"\n");
	printf_log(&sim,"\t--outputpath\t%s\n",_("output data path"));
	printf_log(&sim,"\t--inputpath\t %s\n",_("sets the input path"));
	printf_log(&sim,"\t--version\t%s\n",_("displays the current version"));
	printf_log(&sim,"\t--zip_results\t %s\n",_("zip the results"));
	printf_log(&sim,"\t--simmode\t %s\n",_("Forces a simulation mode."));
	printf_log(&sim,"\t--cpus\t %s\n",_("sets the number of CPUs"));
	printf_log(&sim,"\n");
	printf_log(&sim,"%s\n",_("Additional information about gpvdm is available at https://www.gpvdm.com."));
	printf_log(&sim,"\n");
	printf_log(&sim,"%s\n\n",_("Report bugs to: roderick.mackenzie@durham.ac.uk"));
	exit(0);
}
if (scanarg( argv,argc,"--version")==TRUE)
{
	printf_log(&sim,_("gpvdm_core, Version %s\n"),gpvdm_ver);
	printf_log(&sim,"%s\n",_("gpvdm (General-purpose Photovoltaic Device Model) core."));
	printf_log(&sim,"%s\n",_("Copyright Roderick MacKenzie, released under the MIT License 2009-2022"));
	printf_log(&sim,_("There is ABSOLUTELY NO WARRANTY; not even for MERCHANTABILITY or\n"));
	printf_log(&sim,_("FITNESS FOR A PARTICULAR PURPOSE.\n"));
	printf_log(&sim,"\n");
	exit(0);
}



timer_init(0);
timer_init(1);
dbus_init();


if (scanarg( argv,argc,"--version2")==TRUE)
{
	//if (lock_feature_enabled(&sim)==0)
	//{
	//	printf_log(&sim,_("gpvdm_core/gpvdm_next, Version %s\n"),gpvdm_ver);
	//}else
	//{
	//	printf_log(&sim,_("gpvdm_core, Version %s\n"),gpvdm_ver);
	//}
	printf_log(&sim,"%s\n",_("gpvdm (General-purpose Photovoltaic Device Model) core."));
	printf_log(&sim,"%s\n",_("Copyright Roderick MacKenzie, released under the MIT License 2010-2019"));
	printf_log(&sim,_("There is ABSOLUTELY NO WARRANTY; not even for MERCHANTABILITY or\n"));
	printf_log(&sim,_("FITNESS FOR A PARTICULAR PURPOSE.\n"));
	printf_log(&sim,"\n");
	//<strip>

	//</strip>
	exit(0);
}


char pwd[1000];
if (getcwd(pwd,1000)==NULL)
{
	ewe(&sim,"IO error\n");
}


remove_file(&sim,"snapshots.zip");
remove_file(&sim,"light_dump.zip");


errors_init(&sim);

set_plot_script_dir(pwd);

//set_plot_script_dir(char * in)


if(geteuid()==0) {
	ewe(&sim,"Don't run me as root!\n");
}


srand(time(0));
rainbow_print(&sim,450e-9, 680e-9, "%s\n",_("gpvdm_core - General-purpose Photovoltaic Device Model"));
rainbow_print(&sim,450e-9, 680e-9,_("Copyright (C) 2009-2022 Roderick C. I. MacKenzie,\n"));
rainbow_print(&sim,450e-9, 680e-9,_("Released under the MIT software license"));

rainbow_print(&sim,450e-9, 680e-9,"\n");
rainbow_print(&sim,450e-9, 680e-9,_("If you wish to collaborate in anyway please get in touch:\n"));
rainbow_print(&sim,450e-9, 680e-9,"roderick.mackenzie@durham.ac.uk\n");
rainbow_print(&sim,450e-9, 680e-9,"http://www.gpvdm.com/contact.html\n");
rainbow_print(&sim,450e-9, 680e-9,"\n");
//getchar();
sim.server.on=FALSE;
sim.server.max_threads=1;
sim.server.readconfig=TRUE;

log_clear(&sim);
color_cie_init(&sim);
color_cie_load(&sim);




char name[200];
/*struct inp_file inp;

inp_init(&sim,&inp);
if (inp_load_from_path(&sim,&inp,sim.root_simulation_path,"ver.inp")!=0)
{
	printf_log(&sim,"can't find file %s ver.inp",sim.root_simulation_path);
	exit(0);
}
inp_check(&sim,&inp,1.0);
inp_search_string(&sim,&inp,name,"#core");
inp_free(&sim,&inp);

if (strcmp(name,gpvdm_ver)!=0)
{
printf_log(&sim,"Software is version %s and the input files are version %s\n",gpvdm_ver,name);
exit(0);
}*/

if (scanarg( argv,argc,"--gui")==TRUE)
{
	sim.gui=TRUE;
}

gui_start(&sim);

json_init(&j);
json_load_from_path(&sim,&j,sim.root_simulation_path,"sim.json");

//Check version
	obj=json_obj_find(&(j.obj), "sim");
	if (obj==NULL)
	{
		ewe(&sim,"Object sim not found\n");
	}

	json_get_string(&sim, obj, name,"version");

	if (strcmp(name+1,gpvdm_ver)!=0)
	{
		printf_log(&sim,"Software is version %s and the input files are version %s\n",gpvdm_ver,name+1);
		exit(0);
	}

obj=json_obj_find(&(j.obj), "server");
if (obj==NULL)
{
	ewe(&sim,"Object server not found\n");
}

server_init(&sim);

server2_config_load(&sim,&(sim.server),obj);

obj=json_obj_find(&(j.obj), "hard_limit");

if (obj==NULL)
{
	ewe(&sim,"Object hard_limit not found\n");
}

hard_limit_load(&sim,&(sim.hl),obj);

	obj=json_obj_find(&(j.obj), "math");
	if (obj==NULL)
	{
		ewe(&sim,"Object math not found\n");
	}

	json_get_string(&sim, obj, sim.solver_name,"solver_name");
	json_get_string(&sim, obj, sim.complex_solver_name,"complex_solver_name");
	solver_init(&sim,sim.solver_name);
	complex_solver_init(&sim,sim.complex_solver_name);

json_free(&j);

//long double val=1.1;
//hard_limit_do(&sim,"Eg",&val);
//printf("%Le\n",val);
//getchar();

server2_malloc(&sim,&(sim.server));

if (scanarg( argv,argc,"--lock")==TRUE)
{
	server_set_dbus_finish_signal(&(sim.server), get_arg_plusone( argv,argc,"--lock"));
}

if (scanarg( argv,argc,"--mindbustx")==TRUE)
{
	sim.mindbustx=TRUE;
}


if (scanarg( argv,argc,"--lockfile")==TRUE)
{
	server_set_lock_file(&(sim.server), get_arg_plusone( argv,argc,"--lockfile"));
}

if (scanarg( argv,argc,"--path")==TRUE)
{
	strcpy(sim.command_line_path, get_arg_plusone( argv,argc,"--path"));
}

int ret=0;
char temp[200];
join_path(2,temp,sim.root_simulation_path,"tx.dat");
remove_file(&sim,temp);

#ifdef libfit_enabled
	if (scanarg( argv,argc,"--1fit")==TRUE)
	{
		sim.fitting=FIT_SINGLE_FIT;

		struct fitvars fitconfig;
		fit_init(&sim,&fitconfig);
		int fit_file_found=fit_read_config(&sim,&fitconfig);

		if (fit_file_found==0)
		{
			fit_build_jobs(&sim,&fitconfig);
			fit_run_sims(&sim,&fitconfig);
		}else
		{
			ewe(&sim,"Can't find fit file\n");
		}

		hard_limit_free(&sim,&(sim.hl));
		fit_free(&sim,&fitconfig);
		run=TRUE;
	}else
	if (scanarg( argv,argc,"--fit")==TRUE)
	{

		struct fitvars fitconfig;
		fit_init(&sim,&fitconfig);
		fit_read_config(&sim,&fitconfig);
		sim.fitting=FIT_RUN_FIT;
		int cont=TRUE;

		fit_log_init(&sim);
		fit_now(&sim,&fitconfig);
		fit_free(&sim,&fitconfig);
		run=TRUE;
	}
#endif



if (scanarg( argv,argc,"--simmode")==TRUE)
{
	strcpy(sim.force_sim_mode,get_arg_plusone( argv,argc,"--simmode"));
}

if (run==FALSE)
{
	set_logging_level(&sim,log_level_screen_and_disk);
	//gen_dos_fd_gaus_fd(&sim);

	struct device dev;
	device_init(&sim,&dev);
	strcpy(dev.input_path,sim.root_simulation_path);
	strcpy(dev.output_path,sim.root_simulation_path);

	//sprintf(my_temp,"%s\n%d\n",myserver->command[i],ode);
	//sprintf(lockname,"lock%d.dat",i);
	//set_ewe_lock_file(lockname,my_temp);

//	ode=
	device_run_simulation(&sim,&dev);
	//server_add_job(&sim,sim.root_simulation_path,sim.root_simulation_path);
	//print_jobs(&sim);

	//ret=server_run_jobs(&sim,&(sim.server));

}

dump_clean_cache_files(&sim);
write_lock_file(&sim);
sim_free(&sim);


if (ret!=0)
{
	return 1;
}
return 0;
}

