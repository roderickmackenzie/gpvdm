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


void light_dump_stats(struct simulation *sim,struct light *li)
{
struct dim_light *dim=&li->dim;

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


	buffer_init(&buf);

	buffer_malloc(&buf);

	for (l=0;l<epi->layers;l++)
	{
		sprintf(line,"#light_frac_photon_generation%d\n",l);
		buffer_add_string(&buf,line);

		sprintf(line,"%Le\n",layers[l]);
		buffer_add_string(&buf,line);

	}

	buffer_add_string(&buf,"#end\n");

	buffer_dump_path(sim,get_output_path(sim),"light_stats.dat",&buf);
	buffer_free(&buf);
}


