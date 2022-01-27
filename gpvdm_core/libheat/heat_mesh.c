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

	struct dimensions *dim=&(thermal->dim);
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
	struct object *obj;
	struct dimensions *dim=&(thermal->dim);

	heat_build_obj_pointer_array(sim,thermal, dev);

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{

				obj=thermal->obj[z][x][y];
				thermal->kl[z][x][y]=obj->s->heat.thermal_kl;

			}

		}
	}



}
