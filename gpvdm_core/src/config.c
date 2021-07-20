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

/** @file config.c
@brief load the config files.
*/



#include <stdio.h>
#include <string.h>
#include "sim.h"
#include "dump.h"
#include "config.h"
#include "inp.h"
#include "util.h"
#include "hard_limit.h"
#include "epitaxy.h"
#include "mesh.h"
#include <cal_path.h>
#include <log.h>
#include <lang.h>

static int unused __attribute__((unused));

void device_load_math_config(struct simulation *sim,struct device *in)
{
	int dump;
	struct json_obj *obj;
	struct json_obj *electrical_solver;
	struct json_obj *poisson;

	obj=json_obj_find(&(in->config.obj), "math");
	if (obj==NULL)
	{
		ewe(sim,"Object light not found\n");
	}

	json_get_int(sim, obj, &in->max_electrical_itt0,"maxelectricalitt_first");

	json_get_long_double(sim, obj, &in->electrical_clamp0,"electricalclamp_first");
	json_get_long_double(sim, obj, &in->electrical_error0,"math_electrical_error_first");

	json_get_int(sim, obj, &in->max_electrical_itt,"maxelectricalitt");
	json_get_long_double(sim, obj, &in->electrical_clamp,"electricalclamp");


	json_get_long_double(sim, obj, &in->min_cur_error,"electricalerror");

	json_get_english(sim,obj, &(in->newton_clever_exit),"newton_clever_exit");

	json_get_int(sim, obj, &in->newton_min_itt,"newton_min_itt");
	json_get_int(sim, obj, &in->remesh,"remesh");
	json_get_int(sim, obj, &in->newmeshsize,"newmeshsize");
	json_get_int(sim, obj, &in->config_kl_in_newton,"kl_in_newton");

	json_get_string(sim, obj, in->newton_name,"newton_name");


	json_get_long_double(sim, obj, &sim->T0,"math_t0");
	json_get_long_double(sim, obj, &sim->D0,"math_d0");
	json_get_long_double(sim, obj, &sim->n0,"math_n0");

	json_get_english(sim, obj, &sim->math_stop_on_convergence_problem,"math_stop_on_convergence_problem");
	json_get_english(sim, obj, &in->solver_verbosity,"solver_verbosity");


	json_get_english(sim,obj, &(in->dynamic_mesh),"math_dynamic_mesh");

	//Poisson solver
	electrical_solver=json_obj_find(&(in->config.obj), "electrical_solver");
	if (obj==NULL)
	{
		ewe(sim,"Object electrical_solver not found\n");
	}

	json_get_string(sim,electrical_solver, in->solver_type,"solver_type");

	if (strcmp(in->solver_type,"circuit")==0)
	{
		in->circuit_simulation=TRUE;
		in->drift_diffision_simulations_enabled=FALSE;
	}

	poisson=json_obj_find(electrical_solver, "poisson");
	if (poisson==NULL)
	{
		ewe(sim,"Object electrical_solver not found\n");
	}



	json_get_english(sim,poisson, &(in->math_enable_pos_solver),"math_enable_pos_solver");
	json_get_english(sim,poisson, &(dump),"dump_print_pos_error");
	set_dump_status(sim,dump_print_pos_error, dump);


}

void load_sim_file(struct simulation *sim,struct device *in)
{
	struct json_obj *obj;
	obj=json_obj_find(&(in->config.obj), "sim");
	if (obj==NULL)
	{
		ewe(sim,"Object sim not found\n");
	}

	json_get_int(sim, obj, &in->stoppoint,"stoppoint");
	json_get_string(sim, obj, in->simmode,"simmode");

}

void load_config(struct simulation *sim,struct device *in)
{
struct json_obj *obj;

if (get_dump_status(sim,dump_info_text)==TRUE)
{
	printf_log(sim,"%s\n",_("load: config files"));
}
int i;
char temp[100];

struct inp_file inp;

in->srh_sim=TRUE;
in->ntrapnewton=TRUE;
in->ptrapnewton=TRUE;

in->ylen=0.0;

in->ylen=epitaxy_get_electrical_length(&(in->my_epitaxy));


mesh_check_y(sim,&(in->mesh_data.meshdata_y),in);


in->area=in->xlen*in->zlen;


obj=json_obj_find(&(in->config.obj), "parasitic");

json_get_long_double(sim,obj, &(in->other_layers),"otherlayers");
in->other_layers=fabs(in->other_layers);
hard_limit_do(sim,"other_layers",&(in->other_layers));

json_get_long_double(sim,obj, &(in->Rshunt),"Rshunt");
in->Rshunt=fabs(in->Rshunt)/in->area;

json_get_long_double(sim,obj, &(in->Rcontact),"Rcontact");
in->Rcontact=fabs(in->Rcontact);

json_get_long_double(sim,obj, &(in->Rshort),"Rshort");
in->Rshort=fabs(in->Rshort);


}
