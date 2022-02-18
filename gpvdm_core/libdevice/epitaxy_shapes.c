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

	//epitaxy_debug(sim,in);
}

void epitaxy_debug(struct simulation *sim,struct epitaxy *in)
{
	int l;
	struct shape *s;
	for (l=0;l<in->layers;l++)
	{
		s=&(in->layer[l].s);
		printf("%d %s\n",in->layer[l].nshape,s->name);
	}

	getchar();
}
