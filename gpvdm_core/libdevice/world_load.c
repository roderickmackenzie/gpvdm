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

	json_world_config=json_obj_find(json_world, "config");

	if (json_world_config==NULL)
	{
		ewe(sim,"world.config not found\n");
	}

	json_get_long_double(sim, json_world_config, &(w->x0),"world_margin_x0");
	json_get_long_double(sim, json_world_config, &(w->x1),"world_margin_x1");

	json_get_long_double(sim, json_world_config, &(w->y0),"world_margin_y0");
	json_get_long_double(sim, json_world_config, &(w->y1),"world_margin_y1");

	json_get_long_double(sim, json_world_config, &(w->z0),"world_margin_z0");
	json_get_long_double(sim, json_world_config, &(w->z1),"world_margin_z1");

	w->obj=malloc(sizeof(struct object)*1000);

	return w->items;
}


