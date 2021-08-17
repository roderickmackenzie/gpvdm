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

/** @file shape.c
	@brief Load the shape files.
*/

#include <epitaxy.h>
#include <sim_struct.h>
#include <shape.h>
#include <string.h>
#include <inp.h>
#include <util.h>
#include <cal_path.h>
#include <i.h>
#include <triangles.h>
#include <triangle.h>


void epitaxy_shapes_load(struct simulation *sim,struct epitaxy *in, struct json_obj *obj_epi)
{
	int l=0;
	int ns;
	//int len=0;
	struct shape *s;
	//long double y_pos=0.0;
	int shape_max=0;
	int nshape=0;

	char layer_id[100];
	char shape_id[100];
	struct json_obj *obj_layer;
	struct json_obj *obj_shape;

	for (l=0;l<in->layers;l++)
	{
		sprintf(layer_id,"layer%d",l);
		obj_layer=json_obj_find(obj_epi, layer_id);

		if (obj_layer==NULL)
		{
			ewe(sim,"Object %s not found\n",layer_id);
		}


		json_get_int(sim,obj_layer, &(shape_max),"layer_shapes");
		nshape=0;
		for (ns=0;ns<shape_max;ns++)
		{
			//if (shape_load_file(sim,in,&(in->layer[l].shapes[in->layer[l].nshape]),build,0.0)==TRUE)
			sprintf(shape_id,"shape%d",ns);
			obj_shape=json_obj_find(obj_layer, shape_id);

			if (obj_shape==NULL)
			{
				ewe(sim,"Object %s not found\n",shape_id);
			}

			if (shape_load_from_json(sim,&(in->layer[l].shapes[ns]), obj_shape ,0.0)==TRUE)
			{
				s=&(in->layer[l].shapes[ns]);
				s->epi_index=l;
				s->y0=in->layer[l].y_start+s->y0;		//Starting from zero


				if (s!=NULL)
				{
					s->epi_index=l;
				}
				nshape++;
			}

		}
		in->layer[l].nshape=nshape;

	}

}


