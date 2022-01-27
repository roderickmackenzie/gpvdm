//
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solardevs.
// The model can simulate OLEDs, Perovskite devs, and OFETs.
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

/** @file fdtd_mesh.c
	@brief Setup the FDTD mesh.
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
#include <epitaxy_struct.h>
#include <epitaxy.h>
#include <world.h>
#include "vec.h"
#include <ray_fun.h>

int fdtd_world_x_to_mesh(struct fdtd_data *data,float x)
{
	struct dimensions *dim=&(data->dim);
	int pos=(x-data->world_min.x)/dim->dx;
	return (int)pos;
}

int fdtd_world_y_to_mesh(struct fdtd_data *data,float y)
{
	struct dimensions *dim=&(data->dim);
	int pos=(y-data->world_min.y)/dim->dy;
	return (int)pos;
}

int fdtd_world_z_to_mesh(struct fdtd_data *data,float z)
{
	struct dimensions *dim=&(data->dim);
	int pos=(z-data->world_min.z)/dim->dz;
	return (int)pos;
}

void fdtd_mesh(struct simulation *sim,struct fdtd_data *data,struct device *dev)
{
	int z=0;
	int x=0;
	int y=0;
	float Lx=0.0;
	float Ly=0.0;
	float Lz=0.0;
	float zpos=0.0;
	float xpos=0.0;
	float ypos=0.0;
	struct dimensions *dim=&(data->dim);
	struct vec v;
	struct vec max;
	struct object *obj=NULL;

	world_size(sim,&(data->world_min),&max,&(dev->w), dev);
	vec_print(&(data->world_min));
	vec_print(&max);

	Lx=max.x-data->world_min.x;
	Ly=max.y-data->world_min.y;
	Lz=max.z-data->world_min.z;


	data->excitation_mesh_point_x=Lx/2.0;
	data->excitation_mesh_point_y=Ly/2.0;

	dim->dz=Lz/((float)dim->zlen);
	dim->dx=Lx/((float)dim->xlen);
	dim->dy=Ly/((float)dim->ylen);

	printf_log(sim,"dx=%lf nm, dy=%lf nm, dz=%lf nm dt=%le %le\n",dim->dx*1e9,dim->dy*1e9,dim->dz*1e9,data->dt);


	// The /4 is to make sure we don't go outside of the box at the end.
	zpos=data->world_min.z+dim->dz/40.0;
	xpos=data->world_min.x+dim->dx/40.0;
	ypos=data->world_min.y+dim->dy/40.0;

	for (z=0;z<dim->zlen;z++)
	{
		zpos+=dim->dz/2.0;
		dim->z[z]=zpos;
		zpos+=dim->dz/2.0;
	}

	for (x=0;x<dim->xlen;x++)
	{
		xpos+=dim->dx/2.0;
		dim->x[x]=xpos;
		xpos+=dim->dx/2.0;
	}

	for (y=0;y<dim->ylen;y++)
	{
		ypos+=dim->dy/2.0;
		dim->y[y]=ypos;
		ypos+=dim->dy/2.0;
	}




	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				v.z=dim->z[z];
				v.x=dim->x[x];
				v.y=dim->y[y];
				//printf("%le %le %le\n",v.z,v.x,v.y);
				obj=ray_obj_search_xyz(sim,dev,&v);
				data->obj[z][x][y]=obj;
			}
		}
	}
}
