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

	dev->auger_enabled=FALSE;
	dev->ss_srh_enabled=FALSE;
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
			if (s->dosn.auger_enabled==TRUE)
			{
				dev->auger_enabled=TRUE;
			}

			if (s->dosn.ss_srh_enabled==TRUE)
			{
				dev->ss_srh_enabled=TRUE;
			}

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
				if (s->dosn.auger_enabled==TRUE)
				{
					dev->auger_enabled=TRUE;
				}

				if (s->dosn.ss_srh_enabled==TRUE)
				{
					dev->ss_srh_enabled=TRUE;
				}

				pos++;
			}

		}
	}
}

