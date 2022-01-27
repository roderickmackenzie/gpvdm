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

#include <enabled_libs.h>
#include <stdio.h>
#include <ray.h>
#include <gpvdm_const.h>
#include <math.h>
#include <stdlib.h>
#include <cal_path.h>
#include <log.h>
#include <ray_fun.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dump_ctrl.h>
#include <gui_hooks.h>
#include <util.h>
#include <memory.h>
#include <epitaxy_struct.h>
#include <epitaxy.h>
#include <dump.h>
#include <device_fun.h>
#include <timer.h>

	#include <pthread.h>

/** @file solve.c
	@brief This will call the ray tracer for the standard case.
*/

void ray_benchmark_reset(struct simulation *sim,struct image *in)
{
	in->tot_rays=0;
	in->start_time=timer_get_time(0);
}

void ray_benchmark_print(struct simulation *sim,struct image *in)
{
	double dt=(timer_get_time(0)-in->start_time);
	printf_log(sim,"rays %le rays/s\n",((double)in->tot_rays)/dt);
}




/*#ifdef windows
DWORD WINAPI ray_thread_solve(void *in_data)
#else
void *ray_thread_solve(void *in_data)
#endif
{
	//printf("I get to here!!!\n");
	#ifndef gpvdm_open

	struct thread_data *tdata=in_data;
	struct simulation *sim=tdata->sim;
	struct device *dev=tdata->dev;
	struct dimensions *dim=&dev->ns.dim;
	struct image *my_image=&(dev->my_image);
	double mag=0.0;
	long double max=0;
	long double min=0;
	int x=0;
	int y=0;
	int z=0;

	min=zxy_min_gdouble(dim, dev->Rnet);
	max=zxy_max_gdouble(dim, dev->Rnet);

	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			mag=(double)(dev->Rnet[z][x][y]-min)/max;
			ray_solve(sim,dev,dim->xmesh[x],dim->ymesh[y],tdata->z, mag,tdata->worker);
		}
	}
	ray_dump_shapshots(sim,dev, my_image ,tdata->worker,0);
	ray_reset(tdata->worker);
	tdata->worker->working=FALSE;
	#endif
	return 0;
}*/

void ray_solve_all(struct simulation *sim,struct device *dev)
{

}

void ray_solve(struct simulation *sim,struct device *dev, struct ray_src *raysrc,double mag,struct ray_worker *worker, int *rays_shot)
{


}

