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

/** @file light_utils.c
	@brief Miscellaneous functions for the light model.
*/

#include "util.h"
#include "gpvdm_const.h"
#include "light.h"
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
#include <light_fun.h>
#include <cal_path.h>
#include <dump.h>

static int unused __attribute__((unused));


void light_dump_stats(struct simulation *sim, char *path, struct light *li)
{
struct dimensions *dim=&li->dim;

int x;
int y;
int z;
int l;
long double tot=0.0;
int layer;
struct object *obj;
struct shape *s;
struct dat_file buf;
struct epitaxy *epi=li->epi;
long double layers[100];
char line[1000];

	for (l=0;l<epi->layers;l++)
	{
		layers[l]=0.0;
	}

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				obj=li->obj[z][x][y];//ray_obj_search_xyz(sim,dev,&v);
				s=obj->s;
				layer=s->epi_index;
				//printf("%s\n",s->name);
				//printf("%d\n",layer);
				if (layer!=-1)
				{
					layers[layer]+=(li->Gn[z][x][y]+li->Gp[z][x][y])*dim->dx*dim->dz*dim->dy/2.0;
				}
			}
		}
	}

	tot=0.0;

	for (l=0;l<epi->layers;l++)
	{
		tot+=layers[l];
	}

	for (l=0;l<epi->layers;l++)
	{
		layers[l]/=tot;
	}


	dat_file_init(&buf);

	buffer_malloc(&buf);

	buffer_add_string(&buf,"{\n");
	sprintf(line,"\t\"layers\" : %d,\n",epi->layers);
	buffer_add_string(&buf,line);

	for (l=0;l<epi->layers;l++)
	{
		sprintf(line,"\t\"layer%d\": {\n",l);
		buffer_add_string(&buf,line);

		sprintf(line,"\t\t\"light_frac_photon_generation\": %Le\n",layers[l]);
		buffer_add_string(&buf,line);

		buffer_add_string(&buf,"\t\t}");
		if (l<epi->layers-1)
		{
			buffer_add_string(&buf,",");
		}
		buffer_add_string(&buf,"\n");
	}

	buffer_add_string(&buf,"}\n");

	buffer_dump_path(sim,path,"light_stats.json",&buf);
	buffer_free(&buf);
}


