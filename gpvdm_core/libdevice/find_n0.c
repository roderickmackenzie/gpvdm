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

/** @file sim_find_n0.c
@brief calculate 0V in the dark using the newton solver.
*/

#include <stdio.h>
#include <exp.h>
#include "util.h"
#include "sim.h"
#include "dos.h"
#include "dump.h"
#include "log.h"
#include <cal_path.h>
#include <contacts.h>
#include <lang.h>
#include <light_fun.h>
#include <memory.h>

void find_n0(struct simulation *sim,struct device *in)
{
int x;
int y;
int z;
struct newton_state *ns=&in->ns;
long double ***Bfree_bk=NULL;
long double ***Binterface_bk=NULL;
long double ***Bt_interface_bk=NULL;

struct dimensions *dim=&in->ns.dim;
struct epitaxy *epi=&(in->my_epitaxy);
printf_log(sim,"%s\n",_("Finding equilibrium conditions"));
gdouble oldsun=light_get_sun(&(in->mylight));

contacts_force_to_zero(sim,in);



light_set_sun(&(in->mylight),0);

light_solve_and_update(sim,in,&(in->mylight),0.0);

	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		cpy_zxy_long_double(dim, &(Bfree_bk), &(in->B));
		cpy_zxy_long_double(dim, &(Binterface_bk), &(in->interface_B));
		cpy_zxy_long_double(dim, &(Bt_interface_bk), &(in->interface_Bt));

		zxy_set_gdouble(dim, in->B, 0.0);
		zxy_set_gdouble(dim, in->interface_B, 0.0);
		zxy_set_gdouble(dim, in->interface_Bt, 0.0);

		gdouble save_clamp=in->electrical_clamp;
		int save_ittr=in->max_electrical_itt;
		gdouble save_electricalerror=in->min_cur_error;

		in->electrical_clamp=in->electrical_clamp0;
		in->max_electrical_itt=in->max_electrical_itt0;
		in->min_cur_error=in->electrical_error0;

		//int Nion_enabled=dim->Nion_enabled;
		//dim->Nion_enabled=FALSE;
		//solve_all(sim,in);
		//dim->Nion_enabled=Nion_enabled;

		if (ns->Nion_enabled==TRUE)
		{
			in->mobileion.Nion_mul=1.0e25;
			in->mobileion.full_newton=TRUE;
			solve_all(sim,in);
		}
		//exit(0);
		in->max_electrical_itt=save_ittr;
		in->electrical_clamp=save_clamp;
		in->min_cur_error=save_electricalerror;
	}

	solve_all(sim,in);

	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		cpy_zxy_long_double(dim, &(in->B), &(Bfree_bk));
		cpy_zxy_long_double(dim, &(in->interface_B), &(Binterface_bk));
		cpy_zxy_long_double(dim, &(in->interface_Bt), &(Bt_interface_bk));

		free_zxy_gdouble(dim, &(Bfree_bk));
		free_zxy_gdouble(dim, &(Binterface_bk));
		free_zxy_gdouble(dim, &(Bt_interface_bk));

		reset_np_save(in);
		reset_npequlib(in);
	}

in->contact_charge=cal_contact_charge(in);

light_set_sun(&(in->mylight),oldsun);

contacts_update(sim,in);		//This should restore the value of the contacts

printf_log(sim,"%s\n",_("Finished finding equilibrium conditions"));
}
