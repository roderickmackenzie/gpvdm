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

/** @file circuit_spm.c
@brief Scanning Probe Microscopy
*/

#include "device.h"
#include <mesh.h>
#include "inp.h"
#include "util.h"
#include "gpvdm_const.h"
#include "hard_limit.h"
#include <log.h>
#include <cal_path.h>
#include <circuit.h>
#include <matrix.h>
#include <memory.h>
#include <contacts.h>
#include <epitaxy_struct.h>
#include <epitaxy.h>
#include <exp.h>
#include <dump.h>
#include <lang.h>
#include <server.h>
#include <device_fun.h>

THREAD_FUNCTION circuit_spm_thread(void * in)
{
	//printf("entered\n");

	int n;
	long double V_applied=1.0;
	struct circuit_node *nodes_new;
	struct circuit_node *node;


	//circuit_init(&cir_new);
	struct job *j=(struct job *)in;
	struct worker *w=j->w;
	int worker_number=w->worker_n;

	struct simulation *sim=(struct simulation *)j->sim;
	struct device *dev=(struct device *)j->data0;
	long double *J=(long double *)j->data1;
	struct circuit_node *contact_node=(struct circuit_node *)j->data2;
	struct circuit *cir_new=&(((struct circuit* )j->data3)[worker_number]);
	printf("worker %d %d\n",worker_number,sim->server.worker_max);
	cir_new->quite=TRUE;

	//struct circuit *cir=&dev->cir;
	int nn=j->data_int0;
	//printf("start cpy\n");
	//circuit_cpy(sim,&cir_new,cir);
	nodes_new=(cir_new->nodes);

	//printf("End cpy\n");
	//printf("%d %d\n",cir_new->nodes_len,worker_number);
	for (n=0;n<cir_new->nodes_len;n++)
	{
		node=&(nodes_new[n]);
		if (n==nn)
		{
			node->V=V_applied;
			node->type=CIR_CHANGE_X;
		}else
		{
			if (node->type==CIR_CHANGE_X)
			{
				node->type=CIR_UNKNOWN;
			}
		}
	}



	circuit_calculate_matrix_pos(sim,cir_new);
	matrix_free(sim,&(cir_new->mx));
	circuit_malloc_matrix(sim,cir_new);
	printf("start %p %p %p\n",&(cir_new->mx),cir_new->mx.b,&(cir_new->mx));
	circuit_build_and_solve_matrix(sim,cir_new,dev,worker_number);
	printf("exit\n");
	//printf("dump\n");
	//circuit_dump_to_obj_file(sim,"circuit_voltages2.dat",dev,cir_new);

	//printf("I=%le\n",circuit_node_get_I(sim,cir_new,nn));
	//J_out[contact_node->z][contact_node->x]
	//circuit_print_nodes(sim,cir_new);
	//printf("%lf\n",circuit_node_get_I(sim,cir_new,nn));
	//getchar();

	*J=V_applied/fabs(circuit_node_get_I(sim,cir_new,nn));
	printf("solve->%d %d\n",contact_node->z,contact_node->x);
	//circuit_print_nodes(sim,cir_new);
	//circuit_free(sim,cir_new);

	//getchar();

	j->data0=NULL;
	j->data1=NULL;
	j->data2=NULL;
	j->data3=NULL;
	server2_job_finished(sim,j);


	return 0;
}

void circuit_spm(struct simulation * sim,struct device *dev,long double x0,long double x1,long double z0,long double z1)
{

	int nn;
	int y_max_pos;
	long double x_pos;
	long double z_pos;
	struct job j;
	struct dimensions *dim=&dev->ns.dim;
	struct circuit *cir=&dev->cir;
	//struct circuit_link *links=(cir->links);

	struct circuit_node *contact_node;

	//struct circuit_node *nodes=(cir->nodes);
	struct dat_file buf;
	dat_file_init(&buf);

	char name[PATH_MAX];
	long double **R_out=NULL;
	long double **V_lost=NULL;
	int batch_id=server2_get_next_batch_id(sim,&(sim->server));
	//sim->server.worker_max=1;

	malloc_zx_gdouble(dim, &R_out);
	mem_set_zx_long_double(dim, R_out, 0.0);

	malloc_zx_gdouble(dim, &V_lost);
	mem_set_zx_long_double(dim, V_lost, 0.0);

	sim->server.send_progress_to_gui=TRUE;
	y_max_pos=circuit_cal_max_y_node_level(sim,cir,dev);
	int n;
	struct circuit *cir_array=NULL;
	malloc_1d((void **)&(cir_array),sim->server.worker_max, sizeof(struct circuit));
	for (n=0;n<sim->server.worker_max;n++)
	{
		circuit_init(&cir_array[n]);
		circuit_cpy(sim,&cir_array[n],cir);
	}


	for (nn=0;nn<cir->nodes_len;nn++)
	{
		contact_node=&(cir->nodes[nn]);
		if (contact_node->type==CIR_CHANGE_X)
		{
			//printf("%d %d\n",contact_node->y,y_max_pos);
			if (contact_node->y==y_max_pos)
			{
				x_pos=dim->x[contact_node->x];
				z_pos=dim->z[contact_node->z];
				if ((x_pos>=x0)&&(x_pos<=x1))
				{
					//printf("%Le %Le %Le\n",z0,z_pos,z1);
					if ((z_pos>=z0)&&(z_pos<=z1))
					{
						job_init(sim,&j);
						sprintf(j.name,"circuit_spm-%d,%d",contact_node->z,contact_node->x);
						j.fun=&circuit_spm_thread;
						j.sim=(void *)sim;
						j.batch_id=batch_id;

						j.data0=(void *)dev;
						j.data1=(void *)&R_out[contact_node->z][contact_node->x];
						j.data2=(void *)contact_node;
						j.data3=(void *)cir_array;
						//if (nn_first==-1)
						//{
						//	nn_first=nn;
						//}
						j.data_int0=nn;//nn_first;
						//malloc_1d((void **)&(j.data2),1,sizeof(int));
						//((int *)j.data2)[0]=l;

						server2_add_job(sim,&(sim->server),&j);
					}
				}
			}
		}

	}

	//server2_dump_jobs(sim,&(sim->server));
	//int store=sim->server.worker_max;			//To make this multithreaded you need a fix to the matrix memory in the light solver
	//sim->server.worker_max=3;
	server2_run_until_done(sim,&(sim->server),batch_id);
	//sim->server.worker_max=store;
	server2_free_finished_jobs(sim,&(sim->server),batch_id);

	for (n=0;n<sim->server.worker_max;n++)
	{
		circuit_free(sim,&cir_array[n]);
	}
	free_1d((void **)&(cir_array), sizeof(struct circuit));

	//dump output
	buffer_malloc(&buf);
	sprintf(name,"%s","spm_R.dat");
	dim_info_to_buf(&buf,dim);
	strcpy(buf.type,"heat");
	sprintf(buf.title,"%s - %s",_("R spm"),_("position"));
	strcpy(buf.data_label,_("R spm"));
	strcpy(buf.data_units,"Ohms");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.time=dev->time;
	buf.Vexternal=0.0;
	buf.z=1;
	buf.y=dim->xlen;
	buf.x=dim->zlen;
	buffer_add_info(sim,&buf);
	buffer_add_zx_data(sim,&buf,dim, R_out);
	buffer_dump_path(sim,dev->output_path,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","spm_R_x.dat");
	dim_info_to_buf(&buf,dim);
	strcpy(buf.type,"heat");
	sprintf(buf.title,"%s - %s",_("R spm"),_("position"));
	strcpy(buf.data_label,_("R spm"));
	strcpy(buf.data_units,"Ohms");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.time=dev->time;
	buf.Vexternal=0.0;
	buf.z=1;
	buf.y=dim->xlen;
	buf.x=1;
	buffer_add_info(sim,&buf);

	int x;
	char temp[200];
	for (x=0;x<dim->xlen;x++)
	{
		sprintf(temp,"%Le %Le\n",dim->x[x],R_out[dim->zlen/2][x]);
		buffer_add_string(&buf,temp);
	}
	//buffer_add_zx_data(sim,&buf,dim, R_out);
	buffer_dump_path(sim,dev->output_path,name,&buf);
	buffer_free(&buf);

	long double my_j=0.0;
	long double R_avg=0.0;
	long double aera=0.0;

	for (nn=0;nn<cir->nodes_len;nn++)
	{
		contact_node=&(cir->nodes[nn]);
		if (contact_node->type==CIR_CHANGE_X)
		{
			//printf("%d %d\n",contact_node->y,y_max_pos);
			if (contact_node->y==y_max_pos)
			{
				x_pos=dim->x[contact_node->x];
				z_pos=dim->z[contact_node->z];
				if ((x_pos>=x0)&&(x_pos<=x1))
				{
					if ((z_pos>=z0)&&(z_pos<=z1))
					{
						my_j=200.0;	//A/m2
						V_lost[contact_node->z][contact_node->x]=R_out[contact_node->z][contact_node->x]*my_j*dim->dX[contact_node->x]*dim->dZ[contact_node->z];
						R_avg+=R_out[contact_node->z][contact_node->x]*dim->dX[contact_node->x]*dim->dZ[contact_node->z];
						aera+=dim->dX[contact_node->x]*dim->dZ[contact_node->z];
					}
				}
			}
		}

	}
	R_avg=R_avg/aera;

	//dump output
	buffer_malloc(&buf);
	sprintf(name,"%s","Vlost_spm.dat");
	dim_info_to_buf(&buf,dim);
	strcpy(buf.type,"heat");
	sprintf(buf.title,"%s - %s",_("Vlost spm"),_("position"));
	strcpy(buf.data_label,_("V spm"));
	strcpy(buf.data_units,"Volts");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.time=dev->time;
	buf.Vexternal=0.0;
	buf.z=1;
	buf.y=dim->xlen;
	buf.x=dim->zlen;
	buffer_add_info(sim,&buf);
	buffer_add_zx_data(sim,&buf,dim, R_out);
	buffer_dump_path(sim,dev->output_path,name,&buf);
	buffer_free(&buf);

	FILE *out;
	out=fopena(dev->output_path,"sim_info.dat","w");
	fprintf(out,"{\n");
	fprintf(out,"\t\"R_avg\": %Le\n",R_avg);
	fprintf(out,"}");
	fclose(out);

	free_zx_gdouble(dim,&R_out);
	free_zx_gdouble(dim,&V_lost);

}


