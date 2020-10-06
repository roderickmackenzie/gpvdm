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


/** @file device_obj.c
	@brief Build the pointer array for the device
*/

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#include <stdio.h>
#include <device.h>
#include <string.h>
#include <dump.h>
#include <mesh.h>
#include <ray_fun.h>
#include <newton_tricks.h>
#include <memory.h>
#include <circuit.h>
#include <shape.h>
#include <heat.h>
#include <heat_fun.h>
#include <gpvdm_const.h>
#include <lib_fxdomain.h>
#include <fxdomain_fun.h>
#include <perovskite.h>
#include <sim.h>
#include <device_fun.h>
#include <epitaxy.h>
#include <light_fun.h>
#include <enabled_libs.h>
#include <lang.h>
#include <log.h>

static int unused __attribute__((unused));
static char* unused_pchar __attribute__((unused));

void device_obj_dump(struct simulation *sim,struct device *dev)
{
	struct dimensions *dim=&(dev->ns.dim);
	struct dat_file buf;

	buffer_init(&buf);

	buffer_malloc(&buf);
	dim_info_to_buf(&buf,dim);
	strcpy(buf.title,_("Electrical objects"));
	strcpy(buf.type,"zxy-d");
	strcpy(buf.data_label,_("Electrical objects"));
	strcpy(buf.data_units,"au");
	buf.x=dim->xlen;
	buf.y=dim->ylen;
	buf.z=dim->zlen;
	buffer_add_info(sim,&buf);
	buffer_add_zxy_object_uid(sim,&buf,dev->obj_zxy, dim);
	buffer_dump_path(sim,"","electrical_object_uid.dat",&buf);
	buffer_free(&buf);

}

void device_build_obj_pointer_array(struct simulation *sim,struct device *dev)
{
	printf_log(sim,"Device: Building pointer array\n");
	int x=0;
	int y=0;
	int z=0;
	int l=0;
	struct object *obj;
	struct dimensions *dim=&(dev->ns.dim);
	struct epitaxy *epi=&(dev->my_epitaxy);
	struct vec v;
	long double mid_point=0.0;
	long double device_length=0.0;
	device_length=epitaxy_get_total_length_y(epi);

	//printf("%d\n",dev->circuit_simulation);
	//getchar();

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (l=0;l<epi->layers;l++)
			{
				mid_point=epi->layer[l].y_start+epi->layer[l].width/2.0;

				v.z=dim->zmesh[z];
				v.x=dim->xmesh[x];
				v.y=mid_point;

				obj=ray_obj_search_xyz(sim,dev,&v);

				if (obj==NULL)
				{
					ewe(sim,"No object\n");
					getchar();
				}

				dev->obj_zx_layer[z][x][l]=obj;
				//printf("%s\n",obj->name);
			}

			for (y=0;y<dim->ylen;y++)
			{
				if (dev->circuit_simulation==FALSE)
				{
					v.z=dim->zmesh[z];
					v.x=dim->xmesh[x];
					v.y=epi->device_start+dim->ymesh[y];

				}else
				{
					if (epi->device_start+dim->ymesh[y]>device_length)
					{
						l=epi->layers-1;
					}else
					{
						l=epitaxy_get_layer(epi,epi->device_start+dim->ymesh[y]);
					}
					mid_point=epi->layer[l].y_start+epi->layer[l].width/2.0;

					v.z=dim->zmesh[z];
					v.x=dim->xmesh[x];
					v.y=mid_point;

					//obj=ray_obj_search_xyz(sim,dev,&v);
				}

				obj=ray_obj_search_xyz(sim,dev,&v);

				if (obj==NULL)
				{
					ewe(sim,"No object\n");
					getchar();
				}

				dev->obj_zxy[z][x][y]=obj;

			}

			//getchar();
		}
	}

	device_obj_dump(sim,dev);
}

