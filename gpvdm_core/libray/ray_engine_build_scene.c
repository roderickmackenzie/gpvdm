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

#include <stdio.h>
#include <ray.h>
#include <ray_fun.h>
#include <gpvdm_const.h>
#include <math.h>
#include <stdlib.h>
#include <cal_path.h>
#include <log.h>
#include <device.h>
#include <inp.h>
#include <util.h>
#include <triangles.h>
#include <memory.h>
#include <epitaxy_struct.h>
#include <epitaxy.h>
#include <device_fun.h>


/** @file ray_build_scene.c
	@brief Set up the simulation window for the ray tracer
*/


void ray_build_scene(struct simulation *sim,struct device *dev,struct image *my_image,struct epitaxy *my_epitaxy)
{

	int layer;
	double xlen=dev->xlen;
	double zlen=dev->zlen;
	double dx=xlen*0.01;

	double start_z=zlen/2.0;
	double start_x=xlen/2.0;

	double device_height=epitaxy_get_optical_length(my_epitaxy);
	double sim_window_top=device_height*2.0;
	double scene_y0=device_height*-4.0;

	struct object *obj;

	my_image->y_escape_level=-device_height*1.1;

	double scene_dx=xlen+dx*2.0;
	double scene_dy=(sim_window_top-scene_y0);
	double scene_dz=dev->zlen;

	double camera_dx=0.0;
	double camera_dz=0.0;

	double camera_y=0.0;
	double camera_x=0.0;
	double camera_z=0.0;

	//dump_plane_to_file("l.dat",my_image);
	//exit(0);


	double x_start=dx+dx/2.0;
	double x_stop=dx+xlen-dx/2.0;
	dx=(x_stop-x_start)/((double)my_image->n_start_rays);

	my_image->n_start_rays=0;

	if (my_image->ray_xsrc!=-1.0)
	{
		my_image->start_rays[my_image->n_start_rays].x=my_image->ray_xsrc;
		my_image->start_rays[my_image->n_start_rays].y=my_image->ray_ysrc;
		my_image->start_rays[my_image->n_start_rays].z=start_z;
		my_image->n_start_rays++;
	}else
	{
		for (layer=0;layer<my_epitaxy->layers;layer++)
		{
			//printf("%d %Le %Le\n",layer,my_epitaxy->layer[layer].y_start,dev->my_epitaxy.device_start);
			//getchar();

			if (my_epitaxy->layer[layer].pl_enabled==TRUE)
			{
				//layer=0;
				my_image->start_rays[my_image->n_start_rays].x=start_x;
				my_image->start_rays[my_image->n_start_rays].y=my_epitaxy->layer[layer].y_start+my_epitaxy->layer[layer].width/2.0;
				//printf("%le\n",my_image->start_rays[my_image->n_start_rays].y);
				//getchar();
				my_image->start_rays[my_image->n_start_rays].z=start_z;
				my_image->n_start_rays++;
				//break;
			}
		}

	}


	if (my_image->viewpoint_enabled==TRUE)
	{
		//double camera_dy=scene_dy*0.05;
		camera_dx=scene_dx*0.9;
		camera_dz=scene_dz*0.9;

		camera_y=scene_y0+scene_dy*0.1;
		camera_x=camera_dx*0.05;
		camera_z=camera_dz*0.05;

		obj=add_plane(dev,camera_x,camera_y, camera_z, camera_dx,camera_dz,RAY_VIEWPOINT);

		obj->epi_layer=-1;
		strcpy(obj->name,"viewpoint");

		dim_set_simple_mesh_z(&(my_image->viewpoint_dim), camera_z, camera_z+camera_dz);
		dim_set_simple_mesh_x(&(my_image->viewpoint_dim), camera_x, camera_x+camera_dx);

		//printf("two\n");
		//getchar();
	}


	//struct shape *s;

	//for (i=0;i<dev->ncontacts;i++)
	//{
	//	ray_add_shape_to_scene(struct simulation *sim,struct image *my_image,struct shape *s,double y_stop);
	//	//&(in->contacts[i].shape)
	//}

	device_dump_world_to_file(sim,dev,"all_triangles.dat");
	//printf("%le %le %le\n",my_image->start_rays[0].x,my_image->start_rays[0].y,my_image->start_rays[0].z);
	//getchar();
	//dump_plane(my_image);
	//dump_plane_to_file(my_image);
	//exit(0);
}
