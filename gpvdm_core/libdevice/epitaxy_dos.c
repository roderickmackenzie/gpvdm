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

/** @file epitaxy_dos.c
	@brief Load the dos files into the epitaxy
*/

#include <string.h>
#include "epitaxy.h"
#include "inp.h"
#include "util.h"
#include "gpvdm_const.h"
#include <cal_path.h>
#include <shape.h>
#include <log.h>
#include <dos.h>

void epitaxy_load_dos_files(struct simulation *sim,struct device *dev, struct epitaxy *epi,struct json_obj *json_epi)
{
	int l;
	int ns;
	int pos;
	int enabled=FALSE;
	char temp[100];
	struct shape *s;
	struct json_obj *json_dos;
	struct json_obj *json_layer;
	struct json_obj *json_shape_dos;
	struct epi_layer *layer;
	struct json_obj *json_sub_shape;

	pos=0;

	if (dev->drift_diffision_simulations_enabled==FALSE)
	{
		return;
	}

	for (l=0;l<epi->layers;l++)
	{
		layer=&(epi->layer[l]);
		s=&(layer->s);
		sprintf(temp,"layer%d",l);
		json_layer=json_obj_find(json_epi, temp);
		json_dos=json_obj_find(json_layer, "shape_dos");
		json_get_english(sim,json_dos, &(enabled),"enabled");
		printf("%s %s %d %d %d\n",temp,s->dos_file,l,epi->layers,enabled);
		if (enabled==TRUE)
		{
			printf("Loading %d\n",pos);
			load_dos(sim,dev,s,json_dos);
			pos++;
		}
		

	}


	for (l=0;l<epi->layers;l++)
	{
		layer=&(epi->layer[l]);
		s=&(layer->s);
		sprintf(temp,"layer%d",l);
		json_layer=json_obj_find(json_epi, temp);

		for (ns=0;ns<layer->nshape;ns++)
		{
			s=&(layer->shapes[ns]);
			sprintf(temp,"shape%d",ns);
			json_sub_shape=json_obj_find(json_layer, temp);
			json_shape_dos=json_obj_find(json_sub_shape, "shape_dos");
			json_get_english(sim, json_shape_dos, &(enabled),"enabled");
			if (enabled==TRUE)
			{
				load_dos(sim,dev,s,json_shape_dos);
				pos++;
			}

		}
	}
}

