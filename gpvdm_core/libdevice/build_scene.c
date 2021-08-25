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
#include <enabled_libs.h>
#include <world.h>
#include <detector.h>

/** @file device_build_scene.c
	@brief Build the scene with triangles.
*/


void device_build_scene(struct simulation *sim,struct device *dev)
{
	int l;
	int i;
	int add_layer;
	double xlen=dev->xlen;
	double zlen=dev->zlen;
	struct epitaxy *epi = &(dev->my_epitaxy);
	struct world *w=&(dev->w);
	struct vec min;
	struct vec max;
	world_size(sim,&min,&max,&(dev->w), dev);
	vec_print(&min);
	vec_print(&max);
	int c;

	struct shape *s;
	struct shape *contact_shape;

	struct epi_layer *layer;

	s=&(dev->big_box);
	shape_init(sim,s);

	strcpy(s->shape_type,"box");
	strcpy(s->name,"big_box");
	strcpy(s->optical_material,"generic/air");

	#ifdef libcircuit_enabled
		strcpy(s->com.component,"none");
	#endif

	s->z0=min.z;
	s->x0=min.x;
	s->y0=min.y;

	s->dz=max.z-min.z;
	s->dx=max.x-min.x;
	s->dy=max.y-min.y;

	s->dy_padding=0.0;
	s->dx_padding=0.0;
	s->dz_padding=0.0;

	shape_load_materials(sim,s);
	device_add_shape_to_world(sim,dev,s);

	for (l=0;l<epi->layers;l++)
	{
		add_layer=TRUE;
		layer=&(epi->layer[l]);
		s=&(layer->s);

		if (s->enabled==FALSE)
		{
			add_layer=FALSE;			
		}

		if (layer->layer_type==LAYER_CONTACT)
		{

			if (dev->ns.dim.xlen>1)
			{
				for (c=0;c<dev->ncontacts;c++)
				{
					//printf("a> %d %s %d\n",l,dev->contacts[c].name,dev->contacts[c].position);
					
					if ((dev->contacts[c].position==TOP)&&(l==0))
					{
						contact_shape=&(dev->contacts[c].shape);
						contact_shape->dy=layer->width;
						contact_shape->dz=dev->zlen;
						//contact_shape->y0=layer->y_start;
						//contact_shape->x0=0.0;
						contact_shape->z0=0.0;
						contact_shape->nx=1;
						contact_shape->nz=1;
						contact_shape->epi_index=l;
						//if (contact_shape->flip_y==FALSE)
						//{
							contact_shape->y0=layer->y_start;		//Starting from zero
						//}else
						//{
						//	contact_shape->y0=layer->y_stop;		//Starting from top of layer
						//}
						//printf("%d\n",dev->contacts[c].shape.flip_y);
						//getchar();
						//dev->contacts[c].shape.flip_y=FALSE;

						contact_shape->dy_padding=0.0;
						contact_shape->dx_padding=0.0;
						contact_shape->dz_padding=0.0;
						device_add_shape_to_world(sim,dev,contact_shape);		//,epi->layer[l].y_stop

						add_layer=FALSE;
					}

					if ((dev->contacts[c].position==BOTTOM)&&(l==epi->layers-1))
					{
						contact_shape=&(dev->contacts[c].shape);
	//							printf("b> %d %s %d\n",l,dev->contacts[c].name,dev->contacts[c].position);
						contact_shape->dy=layer->width;
						contact_shape->dz=dev->zlen;
						contact_shape->z0=0.0;
						contact_shape->y0=layer->y_start;
						//dev->contacts[c].shape.x0=0.0;
						contact_shape->nx=1;
						contact_shape->nz=1;
						contact_shape->epi_index=l;

						contact_shape->y0=layer->y_start;		//Starting from zero


						contact_shape->dy_padding=0.0;
						contact_shape->dx_padding=0.0;
						contact_shape->dz_padding=0.0;

						device_add_shape_to_world(sim,dev,contact_shape);	//,epi->layer[l].y_stop
						add_layer=FALSE;
					}

				}
			
			}
		}


		if (add_layer==TRUE)
		{
			//obj=add_box(dev,0.0,layer->y_start,0.0,xlen,fabs(layer->width),zlen,RAY_OBJECT);
			//obj->epi_layer=l;
			//obj->s=&(layer->s);
			//strcpy(obj->name,layer->s.name);layer->s.name
			s->x0=0.0;
			s->dx=xlen;
			s->dz=zlen;

			s->nx=1;
			s->nz=1;

			s->epi_index=l;
			//if (s->flip_y==FALSE)
			//{
				s->y0=layer->y_start;		//Starting from zero
			//}else
			//{
			//	s->y0=layer->y_stop;		//Starting from top of layer
			//}
			//printf("name= %s %s %d %d %d\n",s->name,s->shape_type,add_layer,l,s->enabled);
			//getchar();
			device_add_shape_to_world(sim,dev,s);

		}
	}
	


	for (c=0;c<dev->ncontacts;c++)
	{
		if ((dev->contacts[c].position==LEFT))
		{
			device_add_shape_to_world(sim,dev,&(dev->contacts[c].shape));
		}
	}

	//printf(">>>>>>>>>>>>>>>>>>>\n");

	for (l=0;l<epi->layers;l++)
	{
		for (i=0;i<epi->layer[l].nshape;i++)
		{
			s=&epi->layer[l].shapes[i];
			s->epi_index=l;
			device_add_shape_to_world(sim,dev,s);
		}
	}


	for (i=0;i<w->items;i++)
	{
		s=&(w->shapes[i]);
		s->epi_index=-1.0;
		device_add_shape_to_world(sim,dev,s);
	}

	if ((sim->log_level==log_level_disk)||(sim->log_level==log_level_screen_and_disk))
	{
		device_dump_world_to_file(sim,dev,"device.dat");
		device_world_stats(sim,dev);
	}

	dectors_add_to_scene(sim,w,&(dev->my_image),&min, &max);
}
