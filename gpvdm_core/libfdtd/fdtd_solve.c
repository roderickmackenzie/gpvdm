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

#include "vec.h"


void solve_E(struct simulation *sim,struct fdtd_data *data)
{
	int z=0;
	int x=0;
	int y=0;

	float C=0.0;

	float dHzdy=0.0;
	float dHydz=0.0;

	float dHxdz=0.0;
	float dHzdx=0.0;

	float dHydx=0.0;
	float dHxdy=0.0;

	float Ez_last=0.0;
	float Ex_last=0.0;
	float Ey_last=0.0;

	float sigma=0.0;

	for (z=0;z<data->zlen;z++)
	{
		for (x=1;x<data->xlen;x++)
		{
			for (y=0;y<data->ylen;y++)
			{
				C=data->dt2/(epsilon0f*data->epsilon_r[z][x][y]);
				sigma=data->sigma[z][x][y];
			/*	float damp=1e4;
				if (x<5)
				{
					sigma=damp;
				}

				if (x>data->xlen-5)
				{
					sigma=damp;
				}

				if (y<5)
				{
					sigma=damp;
				}

				if (y>data->ylen-5)
				{
					sigma=damp;
				}*/

				if (y!=0)
				{
					dHzdy=(data->Hz[z][x][y]-data->Hz[z][x][y-1])/data->dy;
					dHxdy=(data->Hx[z][x][y]-data->Hx[z][x][y-1])/data->dy;
				}else
				{
					dHzdy=(data->Hz[z][x][y+1]-data->Hz[z][x][y])/data->dy;
					dHxdy=(data->Hx[z][x][y+1]-data->Hx[z][x][y])/data->dy;
				}

				if (x!=0)
				{
					dHzdx=(data->Hz[z][x][y]-data->Hz[z][x-1][y])/data->dx;
					dHydx=(data->Hy[z][x][y]-data->Hy[z][x-1][y])/data->dx;
				}else
				{
					dHzdx=(data->Hz[z][x+1][y]-data->Hz[z][x][y])/data->dx;
					dHydx=(data->Hy[z][x+1][y]-data->Hy[z][x][y])/data->dx;
				}
				dHydz=0.0;

				dHxdz=0.0;


				Ez_last=data->Ez_last[z][x][y];
				Ex_last=data->Ex_last[z][x][y];
				Ey_last=data->Ey_last[z][x][y];

				data->Ex[z][x][y]=C*(dHzdy-dHydz)-Ex_last*(sigma*C-1.0);
				data->Ey[z][x][y]=C*(dHxdz-dHzdx)-Ey_last*(sigma*C-1.0);
				data->Ez[z][x][y]=C*(dHydx-dHxdy)-Ez_last*(sigma*C-1.0);

			}
		}
	}

	/*
	for (z=0;z<data->zlen;z++)
	{

		for (x=1;x<data->xlen-1;x++)
		{
			for (y=1;y<(data->ylen-1);y++)
			{
				
				if (x==1)
				{
					float Ex_last_l=data->Ex_last[z][x-1][y];
					float Ey_last_l=data->Ey_last[z][x-1][y];
					float Ez_last_l=data->Ez_last[z][x-1][y];
					data->Ex[z][x-1][y]=data->Ex_last[z][x][y]+((clf*data->dt2-data->dx)/(clf*data->dt2+data->dx))*(data->Ex[z][x][y]-Ex_last_l);
					data->Ey[z][x-1][y]=data->Ey_last[z][x][y]+((clf*data->dt2-data->dx)/(clf*data->dt2+data->dx))*(data->Ey[z][x][y]-Ey_last_l);
					data->Ez[z][x-1][y]=data->Ez_last[z][x][y]+((clf*data->dt2-data->dz)/(clf*data->dt2+data->dx))*(data->Ez[z][x][y]-Ez_last_l);
				}
				
				if (x==data->xlen-2)
				{
					float Ex_last_r=data->Ex_last[z][x+1][y];
					float Ey_last_r=data->Ey_last[z][x+1][y];
					float Ez_last_r=data->Ez_last[z][x+1][y];
					data->Ex[z][x+1][y]=data->Ex_last[z][x][y]+((clf*data->dt2-data->dx)/(clf*data->dt2+data->dx))*(data->Ex[z][x][y]-Ex_last_r);
					data->Ey[z][x+1][y]=data->Ey_last[z][x][y]+((clf*data->dt2-data->dx)/(clf*data->dt2+data->dx))*(data->Ey[z][x][y]-Ey_last_r);
					data->Ez[z][x+1][y]=data->Ez_last[z][x][y]+((clf*data->dt2-data->dx)/(clf*data->dt2+data->dx))*(data->Ez[z][x][y]-Ez_last_r);
				}
				

				if (y==1)
				{
					float Ex_last_d=data->Ex_last[z][x][y-1];
					float Ey_last_d=data->Ey_last[z][x][y-1];
					float Ez_last_d=data->Ez_last[z][x][y-1];
					data->Ex[z][x][y-1]=data->Ex_last[z][x][y]+((clf*data->dt2-data->dy)/(clf*data->dt2+data->dy))*(data->Ex[z][x][y]-Ex_last_d);
					data->Ey[z][x][y-1]=data->Ey_last[z][x][y]+((clf*data->dt2-data->dy)/(clf*data->dt2+data->dy))*(data->Ey[z][x][y]-Ey_last_d);
					data->Ez[z][x][y-1]=data->Ez_last[z][x][y]+((clf*data->dt2-data->dy)/(clf*data->dt2+data->dy))*(data->Ez[z][x][y]-Ez_last_d);
				}

				if (y==(data->ylen-2))
				{
					float Ex_last_u=data->Ex_last[z][x][y+1];
					float Ey_last_u=data->Ey_last[z][x][y+1];
					float Ez_last_u=data->Ez_last[z][x][y+1];
					//printf("%ld %le %le\n",dt2,dx,clight);
			
					data->Ex[z][x][y+1]=data->Ex_last[z][x][y]+((clf*data->dt2-data->dy)/(clf*data->dt2+data->dy))*(data->Ex[z][x][y]-Ex_last_u);
					data->Ey[z][x][y+1]=data->Ey_last[z][x][y]+((clf*data->dt2-data->dy)/(clf*data->dt2+data->dy))*(data->Ey[z][x][y]-Ey_last_u);
					data->Ez[z][x][y+1]=data->Ez_last[z][x][y]+((clf*data->dt2-data->dy)/(clf*data->dt2+data->dy))*(data->Ez[z][x][y]-Ez_last_u);

				}
				
			}
		}

	}*/
}



void solve_H(struct simulation *sim,struct fdtd_data *data)
{
int z;
int x;
int y;

float C=0.0;
float dEydz=0.0;
float dEzdy=0.0;

float dEzdx=0.0;
float dExdz=0.0;

float dExdy=0.0;
float dEydx=0.0;

float Hz_last=0.0;
float Hx_last=0.0;
float Hy_last=0.0;

for (z=0;z<data->zlen;z++)
{
	for (x=0;x<data->xlen;x++)
	{
		for (y=0;y<data->ylen-1;y++)
		{

			C=data->dt2/mu0f;

			dEydz=0.0;
			if (y!=data->ylen-1)
			{
				dEzdy=(data->Ez[z][x][y+1]-data->Ez[z][x][y])/data->dy;
				dExdy=(data->Ex[z][x][y+1]-data->Ex[z][x][y])/data->dy;

			}else
			{
				dEzdy=(data->Ez[z][x][y]-data->Ez[z][x][y-1])/data->dy;
				dExdy=(data->Ex[z][x][y]-data->Ex[z][x][y-1])/data->dy;
			}

			if (x!=data->xlen-1)
			{
				dEzdx=(data->Ez[z][x+1][y]-data->Ez[z][x][y])/data->dx;
				dEydx=(data->Ey[z][x+1][y]-data->Ey[z][x][y])/data->dx;
			}else
			{
				dEzdx=(data->Ez[z][x][y]-data->Ez[z][x-1][y])/data->dx;
				dEydx=(data->Ey[z][x][y]-data->Ey[z][x-1][y])/data->dx;

			}
			dExdz=0.0;

			Hz_last=data->Hz_last[z][x][y];
			Hx_last=data->Hx_last[z][x][y];
			Hy_last=data->Hy_last[z][x][y];

			data->Hx[z][x][y]=(dEydz-dEzdy)*C+Hx_last;
			data->Hy[z][x][y]=(dEzdx-dExdz)*C+Hy_last;
			data->Hz[z][x][y]=(dExdy-dEydx)*C+Hz_last;


			if ((isnan(data->Hz[z][x][y])!=0)||(isnan(data->Hx[z][x][y])!=0)||(isnan(data->Hy[z][x][y])!=0))
			{
				printf("nan detected %f %f %f\n",data->Hz[z][x][y],data->Hx[z][x][y],data->Hy[z][x][y]);
				exit(0);
			}

			if ((isinf(data->Hz[z][x][y])!=0)||(isinf(data->Hx[z][x][y])!=0)||(isinf(data->Hy[z][x][y])!=0))
			{
				printf("inf detected %f %f %f\n",data->Hz[z][x][y],data->Hx[z][x][y],data->Hy[z][x][y]);
				printf("%e %e %e %e %e\n", data->Hx_last[z][x][y],data->Ez[z][x][y+1],data->Ez[z][x][y],data->Ey[z][x+1][y],data->Ey[z][x][y]);
				exit(0);
			}
		}
	}

}

}

void fdtd_excitation(struct simulation *sim,struct fdtd_data *data)
{
	int z=0;
	int x=data->excitation_mesh_point_x;
	int y=data->excitation_mesh_point_y;
	//for (j=0;j<data->ylen;j++)
	{
		//for (i=0;i<data->zlen;i++)
		{
			//if (j==data->excitation_mesh_point)
			{
				//if ((y[j]>src_start)&&(y[j]<src_stop))
				{
					float start=data->y_mesh[1];
					float a=0.0;
					float b=0.0;
					float c=0.0;
					float phi=0.0;
					float theta=0.0;


					float dot=0.0;
					phi=0.0;
					theta=90.0;

					float shift=-2.0;
					a=sin(theta*(2.0*M_PI/360.0))*cos(phi*(2.0*M_PI/360.0));
					b=sin(theta*(2.0*M_PI/360.0))*sin(phi*(2.0*M_PI/360.0));
					c=cos(theta*(2.0*M_PI/360.0));

					dot=tan(2.0*M_PI*(shift)/360.0)*(data->y_mesh[y]-start)*2.0*M_PI/data->lambda;
					float mod=1.0;
					//dot=0.0;
					data->Ex[z][x][y]=mod*a*sin(dot-data->time*data->omega);
					data->Ey[z][x][y]=mod*b*sin(dot-data->time*data->omega);
					data->Ez[z][x][y]=mod*c*sin(dot-data->time*data->omega);
				}
			}
		}
	}


///////////////////////////

}

void fdtd_time_step(struct simulation *sim,struct fdtd_data *data)
{
	fdtd_memcpy(data, data->Ez_last, data->Ez);
	fdtd_memcpy(data, data->Ex_last, data->Ex);
	fdtd_memcpy(data, data->Ey_last, data->Ey);

	fdtd_memcpy(data, data->Hz_last, data->Hz);
	fdtd_memcpy(data, data->Hx_last, data->Hx);
	fdtd_memcpy(data, data->Hy_last, data->Hy);

	data->time+=data->dt;
}
void fdtd_solve_step(struct simulation *sim,struct fdtd_data *data)
{
	FILE * conv;

	int ret=-1;

	if (data->use_gpu==TRUE)
	{
		ret=fdtd_opencl_solve_step(sim,data);
	}

	if (ret==-1)
	{
		solve_E(sim,data);
		fdtd_excitation(sim,data);
		//printf("power %e\n",fdtd_power_zxy(sim,data,0, data->excitation_mesh_point, data->excitation_mesh_point));
		conv=fopen("conv.dat","a");
		if (conv!=NULL)
		{
			fprintf(conv," %d %e %e %e\n",data->step,fdtd_power_zxy(sim,data,0, data->excitation_mesh_point_x, data->excitation_mesh_point_y),fdtd_power_y(sim,data,  data->ylen-data->ylen/4),fdtd_test_conv(sim,data));
			fclose(conv);
		}
		solve_H(sim,data);
	}

	//getchar();
	fdtd_time_step(sim,data);
	//usleep(100);
}


