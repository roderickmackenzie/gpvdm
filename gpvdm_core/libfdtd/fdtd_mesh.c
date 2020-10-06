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
