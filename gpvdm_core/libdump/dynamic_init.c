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

/** @file dynamic_init.c
@brief init for: setup the dynamic dump stuff, this enables things like average charge density to be stored for each simulation stip and then wrtten to disk
*/

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <i.h>
#include <exp.h>
#include <dos.h>
#include "sim.h"
#include "dump.h"
#include "dat_file.h"
#include "dynamic_store.h"
#include "memory.h"
#include "contacts.h"
#include <lang.h>
#include <cal_path.h>


static int unused __attribute__((unused));

void dump_dynamic_init(struct simulation *sim,struct dynamic_store *store,struct device *in)
{
int x=0;
int y=0;
int z=0;
struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;

if (get_dump_status(sim,dump_dynamic)==TRUE)
{
	//recombination
	inter_init(sim,&(store->R_nfree_to_pfree));
	inter_init(sim,&(store->R_srh_nfree));
	inter_init(sim,&(store->R_srh_pfree));
	inter_init(sim,&(store->R_srh_nfree_to_ptrap));
	inter_init(sim,&(store->R_srh_pfree_to_ntrap));
	inter_init(sim,&(store->T_srh_pfree_to_ptrap));
	inter_init(sim,&(store->T_srh_nfree_to_ntrap));
	inter_init(sim,&(store->G_n));
	inter_init(sim,&(store->G_p));
	inter_init(sim,&(store->R_surface_y0));
	inter_init(sim,&(store->R_surface_y1));

	//charge
	inter_init(sim,&(store->Q_nfree));
	inter_init(sim,&(store->Q_pfree));
	inter_init(sim,&(store->Q_ntrap));
	inter_init(sim,&(store->Q_ptrap));
	inter_init(sim,&(store->Q_nfree_and_ntrap));
	inter_init(sim,&(store->Q_pfree_and_ptrap));

	//inter_init(sim,&(store->ntrap_delta_out));
	//inter_init(sim,&(store->ptrap_delta_out));
	//inter_init(sim,&(store->nfree_delta_out));
	//inter_init(sim,&(store->pfree_delta_out));
	//inter_init(sim,&(store->charge_change));
	//inter_init(sim,&(store->tpc_filledn));
	//inter_init(sim,&(store->tpc_filledp));
	//inter_init(sim,&(store->dynamic_np));

	//inter_init(sim,&(store->dynamic_charge_tot));

	//mobility
	inter_init(sim,&(store->mu_n));
	inter_init(sim,&(store->mu_p));
	inter_init(sim,&(store->mu_n_p_avg));

	//Thermal
	inter_init(sim,&(store->H_joule));
	inter_init(sim,&(store->H_recombination));
	inter_init(sim,&(store->H_parasitic));

	//srh rates

	inter_init(sim,&(store->srh_n_r1));
	inter_init(sim,&(store->srh_n_r2));
	inter_init(sim,&(store->srh_n_r3));
	inter_init(sim,&(store->srh_n_r4));

	inter_init(sim,&(store->srh_p_r1));
	inter_init(sim,&(store->srh_p_r2));
	inter_init(sim,&(store->srh_p_r3));
	inter_init(sim,&(store->srh_p_r4));

	//J
	inter_init(sim,&(store->J_y0_n));
	inter_init(sim,&(store->J_y0_p));
	inter_init(sim,&(store->J_y1_n));
	inter_init(sim,&(store->J_y1_p));

	inter_init(sim,&(store->dynamic_jn_drift));
	inter_init(sim,&(store->dynamic_jn_diffusion));
	inter_init(sim,&(store->dynamic_jp_drift));
	inter_init(sim,&(store->dynamic_jp_diffusion));
	inter_init(sim,&(store->jn_avg));
	inter_init(sim,&(store->jp_avg));
	inter_init(sim,&(store->dynamic_jn));
	inter_init(sim,&(store->dynamic_jp));
	inter_init(sim,&(store->jnout_mid));
	inter_init(sim,&(store->jpout_mid));
	inter_init(sim,&(store->iout));
	inter_init(sim,&(store->jout));


	//pl
	if (in->emission_enabled==TRUE)
	{
		inter_init(sim,&(store->dynamic_pl));
		inter_init(sim,&(store->dynamic_pl_tot));
	}


	//field
	inter_init(sim,&(store->E_field));
	inter_init(sim,&(store->dynamic_Vapplied));
	inter_init(sim,&(store->band_bend));


	//other
	inter_init(sim,&(store->dynamic_qe));
	store->band_snapshot=NULL;

}
}

