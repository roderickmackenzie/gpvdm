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

/** @file fdtd_util.c
	@brief Helper functions for FDTD.
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

void fdtd_zero_arrays(struct simulation *sim,struct fdtd_data *data)
{
	fdtd_set_3d_float(data, data->Ex, 0.0);
	fdtd_set_3d_float(data, data->Ey, 0.0);
	fdtd_set_3d_float(data, data->Ez, 0.0);

	fdtd_set_3d_float(data, data->Hx, 0.0);
	fdtd_set_3d_float(data, data->Hy, 0.0);
	fdtd_set_3d_float(data, data->Hz, 0.0);

	fdtd_set_3d_float(data, data->Ex_last, 0.0);
	fdtd_set_3d_float(data, data->Ey_last, 0.0);
	fdtd_set_3d_float(data, data->Ez_last, 0.0);

	fdtd_set_3d_float(data, data->Ex_last_last, 0.0);
	fdtd_set_3d_float(data, data->Ey_last_last, 0.0);
	fdtd_set_3d_float(data, data->Ez_last_last, 0.0);

	fdtd_set_3d_float(data, data->Hx_last, 0.0);
	fdtd_set_3d_float(data, data->Hy_last, 0.0);
	fdtd_set_3d_float(data, data->Hz_last, 0.0);

}


void fdtd_set_lambda(struct simulation *sim,struct fdtd_data *data,struct device *cell,float lambda)
{
	int z;
	int x;
	int y;
	int layer;
	float alpha;
	float n;
	float kappa;

	fdtd_zero_arrays(sim,data);

	data->time=0.0;
	data->step=0;
	data->lambda=lambda;
	data->f=clf/data->lambda;
	data->omega=2.0*3.14159*data->f;

	//float min=1.0/(clf*sqrt(pow(1.0/data->dz,2.0)+pow(1.0/data->dx,2.0)+pow(1.0/data->dy,2.0)));
	//data->dt=1e-18;//min*0.1;
	//

	float min=1.0/(clf*sqrt(pow(1.0/data->dz,2.0)+pow(1.0/data->dx,2.0)+pow(1.0/data->dy,2.0)));
	data->dt=min*0.01;

	printf("dt=%e\n",data->dt);

	data->dt2=data->dt/2.0;
	//fdtd_setup_simulation(sim,data);
	printf(" lambda=%f\n",data->lambda*1e9);

	for (z=0;z<data->zlen;z++)
	{
		for (x=0;x<data->xlen;x++)
		{
			for (y=0;y<data->ylen;y++)
			{
				layer=data->layer[y];
				
				if (layer!=-1)
				{
					n=inter_get_noend(&(cell->my_epitaxy.layer[layer].s.n),data->lambda);
					data->epsilon_r[z][x][y]=pow(n,2.0);
					alpha=inter_get_noend(&(cell->my_epitaxy.layer[layer].s.alpha),data->lambda);
					kappa=(data->lambda*alpha)/(4.0*M_PI);
					data->sigma[z][x][y]=(2.0*n*kappa/mu0f)/(3e8/4/M_PI);	//taken from "optical properties of solids" F. Wooten eq. 2.91
					//printf("%e\n",data->sigma[z][x][y]);	
	//*epsilon0f*sqrtf(powf((1+(kappa*kappa*2)/(data->omega*data->omega*mu0f*epsilon0f)),2.0)-1.0);
				}else
				{
					data->epsilon_r[z][x][y]=1.0;
					data->sigma[z][x][y]=0.0;
				}



			}
		}
	}

}
