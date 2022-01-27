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
#include <heat_fun.h>
#include <mesh.h>
#include <device_fun.h>

static int unused __attribute__((unused));

/** @file config.c
	@brief Loads the heat configuration files.
*/



void heat_load_config_file(struct simulation *sim,struct heat *thermal, struct json_obj *json_config)
{
	struct json_obj *json_thermal;
	struct json_obj *json_thermal_boundary;

	json_thermal=json_obj_find(json_config, "thermal");

	json_get_english(sim,json_thermal, &(thermal->newton_enable_external_thermal),"thermal");
	json_get_english(sim,json_thermal, &(thermal->thermal_couple_to_electrical_solver),"thermal_couple_to_electrical_solver");

	json_get_english(sim,json_thermal, &(thermal->thermal_model_type),"thermal_model_type");

	json_get_english(sim,json_thermal, &(thermal->thermal_l),"thermal_l");

	json_get_english(sim,json_thermal, &(thermal->thermal_e),"thermal_e");

	json_get_english(sim,json_thermal, &(thermal->thermal_h),"thermal_h");

	json_get_int(sim,json_thermal, &(thermal->thermal_max_ittr),"thermal_max_ittr");
	json_get_long_double(sim,json_thermal, &(thermal->min_error),"thermal_min_error");

	json_get_int(sim,json_thermal, &(thermal->nofluxl),"nofluxl");

	json_get_english(sim,json_thermal, &(thermal->joule_heating),"joule_heating");
	json_get_english(sim,json_thermal, &(thermal->parasitic_heating),"parasitic_heating");

	json_get_english(sim,json_thermal, &(thermal->recombination_heating),"recombination_heating");

	json_get_english(sim,json_thermal, &(thermal->optical_heating),"optical_heating");

	json_get_int(sim,json_thermal, &(thermal->dump_verbosity),"dump_verbosity");
	json_get_english(sim,json_thermal, &(thermal->solver_verbosity),"solver_verbosity");

	json_thermal_boundary=json_obj_find(json_config, "thermal_boundary");

	json_get_english(sim,json_thermal_boundary, &(thermal->Ty0_boundry),"Ty0_boundry");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->Ty0),"Ty0");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->heatsink_y0),"heatsink_y0");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->heatsink_length_y0),"heatsink_length_y0");

	json_get_english(sim,json_thermal_boundary, &(thermal->Ty1_boundry),"Ty1_boundry");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->Ty1),"Ty1");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->heatsink_y1),"heatsink_y1");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->heatsink_length_y1),"heatsink_length_y1");

	json_get_english(sim,json_thermal_boundary, &(thermal->Tx0_boundry),"Tx0_boundry");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->Tx0),"Tx0");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->heatsink_x0),"heatsink_x0");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->heatsink_length_x0),"heatsink_length_x0");

	json_get_english(sim,json_thermal_boundary, &(thermal->Tx1_boundry),"Tx1_boundry");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->Tx1),"Tx1");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->heatsink_x1),"heatsink_x1");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->heatsink_length_x1),"heatsink_length_x1");

	json_get_english(sim,json_thermal_boundary, &(thermal->Tz0_boundry),"Tz0_boundry");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->Tz0),"Tz0");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->heatsink_z0),"heatsink_z0");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->heatsink_length_z0),"heatsink_length_z0");

	json_get_english(sim,json_thermal_boundary, &(thermal->Tz1_boundry),"Tz1_boundry");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->Tz1),"Tz1");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->heatsink_z1),"heatsink_z1");
	json_get_long_double(sim,json_thermal_boundary, &(thermal->heatsink_length_z1),"heatsink_length_z1");


}

void heat_load_config(struct simulation *sim,struct heat *thermal, struct device *dev)
{
	//struct dimensions *dim_dev=&(dev->ns.dim);
	struct dimensions *dim=&(thermal->dim);
	struct epitaxy* epi=&(dev->my_epitaxy);

	heat_load_config_file(sim,thermal,&(dev->config.obj));

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
			mesh_gen_simple(sim, &(thermal->mesh_data.meshdata_y),stop-start,50);
			//mesh_cpy(sim,&(thermal->mesh_data.meshdata_y),&(dev->mesh_data.meshdata_y));
			thermal->mesh_data.meshdata_y.start=start;
		}

		mesh_to_dim(sim,dim, &(thermal->mesh_data.meshdata_z),'z');
		mesh_to_dim(sim,dim, &(thermal->mesh_data.meshdata_x),'x');
		mesh_to_dim(sim,dim, &(thermal->mesh_data.meshdata_y),'y');

		heat_malloc(sim,thermal);
		heat_build_materials_arrays(sim,thermal,dev);
		heat_setup_dump_dir(sim,get_output_path(dev),thermal);
		heat_set_initial_distribution(thermal);
	}else
	{
		printf_log(sim,"Isothermal heat model\n");
	}
}
