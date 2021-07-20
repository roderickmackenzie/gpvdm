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
#include <json.h>

THREAD_FUNCTION thread_dos_n(void * in)
{
	char temp[100];
	char path[PATH_MAX];
	struct dosconfig confige;
	struct dosconfig configh;

	struct job *j=(struct job *)in;
	struct simulation *sim=(struct simulation *)j->sim;
	char *dos_file=(char *)j->data0;
	struct json_obj *json_dos=(struct json_obj *)j->data1;

	dos_config_load(sim,&confige,&configh,dos_file, json_dos);

	printf_log(sim,"Electrons.... %s\n",confige.dos_name);
	//printf_log(sim,"e Eg='%Le' \n",confige.Eg);
	//printf_log(sim,"e Eg='%Le' \n",configh.Eg);

	sprintf(temp,"%s_dosn.dat",confige.dos_name);
	join_path(2, path,get_cache_path(sim),temp);

	//char json_path[PATH_MAX];
	//join_path(2,json_path,get_cache_path(sim),"json_dos_n.dat");
	//json_obj_save_as(sim,json_path,json_dos);

	gen_do(sim,&confige,&configh,path,json_dos,TRUE);

	j->data1=NULL;

	server2_job_finished(sim,j);

	return 0;
}

THREAD_FUNCTION thread_dos_p(void * in)
{
	char temp[100];
	char path[PATH_MAX];
	FILE *out;
	struct dosconfig confige;
	struct dosconfig configh;
	struct job *j=(struct job *)in;
	struct simulation *sim=(struct simulation *)j->sim;
	char *dos_file=(char *)j->data0;
	struct json_obj *json_dos=(struct json_obj *)j->data1;

	dos_config_load(sim,&confige,&configh,dos_file, json_dos);

	printf_log(sim,"Holes.... %s\n",configh.dos_name);
	//printf_log(sim,"h Eg='%Le' \n",confige.Eg);
	//printf_log(sim,"h Eg='%Le' \n",configh.Eg);

	sprintf(temp,"%s_dosp.dat",configh.dos_name);
	join_path(2, path,get_cache_path(sim),temp);

	//char json_path[PATH_MAX];
	//join_path(2,json_path,get_cache_path(sim),"json_dos_p.dat");
	//json_obj_save_as(sim,json_path,json_dos);

	gen_do(sim,&configh,&confige,path,json_dos,FALSE);

	join_path(2, path,get_cache_path(sim),"mat.inp");

	out=fopen(path,"w");
	fprintf(out,"#gpvdm_file_type\n");
	fprintf(out,"cache\n");
	fprintf(out,"#end\n");
	fclose(out);

	j->data1=NULL;
	server2_job_finished(sim,j);

	return 0;
}
void gen_dos_fd_gaus_fd_stand_alone(struct simulation *sim,char *input_path)
{
	struct epitaxy epi;
	char full_name[PATH_MAX];
	epitaxy_init(sim,&epi);
	struct json j;
	struct json_obj *json_epi;

	json_init(&j);

	join_path(2, full_name,input_path,"json.inp");

	json_load(sim,&j,full_name);

	json_epi=json_obj_find(&(j.obj), "epitaxy");

	if (json_epi==NULL)
	{
		ewe(sim,"Object light not found\n");
	}

	epitaxy_load(sim,&epi,json_epi);
	dos_cache_setup(sim,&(sim->doscache),&(j.obj));
	gen_dos_fd_gaus_fd(sim,&epi,json_epi);
	epitaxy_free(sim,&epi);

	json_free(&j);
}

int check_and_gen_dos(struct simulation *sim,struct shape *s,struct json_obj *json_dos, char *check_file, char *md5,int batch_id)
{
	int enabled=FALSE;
	int ret=0;
	struct job my_job;
	char name[200];
	char full_name[PATH_MAX];
	char chk_file[200];
	int launch_server=FALSE;
	FILE *file;
	//json_dump_obj(json_dos);
	//printf("a\n");
	json_get_english(sim,json_dos, &(enabled),"enabled");
	if (enabled==FALSE)
	{
		return FALSE;
	}

	sprintf(chk_file,"%s.chk",s->dos_file);
	join_path(2, full_name,get_cache_path(sim),chk_file);
	strcpy(check_file,chk_file);

	if (json_checksum_check(sim,md5,full_name,json_dos)==FALSE)
	{
		printf("checksum failed\n");
		launch_server=TRUE;
	}

	sprintf(name,"%s_dosn.dat",s->dos_file);

	join_path(2, full_name,get_cache_path(sim),name);
	if (isfile(full_name)!=0)
	{
		launch_server=TRUE;
	}

	sprintf(name,"%s_dosp.dat",s->dos_file);
	join_path(2, full_name,get_cache_path(sim),name);
	if (isfile(full_name)!=0)
	{
		launch_server=TRUE;
	}

	if (launch_server==TRUE)
	{
		job_init(sim,&my_job);
		strcpy(my_job.name,"dosn");
		my_job.fun=&thread_dos_n;
		my_job.sim=(void *)sim;
		my_job.batch_id=batch_id;

		malloc_1d((void **)&(my_job.data0),100,sizeof(char));

		strcpy((char *)my_job.data0,s->dos_file);
		my_job.data1=(void *)json_dos;
		gpvdm_mkdir(get_cache_path(sim));

		//printf("adding job\n");
		server2_add_job(sim,&(sim->server),&my_job);
		job_init(sim,&my_job);
		strcpy(my_job.name,"dosp");
		my_job.fun=&thread_dos_p;
		my_job.sim=(void *)sim;
		my_job.batch_id=batch_id;

		malloc_1d((void **)&(my_job.data0),100,sizeof(char));

		strcpy((char *)my_job.data0,s->dos_file);
		my_job.data1=(void *)json_dos;
		//printf("adding job\n");

		server2_add_job(sim,&(sim->server),&my_job);

		return TRUE;
	}

return FALSE;
}


void gen_dos_fd_gaus_fd(struct simulation *sim,struct epitaxy *my_epitaxy,struct json_obj *json_epi)
{
int i;
int l;
int ns;
char temp[PATH_MAX];
FILE *file;
struct shape *s;
struct inp_file inp;
struct job j;
struct json_obj *json_dos;
struct json_obj *json_layer;
struct json_obj *json_shape_dos;
struct epi_layer *layer;
struct json_obj *json_sub_shape;
char check_files[20][100];
char sum[20][100];
int pos=0;
int batch_id=server2_get_next_batch_id(sim,&(sim->server));

	for (l=0;l<my_epitaxy->layers;l++)
	{
		layer=&(my_epitaxy->layer[l]);
		s=&(layer->s);
		sprintf(temp,"layer%d",l);
		json_layer=json_obj_find(json_epi, temp);
		json_shape_dos=json_obj_find(json_layer, "shape_dos");
		printf_log(sim,"DoS gen: %s > %s\n",temp,s->dos_file);

		if (check_and_gen_dos(sim,s,json_shape_dos,check_files[pos],sum[pos],batch_id)==TRUE)
		{
			pos++;
		}

	}

	for (l=0;l<my_epitaxy->layers;l++)
	{
		layer=&(my_epitaxy->layer[l]);
		s=&(layer->s);
		sprintf(temp,"layer%d",l);
		json_layer=json_obj_find(json_epi, temp);

		for (ns=0;ns<layer->nshape;ns++)
		{
			s=&(layer->shapes[ns]);
			sprintf(temp,"shape%d",ns);
			json_sub_shape=json_obj_find(json_layer, temp);
			json_shape_dos=json_obj_find(json_sub_shape, "shape_dos");
			printf_log(sim,"DoS gen: layer%d,shape%d > %s\n",l,ns,s->dos_file);
			if (check_and_gen_dos(sim,s,json_shape_dos,check_files[pos],sum[pos],batch_id)==TRUE)
			{
				pos++;
			}	
		}
	}
	//server2_dump_jobs(sim,&(sim->server));

	server2_run_until_done(sim,&(sim->server),batch_id);
	server2_free_finished_jobs(sim,&(sim->server),batch_id);
	//server2_dump_jobs(sim,&(sim->server));

	for (i=0;i<pos;i++)
	{
		join_path(2, temp,get_cache_path(sim),check_files[i]);
		file=fopen(temp,"w");
		fprintf(file,"%s\n",sum[i]);
		fclose(file);
	}

	printf_log(sim,_("Finished generating DoS....\n"));

	//exit(0);
}


