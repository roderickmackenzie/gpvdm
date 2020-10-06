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

/** @file mesh.c
	@brief Performs meshing for the heat model.
*/

#include "util.h"
#include "gpvdm_const.h"
#include "heat.h"
#include "device.h"
#include "gpvdm_const.h"
#include "dump.h"
#include "config.h"
#include "inp.h"
#include "util.h"
#include "hard_limit.h"
#include "lang.h"
#include "log.h"
#include "memory.h"
#include <heat_fun.h>
#include <epitaxy.h>
#include <ray_fun.h>

static int unused __attribute__((unused));



void heat_build_obj_pointer_array(struct simulation *sim,struct heat *thermal, struct device *dev)
{
	printf_log(sim,"Heat: Building object pointer array\n");
	int x=0;
	int y=0;
	int z=0;

	struct dim_heat *dim=&(thermal->dim);
	struct vec v;

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				v.z=dim->z[z];
				v.x=dim->x[x];
				v.y=dim->y[y];

				thermal->obj[z][x][y]=ray_obj_search_xyz(sim,dev,&v);
				//printf("%Le %Le %Le %d\n",dim->z[z],dim->x[x],dim->y[y],li->obj[z][x][y]);
				//if (li->obj[z][x][y]!=0)
				//{
				//	printf("%s\n",li->obj[z][x][y]->name);
				//}
				//getchar();

			}
		}
	}


}

void heat_build_materials_arrays(struct simulation *sim,struct heat *thermal, struct device *dev)
{
	int x=0;
	int y=0;
	int z=0;
	//struct object *obj;
	struct dim_heat *dim=&(thermal->dim);

	heat_build_obj_pointer_array(sim,thermal, dev);

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				//obj=thermal->obj[z][x][y];//ray_obj_search_xyz(sim,dev,&v);
				thermal->kl[z][x][y]=thermal->thermal_kl;

			}

		}
	}



}
