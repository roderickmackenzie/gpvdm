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

/** @file shape_load.c
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
#include <log.h>
#include <component_fun.h>
#include <enabled_libs.h>
#include <heat.h>
#include <heat_fun.h>
#include <exciton.h>
#include <exciton_fun.h>
#include <singlet.h>
#include <singlet_fun.h>

int shape_load_from_json(struct simulation *sim,struct shape *s, struct json_obj *obj ,long double y_pos)
{
	int enabled;
	struct json_obj *shape_electrical;
	struct json_obj *json_dos;
	struct json_obj *json_heat;
	struct json_obj *json_exciton;
	struct json_obj *json_singlet;

	char dos_id[100];

	//printf_log(sim,"Loading shape file form json: %s\n",obj->name);

	json_get_english(sim,obj, &(s->enabled),"shape_enabled");

	json_get_long_double(sim,obj, &s->dx,"dx");
	json_get_long_double(sim,obj, &s->dy,"dy");
	json_get_long_double(sim,obj, &s->dz,"dz");

	json_get_long_double(sim,obj, &s->dx_padding,"dx_padding");
	json_get_long_double(sim,obj, &s->dy_padding,"dy_padding");
	json_get_long_double(sim,obj, &s->dz_padding,"dz_padding");

	json_get_int(sim,obj, &(s->nx),"shape_nx");
	json_get_int(sim,obj, &(s->ny),"shape_ny");
	json_get_int(sim,obj, &(s->nz),"shape_nz");

	json_get_string(sim, obj, s->name,"shape_name");
	json_get_string(sim, obj, s->shape_type,"shape_type");

	json_get_string(sim, obj, s->optical_material,"optical_material");
	assert_platform_path(s->optical_material);

	//json_get_string(sim, obj, s->dos_file,"shape_dos");
	//json_get_string(sim, obj, s->pl_file,"shape_pl_file");
	//json_get_string(sim, obj, s->homo_file,"shape_homo_file");
	//json_get_string(sim, obj, s->lumo_file,"shape_lumo_file");
	//json_get_string(sim, obj, s->electrical_file,"shape_electrical");

	#ifdef libcircuit_enabled
		if (s->enabled==TRUE)
		{
			shape_electrical=json_obj_find(obj, "shape_electrical");
			component_load_electrical_file(sim,&(s->com),shape_electrical);
		}
	#endif

	json_get_long_double(sim,obj, &(s->x0),"x0");
	json_get_long_double(sim,obj, &(s->z0),"z0");
	json_get_long_double(sim,obj, &(s->y0),"y0");

	json_get_long_double(sim,obj, &s->rotate_y,"rotate_y");
	json_get_long_double(sim,obj, &s->rotate_x,"rotate_x");

	json_get_long_double(sim,obj, &(s->color_r),"color_r");
	json_get_long_double(sim,obj, &(s->color_g),"color_g");
	json_get_long_double(sim,obj, &(s->color_b),"color_b");

	json_get_long_double(sim,obj, &(s->Gnp),"Gnp");

	json_get_string(sim, obj, s->id,"id");

	//if (s->flip_y==FALSE)
	//{
	//	s->y0=y_pos+s->y0;
	//}else
	//{
	//	s->y0=y_pos;//-s->y0;
	//}

	if (s->enabled==TRUE)
	{
		json_dos=json_obj_find(obj, "shape_dos");
		if (json_dos!=NULL)
		{
			json_get_english(sim,json_dos, &enabled,"enabled");
			json_get_string(sim,json_dos, dos_id, "id");
			if (enabled==TRUE)
			{
				sprintf(s->dos_file,"dos_%s",dos_id);
			}
		}
		shape_load_materials(sim,s);

	}

	json_heat=json_obj_find(obj, "shape_heat");
	if (json_heat!=NULL)
	{
		heat_material_load_from_json(sim,&(s->heat),json_heat );
	}else
	{
		ewe(sim,"shape_heat not found\n");
	}

	json_exciton=json_obj_find(obj, "shape_dos");
	if (json_exciton!=NULL)
	{
		exciton_material_load_from_json(sim,&(s->ex),json_exciton );
	}else
	{
		ewe(sim,"shape_exciton not found\n");
	}

	#ifdef libsinglet_enabled
		json_singlet=json_obj_find(obj, "shape_dos");
		if (json_singlet!=NULL)
		{
			singlet_material_load_from_json(sim,&(s->sing),json_singlet );
		}else
		{
			ewe(sim,"shape_singlet not found\n");
		}
	#endif
return TRUE;
}



