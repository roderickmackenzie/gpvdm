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
	char temp[100];
	struct inp_file inp;

	inp_init(sim,&inp);
	inp_load_from_path(sim,&inp,get_input_path(sim),"math.inp");
	inp_check(sim,&inp,1.50);
	inp_search_int(sim,&inp,&(in->max_electrical_itt0),"#maxelectricalitt_first");
	inp_search_gdouble(sim,&inp,&(in->electrical_clamp0),"#electricalclamp_first");
	inp_search_gdouble(sim,&inp,&(in->electrical_error0),"#math_electrical_error_first");

	inp_search_string(sim,&inp,temp,"#math_enable_pos_solver");
	in->math_enable_pos_solver=english_to_bin(sim,temp);

	inp_search_int(sim,&inp,&(in->max_electrical_itt),"#maxelectricalitt");
	inp_search_gdouble(sim,&inp,&(in->electrical_clamp),"#electricalclamp");
	inp_search_gdouble(sim,&inp,&(in->posclamp),"#posclamp");
	inp_search_gdouble(sim,&inp,&(in->min_cur_error),"#electricalerror");
	//inp_search_int(sim,&inp,&(in->newton_clever_exit),"#newton_clever_exit");

	inp_search_string(sim,&inp,temp,"#newton_clever_exit");
	in->newton_clever_exit=english_to_bin(sim,temp);

	inp_search_int(sim,&inp,&(in->newton_min_itt),"#newton_min_itt");
	inp_search_int(sim,&inp,&(in->remesh),"#remesh");
	inp_search_int(sim,&inp,&(in->newmeshsize),"#newmeshsize");
	inp_search_int(sim,&inp,&(in->pos_max_ittr),"#pos_max_ittr");
	inp_search_int(sim,&inp,&(in->config_kl_in_newton),"#kl_in_newton");
	inp_search_string(sim,&inp,in->solver_name,"#solver_name");
	inp_search_string(sim,&inp,in->complex_solver_name,"#complex_solver_name");
	inp_search_string(sim,&inp,in->newton_name,"#newton_name");

	if (strcmp(in->newton_name,"newton_simple")==0)
	{
		in->circuit_simulation=TRUE;
	}
	inp_search_gdouble(sim,&inp,&(sim->T0),"#math_t0");
	inp_search_gdouble(sim,&inp,&(sim->D0),"#math_d0");
	inp_search_gdouble(sim,&inp,&(sim->n0),"#math_n0");

	inp_search_string(sim,&inp,temp,"#math_dynamic_mesh");
	in->dynamic_mesh=english_to_bin(sim,temp);

	inp_free(sim,&inp);

}

void load_sim_file(struct simulation *sim,struct device *in)
{
	if (get_dump_status(sim,dump_info_text)==TRUE)
	{
		printf_log(sim,"%s\n",_("load: sim.inp"));
	}

	int i;
	char temp[100];

	struct inp_file inp;
	inp_init(sim,&inp);
	inp_load_from_path(sim,&inp,get_input_path(sim),"sim.inp");
	inp_check(sim,&inp,1.2);

	inp_search_string(sim,&inp,in->simmode,"#simmode");

	inp_search_int(sim,&inp,&(in->stoppoint),"#stoppoint");


	inp_free(sim,&inp);

}

void load_config(struct simulation *sim,struct device *in)
{
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


inp_init(sim,&inp);
inp_load_from_path(sim,&inp,get_input_path(sim),"device.inp");
inp_check(sim,&inp,1.24);

in->area=in->xlen*in->zlen;


inp_search_gdouble(sim,&inp,&(in->Rshort),"#Rshort");
in->Rshort=fabs(in->Rshort);

//inp_search_int(sim,&inp,&(in->interfaceleft),"#interfaceleft");
//inp_search_int(sim,&inp,&(in->interfaceright),"#interfaceright");

inp_free(sim,&inp);


inp_init(sim,&inp);
inp_load_from_path(sim,&inp,get_input_path(sim),"parasitic.inp");
inp_check(sim,&inp,1.1);

inp_search_gdouble(sim,&inp,&(in->other_layers),"#otherlayers");
in->other_layers=fabs(in->other_layers);
hard_limit(sim,"#other_layers",&(in->other_layers));

inp_search_gdouble(sim,&inp,&(in->Rshunt),"#Rshunt");
in->Rshunt=fabs(in->Rshunt)/in->area;

inp_search_gdouble(sim,&inp,&(in->Rcontact),"#Rcontact");
in->Rcontact=fabs(in->Rcontact);

inp_search_gdouble(sim,&inp,&(in->test_param),"#test_param");

inp_free(sim,&inp);



}
