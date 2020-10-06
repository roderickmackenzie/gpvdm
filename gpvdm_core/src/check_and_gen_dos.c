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

/** @file check_and_gen_dos.c
@brief poisson solver - make a better guess at 0V in the dark.
*/

#include <string.h>
#include <stdlib.h>
#include <dump.h>
#include <dos.h>
#include "sim.h"
#include "solver_interface.h"
#include "dat_file.h"
#include "log.h"
#include <cal_path.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <lang.h>
#include <memory.h>
#include <checksum.h>
#include <inp.h>
#include <server.h>
#include <epitaxy.h>

THREAD_FUNCTION thread_dos_n(void * in)
{
	struct job *j=(struct job *)in;
	struct simulation *sim=(struct simulation *)j->sim;
	char *dos_file=(char *)j->data0;
	char *lumo_file=(char *)j->data1;
	char *homo_file=(char *)j->data2;

	gen_dos_fd_gaus_n(sim,dos_file,lumo_file,homo_file);

	server2_job_finished(sim,j);

	return 0;
}

THREAD_FUNCTION thread_dos_p(void * in)
{
	struct job *j=(struct job *)in;
	struct simulation *sim=(struct simulation *)j->sim;
	char *st=(char *)j->data0;
	char *dos_file=(char *)j->data0;
	char *lumo_file=(char *)j->data1;
	char *homo_file=(char *)j->data2;

	gen_dos_fd_gaus_p(sim,dos_file,lumo_file,homo_file);

	server2_job_finished(sim,j);

	return 0;
}
void gen_dos_fd_gaus_fd_stand_alone(struct simulation *sim)
{
	struct epitaxy epi;
	char full_name[PATH_MAX];
	epitaxy_init(sim,&epi);
	join_path(2, full_name,get_input_path(sim),"epitaxy.inp");
	epitaxy_load(sim,&epi,full_name);
	gen_dos_fd_gaus_fd(sim,&epi);
	epitaxy_free(sim,&epi);
}


void gen_dos_fd_gaus_fd(struct simulation *sim,struct epitaxy *my_epitaxy)
{
int ret=0;
char name[200];
char full_name[PATH_MAX];
int matnumber=0;
//printf("yes %s %s\n",get_input_path(sim),get_cache_path(sim));
//getchar();
matnumber=my_epitaxy->electrical_layers;
int file_bandn=FALSE;
int file_bandp=FALSE;
int file_an_lumo=FALSE;
int file_an_homo=FALSE;
int file_dos=FALSE;
int launch_server=FALSE;

FILE *file;
int mat=0;
int problem_with_dos=FALSE;
int dostype;
char temp[100];

struct inp_file inp;

struct job j;


for (mat=0;mat<matnumber;mat++)
{
	file_bandn=FALSE;
	file_bandp=FALSE;

	file_an_lumo=FALSE;
	file_an_homo=FALSE;
	file_dos=FALSE;




	problem_with_dos=FALSE;


	sprintf(name,"%s.inp",my_epitaxy->layer[mat].dos_file);
	join_path(2, full_name,get_input_path(sim),name);

	inp_init(sim,&inp);
	inp_load(sim,&inp,full_name);

	inp_search_string(sim,&inp,temp,"#dostype");
	dostype=english_to_bin(sim,temp);
	inp_free(sim,&inp);

	if (checksum_check(sim,full_name)==FALSE)
	{
		problem_with_dos=TRUE;
	}

	sprintf(name,"%s_dosn.dat",my_epitaxy->layer[mat].dos_file);
	join_path(2, full_name,get_cache_path(sim),name);
	if (isfile(full_name)!=0)
	{
		problem_with_dos=TRUE;
		//printf("oh0 %s\n",full_name);
	}

	sprintf(name,"%s_dosp.dat",my_epitaxy->layer[mat].dos_file);
	join_path(2, full_name,get_cache_path(sim),name);
	if (isfile(full_name)!=0)
	{
		problem_with_dos=TRUE;
		//printf("oh1\n");

	}

	if (problem_with_dos==TRUE)
	{
		file_dos=TRUE;
		file_bandn=TRUE;
		file_bandp=TRUE;
		launch_server=TRUE;
		//printf("launch\n");
		//getchar();
	}


	if (dostype==dos_read)
	{
		sprintf(name,"%s_srhbandn.inp",my_epitaxy->layer[mat].dos_file);
		join_path(2, full_name,get_input_path(sim),name);
		if (checksum_check(sim,full_name)==FALSE)
		{
			file_bandn=TRUE;
			launch_server=TRUE;
		}

		sprintf(name,"%s_srhbandp.inp",my_epitaxy->layer[mat].dos_file);
		join_path(2, full_name,get_input_path(sim),name);
		if (checksum_check(sim,full_name)==FALSE)
		{
			file_bandp=TRUE;
			launch_server=TRUE;
		}
	}

	if (dostype==dos_an)
	{
		sprintf(name,"%s.inp",my_epitaxy->lumo_file[mat]);
		join_path(2, full_name,get_input_path(sim),name);
		if (checksum_check(sim,full_name)==FALSE)
		{
			file_an_lumo=TRUE;
			file_bandn=TRUE;
			launch_server=TRUE;
		}

		sprintf(name,"%s.inp",my_epitaxy->homo_file[mat]);
		join_path(2, full_name,get_input_path(sim),name);
		if (checksum_check(sim,full_name)==FALSE)
		{
			file_an_homo=TRUE;
			file_bandp=TRUE;
			launch_server=TRUE;
		}
	}


	if (launch_server==TRUE)
	{


		if (file_bandn==TRUE)
		{
			//server_add_job(sim,name,get_output_path(sim));
			//printf("oh yes\n");
			//getchar();
			job_init(sim,&j);
			strcpy(j.name,"dosn");
			j.fun=&thread_dos_n;
			j.sim=(void *)sim;

			malloc_1d((void **)&(j.data0),100,sizeof(char));
			malloc_1d((void **)&(j.data1),100,sizeof(char));
			malloc_1d((void **)&(j.data2),100,sizeof(char));

			strcpy((char *)j.data0,my_epitaxy->layer[mat].dos_file);
			strcpy((char *)j.data1,my_epitaxy->lumo_file[mat]);
			strcpy((char *)j.data2,my_epitaxy->homo_file[mat]);
			
			server2_add_job(sim,&(sim->server),&j);
		}


		if (file_bandp==TRUE)
		{
			//gen_dos_fd_gaus_p(sim,my_epitaxy->layer[mat].dos_file,my_epitaxy->lumo_file[mat],my_epitaxy->homo_file[mat]);

			//gen_dos_fd_gaus_p(sim,mat);
			//server_add_job(sim,name,get_output_path(sim));
			job_init(sim,&j);
			strcpy(j.name,"dosp");
			j.fun=&thread_dos_p;
			j.sim=(void *)sim;

			malloc_1d((void **)&(j.data0),100,sizeof(char));
			malloc_1d((void **)&(j.data1),100,sizeof(char));
			malloc_1d((void **)&(j.data2),100,sizeof(char));

			strcpy((char *)j.data0,my_epitaxy->layer[mat].dos_file);
			strcpy((char *)j.data1,my_epitaxy->lumo_file[mat]);
			strcpy((char *)j.data2,my_epitaxy->homo_file[mat]);
			
			server2_add_job(sim,&(sim->server),&j);
		}

		server2_run_until_done(sim,&(sim->server));
		server2_free_finished_jobs(sim,&(sim->server));
		//server2_dump_jobs(sim,&(sim->server));

		//exit(0);

		sprintf(name,"%s.inp",my_epitaxy->layer[mat].dos_file);
		join_path(2, full_name,get_input_path(sim),name);
		if (file_dos==TRUE) checksum_write(sim,full_name);


		if (dostype==dos_read)
		{
			sprintf(name,"%s_srhbandn.inp",my_epitaxy->layer[mat].dos_file);
			safe_file(name);
			join_path(2, full_name,get_input_path(sim),name);
			if (file_bandn==TRUE) checksum_write(sim,full_name);

			sprintf(name,"%s_srhbandp.inp",my_epitaxy->layer[mat].dos_file);
			safe_file(name);
			join_path(2, full_name,get_input_path(sim),name);
			if (file_bandp==TRUE) checksum_write(sim,full_name);
		}

		if (dostype==dos_an)
		{
			sprintf(name,"%s.inp",my_epitaxy->lumo_file[mat]);
			join_path(2, full_name,get_input_path(sim),name);
			if (file_an_lumo==TRUE) checksum_write(sim,full_name);

			sprintf(name,"%s.inp",my_epitaxy->homo_file[mat]);
			join_path(2, full_name,get_input_path(sim),name);
			if (file_an_homo==TRUE) checksum_write(sim,full_name);
		}

	//print_jobs(sim);

	//server_run_jobs(sim,&(sim->server));
	printf_log(sim,_("Finished generating DoS....\n"));

	}

}



}

