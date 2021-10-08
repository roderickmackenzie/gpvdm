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

/** @file epitaxy.c
	@brief Load the epitaxy structure.
*/

#include <string.h>
#include "epitaxy.h"
#include "inp.h"
#include "util.h"
#include "gpvdm_const.h"
#include <cal_path.h>
#include <shape.h>
#include <contacts.h>
#include <component.h>
#include <component_fun.h>
#include <log.h>

int epitaxy_load(struct simulation *sim,struct epitaxy *in, struct json_obj *obj_epi)
{
	int i;
	//char temp[100];
	//char interface_file[20];
	char shape_file[20];

	long double y_pos=0.0;
	struct shape *s;

	in->device_start=-1.0;
	in->device_stop=-1.0;
	char layer_id[20];
	//char *test;
	struct json_obj *obj_layer;
	struct json_obj *obj_pl;
	struct json_obj *obj_interface;
	//struct json_obj *obj_shape;

	json_get_int(sim, obj_epi, &(in->layers),"layers");

	if (in->layers>20)
	{
		ewe(sim,"Too many material layers\n");
	}

	if (in->layers<0)
	{
		ewe(sim,"Epitaxy struct not initialized\n");
	}


	for (i=0;i<in->layers;i++)
	{
		in->layer[i].layer_number=i;
		s=&(in->layer[i].s);
		shape_init(sim,s);

		sprintf(layer_id,"layer%d",i);
		obj_layer=json_obj_find(obj_epi, layer_id);

		if (obj_layer==NULL)
		{
			ewe(sim,"Object %s not found\n",layer_id);
		}

		json_get_english(sim,obj_layer, &(in->layer[i].layer_type),"layer_type");

		if (shape_load_from_json(sim,s, obj_layer ,0.0)==FALSE)
		{
			printf_log(sim,"Warning shape %s disabled\n", shape_file);
		}
		
		in->layer[i].s.epi_index=i;

		obj_pl=json_obj_find(obj_layer, "shape_pl");
		epitaxy_load_pl_file(sim,&(in->layer[i]),obj_pl);

		in->layer[i].width=fabs(s->dy);
		s->y0=y_pos;

		//interface file

		//json_dump_obj(obj_layer);
		obj_interface=json_obj_find(obj_layer, "layer_interface");
		epitaxy_load_interface_file(sim,&(in->layer[i]),obj_interface);

		json_get_english(sim,obj_layer, &(in->layer[i].solve_optical_problem),"solve_optical_problem");

		json_get_english(sim,obj_layer, &(in->layer[i].solve_thermal_problem),"solve_thermal_problem");

		//json_get_long_double(sim,obj_layer, &(in->layer[i].Gnp),"Gnp");
		//char temp[20];
		//char full_path[PATH_MAX];
		if (in->layer[i].layer_type==LAYER_ACTIVE)
		{
			if (in->device_start==-1.0)
			{
				in->device_start=y_pos;
			}

			in->layer[i].electrical_layer=TRUE;
		}else
		{
			in->layer[i].electrical_layer=FALSE;
		}

		in->layer[i].y_start=y_pos;
		s->y0=in->layer[i].y_start;
		//printf("%Le %Le %Le\n",in->layer[i].y_start,in->layer[i].width,s->dy);
		y_pos+=in->layer[i].width;
		in->layer[i].y_stop=y_pos;
	}
	//getchar();

	in->device_stop=epitaxy_get_device_stop(in);


	epitaxy_shapes_load(sim,in,obj_epi);


	return in->layers;

}

