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
	struct json_obj *json_spectra;
	struct json_obj *json_light;
	char temp_str[100];

	struct dim_light *dim=&(li->dim);

	li->disable_transfer_to_electrical_mesh=FALSE;

	json_light=json_obj_find(json_config, "light");

	if (json_light==NULL)
	{
		ewe(sim,"Object light not found\n");
	}
	json_get_string(sim, json_light, li->suns_spectrum_file,"sun");
	
	json_get_int(sim, json_light, &li->align_mesh,"alignmesh");

	json_get_int(sim, json_light, &dim->ylen,"meshpoints");

	json_get_string(sim, json_light, temp_str,"light_illuminate_from");

	if (strcmp(temp_str,"bottom")==0)
	{
		li->flip_field=TRUE;
	}else
	{
		li->flip_field=FALSE;
	}

	json_get_english(sim,json_light, &(li->light_wavelength_auto_mesh),"light_wavelength_auto_mesh");

	json_get_int(sim, json_light, &dim->llen,"lpoints");

	json_get_long_double(sim, json_light, &li->lstart,"lstart");
	json_get_long_double(sim, json_light, &li->lstop,"lstop");

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

	json_spectra=json_obj_find(json_light, "light_source_obj_y0");

	if (json_spectra==NULL)
	{
		ewe(sim,"Object light_source_obj_y0 not found\n");
	}

	light_src_load(sim,&(li->light_src_y0), json_spectra);


	json_spectra=json_obj_find(json_light, "light_source_obj_y1");

	if (json_spectra==NULL)
	{
		ewe(sim,"Object light_source_obj_y1 not found\n");
	}

	light_src_load(sim,&(li->light_src_y1), json_spectra);

}

void light_load_config(struct simulation *sim,struct light *li, struct device *dev)
{
	char path_temp[PATH_MAX];
	struct dimensions *dim=&(dev->ns.dim);
	li->epi=&(dev->my_epitaxy);
	li->dim.xlen=dim->xlen;
	li->dim.zlen=dim->zlen;
	li->force_update=FALSE;

	join_path(2,path_temp,get_output_path(dev),"optical_output");
	remove_dir(sim,path_temp);

	light_load_config_file(sim,li,&(dev->config.obj));
	light_load_materials(sim,li,dev);
	light_malloc(sim,li);
	light_build_mesh(sim,li,dim);
	light_build_materials_arrays(sim,li,dev);
}
