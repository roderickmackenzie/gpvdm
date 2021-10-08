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

#include "vec.h"


void fdtd_mesh(struct simulation *sim,struct fdtd_data *data,struct device *cell)
{
	float device_ysize=(float)epitaxy_get_optical_length(&cell->my_epitaxy);
	data->ysize=device_ysize+400e-9;

	data->xsize=data->xsize*device_ysize;

	float device_start=(float)epitaxy_get_device_start(&(cell->my_epitaxy));
	float device_stop=(float)epitaxy_get_device_stop(&(cell->my_epitaxy));

	float start_y=device_start+(device_stop-device_start)/2.0;

	data->excitation_mesh_point_x=data->xlen/2;
	data->excitation_mesh_point_y=data->ylen*(start_y/data->ysize);



	data->dz=data->zsize/((float)data->zlen);
	data->dx=data->xsize/((float)data->xlen);
	data->dy=data->ysize/((float)data->ylen);


	printf ("dy=%lf nm, dz=%lf nm dt=%le %le\n",data->dy*1e9,data->dz*1e9,data->dt,data->time);

	int z=0;
	int x=0;
	int y=0;

	float zpos=data->dz/2.0;
	float xpos=data->dx/2.0;
	float ypos=data->dy/2.0;

	for (z=0;z<data->zlen;z++)
	{
		data->z_mesh[z]=zpos;
		zpos+=data->dz;
	}

	for (x=0;x<data->xlen;x++)
	{
		data->x_mesh[x]=xpos;
		xpos+=data->dx;
	}

	for (y=0;y<data->ylen;y++)
	{
		data->y_mesh[y]=ypos;
		data->layer[y]=epitaxy_get_layer(&cell->my_epitaxy,ypos);
		ypos+=data->dy;
	}


}
