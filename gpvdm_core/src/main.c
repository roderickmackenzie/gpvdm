// 
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// The model can simulate OLEDs, Perovskite cells, and OFETs.
// 
// Copyright (C) 2008-2020 Roderick C. I. MacKenzie
// 
// https://www.gpvdm.com
// r.c.i.mackenzie at googlemail.com
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the GPVDM nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL Roderick C. I. MacKenzie BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

/** @file main.c
@brief main... everything begins here.
*/


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
#include <patch.h>
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
#include <enabled_libs.h>

static int unused __attribute__((unused));


int main (int argc, char *argv[])
{
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

/*struct rpn rpn_cal;
rpn_init(&sim,&rpn_cal);
rpn_add_var(&sim,&rpn_cal,"a",1e-10);
double value1=rpn_evaluate(&sim,&rpn_cal,"log(a)");
printf("rodeval: %le\n",value1);
return 0;*/

if (scanarg( argv,argc,"--gui")==TRUE)
{
	sim.gui=TRUE;
}

int log_level=0;
set_logging_level(&sim,log_level_screen);
cal_path(&sim);

//<strip>

//</strip>


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
	printf_log(&sim,"%s\n",_("Copyright (C) 2010-2019 Roderick C. I. MacKenzie, Releced the BSD 3-clause License"));
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
	printf_log(&sim,"%s\n\n",_("Report bugs to: roderick.mackenzie@nottingham.ac.uk"));
	exit(0);
}
if (scanarg( argv,argc,"--version")==TRUE)
{
	printf_log(&sim,_("gpvdm_core, Version %s\n"),gpvdm_ver);
	printf_log(&sim,"%s\n",_("gpvdm (General-purpose Photovoltaic Device Model) core."));
	printf_log(&sim,"%s\n",_("Copyright Roderick MacKenzie, released under the BSD 3-Clause License 2010-2019"));
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
	printf_log(&sim,_("gpvdm_core, Version %s\n"),gpvdm_ver);
	printf_log(&sim,"%s\n",_("gpvdm (General-purpose Photovoltaic Device Model) core."));
	printf_log(&sim,"%s\n",_("Copyright Roderick MacKenzie, released under the BSD 3-Clause License 2010-2019"));
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

hard_limit_init(&sim);
errors_init(&sim);

dumpfiles_load(&sim);
set_plot_script_dir(pwd);

//set_plot_script_dir(char * in)



if(geteuid()==0) {
	ewe(&sim,"Don't run me as root!\n");
}


srand(time(0));
//printf_log(&sim,"%s\n",_("Token"));
//exit(0);
randomprint(&sim,_("gpvdm_core - General-purpose Photovoltaic Device Model\n"));
randomprint(&sim,_("Copyright (C) 2010-2019 Roderick C. I. MacKenzie,\n"));
randomprint(&sim,_(" Releced the BSD 3-clause License"));

randomprint(&sim,"\n");
randomprint(&sim,_("If you wish to collaborate in anyway please get in touch:\n"));
randomprint(&sim,"roderick.mackenzie@nottingham.ac.uk\n");
randomprint(&sim,"www.rodmack.com/contact.html\n");
randomprint(&sim,"\n");
//getchar();
sim.server.on=FALSE;
sim.server.max_threads=1;
sim.server.readconfig=TRUE;


color_cie_init(&sim);
color_cie_load(&sim);

if (scanarg( argv,argc,"--outputpath")==TRUE)
{
	strcpy(sim.output_path,get_arg_plusone( argv,argc,"--outputpath"));
}


if (scanarg( argv,argc,"--inputpath")==TRUE)
{
	strcpy(sim.input_path,get_arg_plusone( argv,argc,"--inputpath"));
}





char name[200];
struct inp_file inp;

inp_init(&sim,&inp);
if (inp_load_from_path(&sim,&inp,sim.input_path,"ver.inp")!=0)
{
	printf_log(&sim,"can't find file %s ver.inp",sim.input_path);
	exit(0);
}
inp_check(&sim,&inp,1.0);
inp_search_string(&sim,&inp,name,"#core");
inp_free(&sim,&inp);

if (strcmp(name,gpvdm_ver)!=0)
{
printf_log(&sim,"Software is version %s and the input files are version %s\n",gpvdm_ver,name);
exit(0);
}

if (scanarg( argv,argc,"--gui")==TRUE)
{
	sim.gui=TRUE;
}

gui_start(&sim);
server_init(&sim);

server2_config_load(&sim,&(sim.server));
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
join_path(2,temp,get_output_path(&sim),"tx.dat");
remove_file(&sim,temp);

#ifdef libfit_enabled
	if (scanarg( argv,argc,"--1fit")==TRUE)
	{
		sim.fitting=TRUE;

		static struct fitvars fitconfig;
		fit_init(&sim,&fitconfig);
		int fit_file_found=fit_read_config(&sim,&fitconfig);

		if (fit_file_found==0)
		{
			fit_run_sims(&sim,&fitconfig);

		}else
		{
			ewe(&sim,"Can't find fit file\n");
		}

		hard_limit_free(&sim);
		run=TRUE;

	}else
	if (scanarg( argv,argc,"--fit")==TRUE)
	{

		static struct fitvars fitconfig;
		fit_init(&sim,&fitconfig);
		fit_read_config(&sim,&fitconfig);
		sim.fitting=TRUE;
		int cont=TRUE;

		fit_log_init(&sim);
		fit_now(&sim,&fitconfig);
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

	server_add_job(&sim,sim.output_path,sim.input_path);
	print_jobs(&sim);

	ret=server_run_jobs(&sim,&(sim.server));

}

server_shut_down(&sim,&(sim.server));

server2_free(&sim,&sim.server);

color_cie_free(&sim);
hard_limit_free(&sim);
dumpfiles_free(&sim);
errors_free(&sim);

if (ret!=0)
{
	return 1;
}
return 0;
}

