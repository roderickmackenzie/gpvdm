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
#include "epitaxy.h"
#include "lang.h"
#include "log.h"
#include <cal_path.h>
#include <light_fun.h>
#include <json.h>
#include <device_fun.h>

static int unused __attribute__((unused));

/** @file light_config.c
	@brief Loads the light configuration files.
*/



void light_load_config_file(struct simulation *sim,struct light *li, struct json_obj *json_config)
{
	struct json_obj *json_light;

	struct dimensions *dim=&(li->dim);

	li->disable_transfer_to_electrical_mesh=FALSE;

	json_light=json_obj_find(json_config, "light");

	if (json_light==NULL)
	{
		ewe(sim,"Object light not found in light_config.c\n");
	}
	json_get_string(sim, json_light, li->suns_spectrum_file,"sun");

	json_get_int(sim, json_light, &li->align_mesh,"alignmesh");

	json_get_int(sim, json_light, &dim->ylen,"meshpoints");

	json_get_english(sim,json_light, &(li->light_wavelength_auto_mesh),"light_wavelength_auto_mesh");

	json_get_long_double(sim, json_light, &li->electron_eff,"electron_eff");
	li->electron_eff=fabs(li->electron_eff);

	json_get_long_double(sim, json_light, &li->hole_eff,"hole_eff");
	li->hole_eff=fabs(li->hole_eff);

	json_get_long_double(sim, json_light, &li->Psun,"Psun");
	//printf("%Le\n",li->Psun);
	//getchar();
	li->Psun=fabs(li->Psun);

	json_get_string(sim, json_light, li->mode,"light_model");

	json_get_long_double(sim, json_light, &(li->Dphotoneff),"Dphotoneff");
	li->Dphotoneff=fabs(li->Dphotoneff);

	json_get_long_double(sim, json_light, &(li->ND),"NDfilter");

	json_get_string(sim, json_light, li->light_file_generation,"light_file_generation");

	json_get_long_double(sim, json_light, &(li->light_file_generation_shift),"light_file_generation_shift");

	json_get_string(sim, json_light, li->light_profile,"light_profile");

	json_get_int(sim, json_light, &li->dump_verbosity,"dump_verbosity");
	//json_dump_obj(obj);
	//getchar();

	//printf("yes'%d'\n",li->dump_verbosity);
	//getchar();

}

void light_load_light_sources(struct simulation *sim,struct light *li, struct json_obj *json_config)
{
	int segs;
	int i;
	char temp[200];
	int y0_found=FALSE;
	int y1_found=FALSE;

	struct json_obj *json_light_sources;
	struct json_obj *json_seg;
	struct json_obj *json_lights;

	char illuminate_from[200];

	json_light_sources=json_obj_find(json_config, "light_sources");

	if (json_light_sources==NULL)
	{
		ewe(sim,"Object json_light_sources not found\n");
	}

	json_lights=json_obj_find(json_light_sources, "lights");

	if (json_lights==NULL)
	{
		ewe(sim,"Object json_lights not found\n");
	}

	json_get_int(sim, json_lights, &segs,"segments");

	for (i=0;i<segs;i++)
	{
		sprintf(temp,"segment%d",i);
		json_seg=json_obj_find(json_lights, temp);
		json_get_string(sim, json_seg, illuminate_from,"light_illuminate_from");

		li->light_src_y0.use_flat_sepctrum=li->use_flat_sepctrum;

		li->light_src_y1.use_flat_sepctrum=li->use_flat_sepctrum;

		if (strcmp(illuminate_from,"y0")==0)
		{
			if (y0_found==FALSE)
			{
				y0_found=TRUE;
				light_src_load(sim,&(li->light_src_y0), json_seg);
			}
		}

		if (strcmp(illuminate_from,"y1")==0)
		{
			if (y1_found==FALSE)
			{
				y1_found=TRUE;
				light_src_load(sim,&(li->light_src_y1), json_seg);
			}
		}
	}

	if (y0_found==TRUE)
	{
		if (y1_found==TRUE)
		{
			ewe(sim,"If you want to illuminate from the top and bottom at the same time drop me a mail..\n");
		}
	}
}

void light_load_config(struct simulation *sim,struct light *li, struct device *dev)
{
	char path_temp[PATH_MAX];
	struct dimensions *dim=&(dev->ns.dim);
	li->epi=&(dev->my_epitaxy);
	li->dim.xlen=dim->xlen;
	li->dim.zlen=dim->zlen;
	li->lstart=dev->lights.lstart;
	li->lstop=dev->lights.lstop;
	li->dim.llen=dev->lights.llen;
	li->dim.dx=dev->xlen;
	li->dim.dz=dev->zlen;

	li->force_update=FALSE;

	join_path(2,path_temp,get_output_path(dev),"optical_output");
	remove_dir(sim,path_temp);


	light_load_config_file(sim,li,&(dev->config.obj));



	light_load_light_sources(sim,li, &(dev->config.obj));

	light_load_materials(sim,li,dev);
	light_malloc(sim,li);
	light_build_mesh(sim,li,dim);
	light_build_materials_arrays(sim,li,dev);

}
