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

int get_next_worker_threads(struct image *my_image)
{
	return -1;
}

void wait_for_threads(struct image *my_image)
{
}


struct thread_data
{
	struct simulation *sim;
	struct device *dev;
	double z;
	double mag;
	struct ray_worker *worker;
};

void *ray_thread_solve(void *in_data)
{
	//printf("I get to here!!!\n");
	return 0;
}

void ray_solve_all(struct simulation *sim,struct device *dev)
{
}

double get_rand()
{
	double r=0.0;
	r = rand();
	r=(double)r/(double)RAND_MAX;
	return r;
}

void ray_solve(struct simulation *sim,struct device *dev, double x0, double y0, double z0,double mag,struct ray_worker *worker)
{


}

void ray_dump_shapshots(struct simulation *sim,struct device *dev, struct image *my_image ,struct ray_worker *worker,int layer)
{
	struct dat_file buf;
	int ret=0;
	char name[PATH_MAX];
	char out_dir[PATH_MAX];
	char temp[200];

	sprintf(out_dir,"%s/ray_trace",get_output_path(dev));

	ret=snprintf(name,PATH_MAX,"%s/light_ray_%d_%.0f.dat",out_dir,layer,(my_image->lam[worker->l]*1e9));
	if (ret<0)
	{
		ewe(sim,"lib ray solve.c: sprintf error\n");
	}
	dump_plane_to_file(sim,name,my_image,dev);

	if (my_image->dump_snapshots==TRUE)
	{
	
		dump_make_snapshot_dir(sim,out_dir,dev->output_path ,"snapshots", dev->snapshot_number);

		buffer_init(&buf);
		buffer_malloc(&buf);

		sprintf(temp,"{\n");
		buffer_add_string(&buf,temp);

		sprintf(temp,"\t\"wavelength\":%le,\n",my_image->lam[worker->l]);
		buffer_add_string(&buf,temp);

		sprintf(temp,"}");
		buffer_add_string(&buf,temp);

		buffer_dump_path(sim,out_dir,"data.json",&buf);
		buffer_free(&buf);

		ret=snprintf(name,PATH_MAX,"%s/RAY_light_beams_%d_%.0f.dat",out_dir,layer,(my_image->lam[worker->l]*1e9));
		if (ret<0)
		{
			ewe(sim,"lib ray solve.c: sprintf error\n");
		}

		dump_plane_to_file(sim,name,my_image,dev);
	}
}
