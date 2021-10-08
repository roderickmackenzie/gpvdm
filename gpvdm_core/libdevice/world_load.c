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

/** @file world_load.c
	@brief Load the world
*/

#include <epitaxy.h>
#include <sim_struct.h>
#include <shape.h>
#include <string.h>
#include <util.h>
#include <cal_path.h>
#include <i.h>
#include <triangles.h>
#include <triangle.h>
#include <world_struct.h>
#include <world.h>
#include <detector.h>

int world_load(struct simulation *sim,struct world *w, struct json_obj *json_world)
{
	int l=0;
	struct shape *s;
	char item_id[100];
	struct json_obj *json_world_data;
	struct json_obj *json_world_config;

	struct json_obj *obj_item;

	json_world_data=json_obj_find(json_world, "world_data");

	if (json_world_data==NULL)
	{
		ewe(sim,"world_data not found\n");
	}

	json_get_int(sim, json_world_data, &(w->items),"segments");

	if (w->items>0)
	{
		w->shapes=(struct shape *)malloc(sizeof(struct shape)*w->items);

		for (l=0;l<w->items;l++)
		{
			sprintf(item_id,"segment%d",l);
			obj_item=json_obj_find(json_world_data, item_id);

			if (obj_item==NULL)
			{
				ewe(sim,"Object %s not found\n",item_id);
			}
			s=&(w->shapes[l]);
			shape_init(sim,s);
			shape_load_from_json(sim,s, obj_item ,0.0);
			s->epi_index=-1;
		}
	}

	json_world_config=json_obj_find(json_world, "config");

	if (json_world_config==NULL)
	{
		ewe(sim,"world.config not found\n");
	}

	json_get_english(sim,json_world_config, &(w->world_automatic_size),"world_automatic_size");

	if (w->world_automatic_size==TRUE)
	{
		json_get_long_double(sim, json_world_config, &(w->x0),"world_margin_x0");
		json_get_long_double(sim, json_world_config, &(w->x1),"world_margin_x1");

		json_get_long_double(sim, json_world_config, &(w->y0),"world_margin_y0");
		json_get_long_double(sim, json_world_config, &(w->y1),"world_margin_y1");

		json_get_long_double(sim, json_world_config, &(w->z0),"world_margin_z0");
		json_get_long_double(sim, json_world_config, &(w->z1),"world_margin_z1");
	}else
	{
		json_get_long_double(sim, json_world_config, &(w->x0),"world_x0");
		json_get_long_double(sim, json_world_config, &(w->x1),"world_x1");

		json_get_long_double(sim, json_world_config, &(w->y0),"world_y0");
		json_get_long_double(sim, json_world_config, &(w->y1),"world_y1");

		json_get_long_double(sim, json_world_config, &(w->z0),"world_z0");
		json_get_long_double(sim, json_world_config, &(w->z1),"world_z1");
	}

	w->obj=malloc(sizeof(struct object)*1000);

	return w->items;
}


