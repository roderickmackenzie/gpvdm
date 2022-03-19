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

/** @file fdtd_solve.c
	@brief Solve the FDTD field on the CPU or get the GPU to do it.
*/

#include <math.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <inp.h>
#include <sim.h>
#include <log.h>
#include <fdtd.h>
#include <memory.h>
#include <server.h>

#include "vec.h"


THREAD_FUNCTION solve_E(void * in)
{
	//int nw;
	struct job *j=(struct job *)in;
	//struct worker *my_worker=j->w;
	//nw=my_worker->worker_n;

	struct simulation *sim=(struct simulation *)j->sim;
	struct fdtd_data *data=(struct fdtd_data *)j->data0;

	int x0=j->data_int0;
	int x1=j->data_int1;
	int y0=j->data_int2;
	int y1=j->data_int3;
	int z0=j->data_int4;
	int z1=j->data_int5;


	int z=0;
	int x=0;
	int y=0;

	float C=0.0;

	float dHxdx=0.0;
	float dHxdy=0.0;
	float dHxdz=0.0;

	float dHydy=0.0;
	float dHydx=0.0;
	float dHydz=0.0;

	float dHzdx=0.0;
	float dHzdy=0.0;
	//float dHzdz=0.0;

	float Ez_last=0.0;
	float Ex_last=0.0;
	float Ey_last=0.0;

	struct dimensions *dim=&(data->dim);

	float sigma=0.0;
	float eps=0.0;
	for (z=z0;z<z1;z++)
	{
		for (x=x0;x<x1;x++)
		{
			for (y=y0;y<y1;y++)
			{
				sigma=data->sigma[z][x][y];
				eps=epsilon0f*data->epsilon_r[z][x][y];
				C=2.0*data->dt2/(sigma*data->dt2+2.0*eps);

				//partial/partial x
					if (x!=dim->xlen-1)
					{
						dHxdx=(data->Hx[z][x+1][y]-data->Hx[z][x][y])/dim->dx;
						dHydx=(data->Hy[z][x+1][y]-data->Hy[z][x][y])/dim->dx;
						dHzdx=(data->Hz[z][x+1][y]-data->Hz[z][x][y])/dim->dx;
					}else
					{
						dHxdx=0.0;
						dHydx=0.0;
						dHzdx=0.0;
					}
				//partial/partial y
					if (y!=dim->ylen-1)
					{
						dHxdy=(data->Hx[z][x][y+1]-data->Hx[z][x][y])/dim->dy;
						dHydy=(data->Hy[z][x][y+1]-data->Hy[z][x][y])/dim->dy;
						dHzdy=(data->Hz[z][x][y+1]-data->Hz[z][x][y])/dim->dy;
					}else
					{
						dHxdy=0.0;
						dHydy=0.0;
						dHzdy=0.0;
					}
				//partial/partial z
					if (z!=dim->zlen-1)
					{
						dHxdz=(data->Hx[z+1][x][y]-data->Hx[z][x][y])/dim->dz;
						dHydz=(data->Hy[z+1][x][y]-data->Hy[z][x][y])/dim->dz;
						//dHzdz=(data->Hz[z+1][x][y]-data->Hz[z][x][y])/dim->dz;
					}else
					{
						dHxdz=0.0;
						dHydz=0.0;
						//dHzdz=0.0;
					}

				Ez_last=data->Ez_last[z][x][y];
				Ex_last=data->Ex_last[z][x][y];
				Ey_last=data->Ey_last[z][x][y];

				data->Ex[z][x][y]=(dHzdy-dHydz-sigma*Ex_last/2.0+eps*Ex_last/data->dt2)*C;
				data->Ey[z][x][y]=(-dHzdx+dHxdz-sigma*Ey_last/2.0+eps*Ey_last/data->dt2)*C;
				data->Ez[z][x][y]=(dHydx-dHxdy-sigma*Ez_last/2.0+eps*Ez_last/data->dt2)*C;

			}
		}
	}
	j->data0=NULL;
	server2_job_finished(sim,j);

	if (z==-1)
	{
		printf("%f",dHydy+dHxdx);
	}
	return 0;

}



THREAD_FUNCTION solve_H(void * in)
{
	//int nw;
	struct job *j=(struct job *)in;
	//struct worker *my_worker=j->w;
	//nw=my_worker->worker_n;

	struct simulation *sim=(struct simulation *)j->sim;
	struct fdtd_data *data=(struct fdtd_data *)j->data0;

	int x0=j->data_int0;
	int x1=j->data_int1;
	int y0=j->data_int2;
	int y1=j->data_int3;
	int z0=j->data_int4;
	int z1=j->data_int5;

int z;
int x;
int y;

float C=0.0;
float dExdx=0.0;
float dExdy=0.0;
float dExdz=0.0;

float dEydy=0.0;
float dEydx=0.0;
float dEydz=0.0;

float dEzdx=0.0;
float dEzdy=0.0;
float dEzdz=0.0;

float Hz_last=0.0;
float Hx_last=0.0;
float Hy_last=0.0;

struct dimensions *dim=&(data->dim);
for (z=z0;z<z1;z++)
{
	for (x=x0;x<x1;x++)
	{
		for (y=y0;y<y1;y++)
		{

			C=data->dt2/mu0f;

			//partial dx
			if (x>0)
			{
				dExdx=(data->Ex[z][x][y]-data->Ex[z][x-1][y])/dim->dx;
				dEydx=(data->Ey[z][x][y]-data->Ey[z][x-1][y])/dim->dx;
				dEzdx=(data->Ez[z][x][y]-data->Ez[z][x-1][y])/dim->dx;
			}else
			{
				dExdx=0.0;
				dEydx=0.0;
				dEzdx=0.0;
			}

			//partial dy
			if (y>0)
			{
				dExdy=(data->Ex[z][x][y]-data->Ex[z][x][y-1])/dim->dy;
				dEydy=(data->Ey[z][x][y]-data->Ey[z][x][y-1])/dim->dy;
				dEzdy=(data->Ez[z][x][y]-data->Ez[z][x][y-1])/dim->dy;
			}else
			{
				dExdy=0.0;
				dEydy=0.0;
				dEzdy=0.0;
			}

			//partial dz
			if (z>0)
			{
				dExdz=(data->Ex[z][x][y]-data->Ex[z-1][x][y])/dim->dz;
				dEydz=(data->Ey[z][x][y]-data->Ey[z-1][x][y])/dim->dz;
				dEzdz=(data->Ez[z][x][y]-data->Ez[z-1][x][y])/dim->dz;
			}else
			{
				dExdz=0.0;
				dEydz=0.0;
				dEzdz=0.0;
			}


			//dExdz=0.0;

			Hz_last=data->Hz_last[z][x][y];
			Hx_last=data->Hx_last[z][x][y];
			Hy_last=data->Hy_last[z][x][y];

			data->Hx[z][x][y]=(-dEzdy+dEydz)*C+Hx_last;		//-dEzdy
			data->Hy[z][x][y]=(dEzdx-dExdz)*C+Hy_last;
			data->Hz[z][x][y]=(-dEydx+dExdy)*C+Hz_last;		//dExdy
			/*if (dExdy!=0.0)
			{
				printf("%d %d %d %le\n",z,x,y,dExdy);
			}*/

			/*if ((isnan(data->Hz[z][x][y])!=0)||(isnan(data->Hx[z][x][y])!=0)||(isnan(data->Hy[z][x][y])!=0))
			{
				printf("nan detected %f %f %f\n",data->Hz[z][x][y],data->Hx[z][x][y],data->Hy[z][x][y]);
				exit(0);
			}

			if ((isinf(data->Hz[z][x][y])!=0)||(isinf(data->Hx[z][x][y])!=0)||(isinf(data->Hy[z][x][y])!=0))
			{
				printf("inf detected %f %f %f\n",data->Hz[z][x][y],data->Hx[z][x][y],data->Hy[z][x][y]);
				printf("%e %e %e %e %e\n", data->Hx_last[z][x][y],data->Ez[z][x][y+1],data->Ez[z][x][y],data->Ey[z][x+1][y],data->Ey[z][x][y]);
				exit(0);
			}*/
		}
	}

}
	j->data0=NULL;
	server2_job_finished(sim,j);

	if (z==-1)
	{
		printf("%f",dEzdz+dEydy+dExdx);
	}

	return 0;
}

void fdtd_abc(struct simulation *sim,struct fdtd_data *data)
{
	int z=0;
	int x=0;
	int y=0;
	struct dimensions *dim=&(data->dim);
	float dz=dim->z[1]-dim->z[0];
	float dx=dim->x[1]-dim->x[0];
	float dy=dim->y[1]-dim->y[0];

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{

				if (dim->zlen>1)
				{
					if (z==dim->zlen-1)
					{
						float S=sqrt(data->dt2/(data->epsilon_r[z][x][y]*epsilon0f*dz))*sqrt(data->dt2/(mu0f*dz));
						data->Ex[z][x][y]=data->Ex[z-1][x][y]+((S-1.0)/(S+1.0))*(data->Ex[z-1][x][y]-data->Ex_last[z][x][y]);
						data->Ey[z][x][y]=data->Ey[z-1][x][y]+((S-1.0)/(S+1.0))*(data->Ey[z-1][x][y]-data->Ey_last[z][x][y]);
						data->Ez[z][x][y]=data->Ez[z-1][x][y]+((S-1.0)/(S+1.0))*(data->Ez[z-1][x][y]-data->Ez_last[z][x][y]);
					}
					
					if (z==0)
					{
						float S=sqrt(data->dt2/(data->epsilon_r[z][x][y]*epsilon0f*dz))*sqrt(data->dt2/(mu0f*dz));
						data->Ex[z][x][y]=data->Ex[z+1][x][y]+((S-1.0)/(S+1.0))*(data->Ex[z+1][x][y]-data->Ex_last[z][x][y]);
						data->Ey[z][x][y]=data->Ey[z+1][x][y]+((S-1.0)/(S+1.0))*(data->Ey[z+1][x][y]-data->Ey_last[z][x][y]);
						data->Ez[z][x][y]=data->Ez[z+1][x][y]+((S-1.0)/(S+1.0))*(data->Ez[z+1][x][y]-data->Ez_last[z][x][y]);
					}
				}

				if (dim->xlen>1)
				{
					if (x==0)
					{
						float S=sqrt(data->dt2/(data->epsilon_r[z][x][y]*epsilon0f*dx))*sqrt(data->dt2/(mu0f*dx));
						data->Ex[z][x][y]=data->Ex[z][x+1][y]+((S-1.0)/(S+1.0))*(data->Ex[z][x+1][y]-data->Ex_last[z][x][y]);
						data->Ey[z][x][y]=data->Ey[z][x+1][y]+((S-1.0)/(S+1.0))*(data->Ey[z][x+1][y]-data->Ey_last[z][x][y]);
						data->Ez[z][x][y]=data->Ez[z][x+1][y]+((S-1.0)/(S+1.0))*(data->Ez[z][x+1][y]-data->Ez_last[z][x][y]);
					}

					if (x==dim->xlen-1)
					{
						float S=sqrt(data->dt2/(data->epsilon_r[z][x][y]*epsilon0f*dx))*sqrt(data->dt2/(mu0f*dx));
						data->Ex[z][x][y]=data->Ex[z][x-1][y]+((S-1.0)/(S+1.0))*(data->Ex[z][x-1][y]-data->Ex_last[z][x][y]);
						data->Ey[z][x][y]=data->Ey[z][x-1][y]+((S-1.0)/(S+1.0))*(data->Ey[z][x-1][y]-data->Ey_last[z][x][y]);
						data->Ez[z][x][y]=data->Ez[z][x-1][y]+((S-1.0)/(S+1.0))*(data->Ez[z][x-1][y]-data->Ez_last[z][x][y]);
					}
				}

				if (dim->ylen>1)
				{
					if (y==0)
					{
						float S=sqrt(data->dt2/(data->epsilon_r[z][x][y]*epsilon0f*dy))*sqrt(data->dt2/(mu0f*dy));
						data->Ex[z][x][y]=data->Ex[z][x][y+1]+((S-1.0)/(S+1.0))*(data->Ex[z][x][y+1]-data->Ex_last[z][x][y]);
						data->Ey[z][x][y]=data->Ey[z][x][y+1]+((S-1.0)/(S+1.0))*(data->Ey[z][x][y+1]-data->Ey_last[z][x][y]);
						data->Ez[z][x][y]=data->Ez[z][x][y+1]+((S-1.0)/(S+1.0))*(data->Ez[z][x][y+1]-data->Ez_last[z][x][y]);
					}

					if (y==dim->ylen-1)
					{
						float S=sqrt(data->dt2/(data->epsilon_r[z][x][y]*epsilon0f*dy))*sqrt(data->dt2/(mu0f*dy));
						data->Ex[z][x][y]=data->Ex[z][x][y-1]+((S-1.0)/(S+1.0))*(data->Ex[z][x][y-1]-data->Ex_last[z][x][y]);
						data->Ey[z][x][y]=data->Ey[z][x][y-1]+((S-1.0)/(S+1.0))*(data->Ey[z][x][y-1]-data->Ey_last[z][x][y]);
						data->Ez[z][x][y]=data->Ez[z][x][y-1]+((S-1.0)/(S+1.0))*(data->Ez[z][x][y-1]-data->Ez_last[z][x][y]);
					}
				}
			}
		}
	}
}

void fdtd_time_step(struct simulation *sim,struct fdtd_data *data)
{
	struct dimensions *dim=&(data->dim);

	cpy_light_zxy_float_no_alloc(dim, &(data->Ez_last), &(data->Ez));
	cpy_light_zxy_float_no_alloc(dim, &(data->Ex_last), &(data->Ex));
	cpy_light_zxy_float_no_alloc(dim, &(data->Ey_last), &(data->Ey));
	cpy_light_zxy_float_no_alloc(dim, &(data->Hz_last), &(data->Hz));
	cpy_light_zxy_float_no_alloc(dim, &(data->Hx_last), &(data->Hx));
	cpy_light_zxy_float_no_alloc(dim, &(data->Hy_last), &(data->Hy));

	data->time+=data->dt;
}

void split_and_run(struct simulation *sim,struct fdtd_data *data,struct device *dev,void *solver)
{
	int jobs=4;
	struct job j;
	struct dimensions *dim=&(data->dim);
	int batch_id=server2_get_next_batch_id(sim,&(sim->server));
	int xyz=0;
	int xyz_max=0;
	int x0=0;
	int x1=dim->xlen;
	int y0=0;
	int y1=dim->ylen;
	int z0=0;
	int z1=dim->zlen;


	int dxyz=0;
	char dir='x';
	if (dim->ylen==1)
	{
		dxyz=dim->xlen/jobs;
		xyz_max=dim->xlen;
		dir='x';
	}else
	if (dim->xlen==1)
	{
		dxyz=dim->ylen/jobs;
		xyz_max=dim->ylen;
		dir='y';
	}else
	if (dim->zlen==1)
	{
		dxyz=dim->ylen/jobs;
		xyz_max=dim->ylen;
		dir='y';
	}

	//printf("%d %d %d %d %d %c\n",dim->xlen,dim->ylen,dim->zlen,dxyz,xyz_max,dir);
	while (xyz<xyz_max)
	{
		if (dir=='x')
		{
			x0=xyz;
			x1=xyz+dxyz;

			if (x1>=dim->xlen)
			{
				x1=dim->xlen;
			}
		}else
		if (dir=='y')
		{
			y0=xyz;
			y1=xyz+dxyz;

			if (y1>=dim->ylen)
			{
				y1=dim->ylen;
			}
		}else
		if (dir=='z')
		{
			z0=xyz;
			z1=xyz+dxyz;

			if (z1>=dim->zlen)
			{
				z1=dim->zlen;
			}
		}


		job_init(sim,&j);
		sprintf(j.name,"fdtd-%d-%d",xyz,xyz+dxyz);
		j.fun=solver;
		j.sim=(void *)sim;
		j.data0=(void *)data;
		j.batch_id=batch_id;

		j.data_int0=x0;
		j.data_int1=x1;
		j.data_int2=y0;
		j.data_int3=y1;
		j.data_int4=z0;
		j.data_int5=z1;
		server2_add_job(sim,&(sim->server),&j);
		xyz=xyz+dxyz;
	}

	server2_run_until_done(sim,&(sim->server),batch_id);

	server2_free_finished_jobs(sim,&(sim->server),batch_id);
}

void fdtd_solve_step(struct simulation *sim,struct fdtd_data *data,struct device *dev)
{

	split_and_run(sim,data,dev,&solve_E);
	fdtd_abc(sim,data);
	fdtd_excitation(sim,data,dev);
	split_and_run(sim,data,dev,&solve_H);


	fdtd_time_step(sim,data);
}


