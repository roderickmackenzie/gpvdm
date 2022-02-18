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
	return;
	struct dimensions *dim=&(dev->ns.dim);
	struct dat_file buf;

	dat_file_init(&buf);

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
	if (dev->electrical_simulation_enabled==FALSE)
	{
		return;
	}

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

	//v.z=dim->z[0];
	//v.x=dim->x[dim->xlen/2];
	/*for (y=0;y<dim->ylen;y++)
	{
		v.y=dim->y[y];
		obj=ray_obj_search_xyz(sim,dev,&v);
		printf("%d %s\n",y,obj->name);
	}*/

	//getchar();
	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (l=0;l<epi->layers;l++)
			{
				mid_point=epi->layer[l].y_start+epi->layer[l].width/2.0;

				v.z=dim->z[z];
				v.x=dim->x[x];
				v.y=mid_point;

				obj=ray_obj_search_xyz(sim,dev,&v);

				if (obj==NULL)
				{
					ewe(sim,"No object\n");
					getchar();
				}
				//printf("%d %d %s\n",z,x,obj->name);
				dev->obj_zx_layer[z][x][l]=obj;
				//printf("%s\n",obj->name);
			}

			for (y=0;y<dim->ylen;y++)
			{
				if (dev->circuit_simulation==FALSE)
				{
					v.z=dim->z[z];
					v.x=dim->x[x];
					v.y=epi->device_start+dim->y[y];

				}else
				{
					if (epi->device_start+dim->y[y]>device_length)
					{
						l=epi->layers-1;
					}else
					{
						l=epitaxy_get_layer(epi,epi->device_start+dim->y[y]);
					}
					mid_point=epi->layer[l].y_start+epi->layer[l].width/2.0;

					v.z=dim->z[z];
					v.x=dim->x[x];
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
				//printf("%d %d %s\n",x,y,obj->name);
			}

			//getchar();
		}
	}

	device_obj_dump(sim,dev);
}

