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
#include <heat_fun.h>
#include <mesh.h>

static int unused __attribute__((unused));

/** @file config.c
	@brief Loads the heat configuration files.
*/



void heat_load_config_file(struct simulation *sim,struct heat *thermal)
{
	char temp[100];

	struct inp_file inp;

	inp_init(sim,&inp);
	inp_load_from_path(sim,&inp,get_input_path(sim),"thermal.inp");
	inp_check(sim,&inp,1.1);

	inp_search_string(sim,&inp,temp,"#thermal");
	thermal->newton_enable_external_thermal=english_to_bin(sim,temp);

	inp_search_string(sim,&inp,temp,"#thermal_model_type");
	thermal->thermal_model_type=english_to_bin(sim,temp);

	inp_search_string(sim,&inp,temp,"#thermal_l");
	thermal->thermal_l=english_to_bin(sim,temp);

	inp_search_string(sim,&inp,temp,"#thermal_e");
	thermal->thermal_e=english_to_bin(sim,temp);

	inp_search_string(sim,&inp,temp,"#thermal_h");
	thermal->thermal_h=english_to_bin(sim,temp);

	inp_search_int(sim,&inp,&(thermal->thermal_max_ittr),"#thermal_max_ittr");

	inp_search_gdouble(sim,&inp,&(thermal->thermal_kl),"#thermal_kl");
	inp_search_gdouble(sim,&inp,&(thermal->thermal_tau_e),"#thermal_tau_e");
	inp_search_gdouble(sim,&inp,&(thermal->thermal_tau_h),"#thermal_tau_h");

	inp_search_int(sim,&inp,&(thermal->nofluxl),"#nofluxl");

	inp_search_string(sim,&inp,temp,"#joule_heating");
	thermal->joule_heating=english_to_bin(sim,temp);
	
	inp_search_string(sim,&inp,temp,"#recombination_heating");
	thermal->recombination_heating=english_to_bin(sim,temp);

	inp_search_string(sim,&inp,temp,"#optical_heating");
	thermal->optical_heating=english_to_bin(sim,temp);

	inp_free(sim,&inp);

	inp_load_from_path(sim,&inp,get_input_path(sim),"thermal_boundry.inp");
	inp_check(sim,&inp,1.0);

	inp_search_string(sim,&inp,temp,"#Ty0_boundry");
	thermal->Ty0_boundry=english_to_bin(sim,temp);
	inp_search_gdouble(sim,&inp,&(thermal->Ty0),"#Ty0");
	inp_search_gdouble(sim,&inp,&(thermal->heatsink_y0),"#heatsink_y0");
	inp_search_gdouble(sim,&inp,&(thermal->heatsink_length_y0),"#heatsink_length_y0");

	inp_search_string(sim,&inp,temp,"#Ty1_boundry");
	thermal->Ty1_boundry=english_to_bin(sim,temp);
	inp_search_gdouble(sim,&inp,&(thermal->Ty1),"#Ty1");
	inp_search_gdouble(sim,&inp,&(thermal->heatsink_y1),"#heatsink_y1");
	inp_search_gdouble(sim,&inp,&(thermal->heatsink_length_y1),"#heatsink_length_y1");

	inp_search_string(sim,&inp,temp,"#Tx0_boundry");
	thermal->Tx0_boundry=english_to_bin(sim,temp);
	inp_search_gdouble(sim,&inp,&(thermal->Tx0),"#Tx0");
	inp_search_gdouble(sim,&inp,&(thermal->heatsink_x0),"#heatsink_x0");
	inp_search_gdouble(sim,&inp,&(thermal->heatsink_length_x0),"#heatsink_length_x0");

	inp_search_string(sim,&inp,temp,"#Tx1_boundry");
	thermal->Tx1_boundry=english_to_bin(sim,temp);
	inp_search_gdouble(sim,&inp,&(thermal->Tx1),"#Tx1");
	inp_search_gdouble(sim,&inp,&(thermal->heatsink_x1),"#heatsink_x1");
	inp_search_gdouble(sim,&inp,&(thermal->heatsink_length_x1),"#heatsink_length_x1");

	inp_search_string(sim,&inp,temp,"#Tz0_boundry");
	thermal->Tx0_boundry=english_to_bin(sim,temp);
	inp_search_gdouble(sim,&inp,&(thermal->Tz0),"#Tz0");
	inp_search_gdouble(sim,&inp,&(thermal->heatsink_z0),"#heatsink_z0");
	inp_search_gdouble(sim,&inp,&(thermal->heatsink_length_z0),"#heatsink_length_z0");

	inp_search_string(sim,&inp,temp,"#Tz1_boundry");
	thermal->Tz1_boundry=english_to_bin(sim,temp);
	inp_search_gdouble(sim,&inp,&(thermal->Tz1),"#Tz1");
	inp_search_gdouble(sim,&inp,&(thermal->heatsink_z1),"#heatsink_z1");
	inp_search_gdouble(sim,&inp,&(thermal->heatsink_length_z1),"#heatsink_length_z1");

	inp_free(sim,&inp);

}

void heat_load_config(struct simulation *sim,struct heat *thermal, struct device *dev)
{
	//struct dimensions *dim_dev=&(dev->ns.dim);
	struct dim_heat *dim=&(thermal->dim);
	struct epitaxy* epi=&(dev->my_epitaxy);
	heat_load_config_file(sim,thermal);

	if (thermal->newton_enable_external_thermal==TRUE)
	{
		mesh_cpy(sim,&(thermal->mesh_data.meshdata_z),&(dev->mesh_data.meshdata_z));
		mesh_cpy(sim,&(thermal->mesh_data.meshdata_x),&(dev->mesh_data.meshdata_x));

		if (thermal->thermal_model_type==THERMAL_HYDRODYNAMIC)
		{
			mesh_cpy(sim,&(thermal->mesh_data.meshdata_y),&(dev->mesh_data.meshdata_y));
			thermal->mesh_data.meshdata_y.start=dev->my_epitaxy.device_start;
		}else
		{
			//We are assuming the lattice heat model covers only the active layer - for now
			long double start=epitaxy_get_heat_problem_start(epi);
			long double stop=epitaxy_get_heat_problem_stop(epi);
			mesh_gen_simple(sim, &(thermal->mesh_data.meshdata_y),stop-start,40);
			//mesh_cpy(sim,&(thermal->mesh_data.meshdata_y),&(dev->mesh_data.meshdata_y));
			thermal->mesh_data.meshdata_y.start=start;
		}

		mesh_to_dim_heat(sim,dim, &(thermal->mesh_data.meshdata_z),'z');
		mesh_to_dim_heat(sim,dim, &(thermal->mesh_data.meshdata_x),'x');
		mesh_to_dim_heat(sim,dim, &(thermal->mesh_data.meshdata_y),'y');

		heat_malloc(sim,thermal);
		heat_build_materials_arrays(sim,thermal,dev);
		heat_setup_dump_dir(sim,thermal);
		heat_set_initial_distribution(thermal);
	}else
	{
		printf_log(sim,"Isothermal heat model\n");
	}
}
