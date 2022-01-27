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

/** @file dump_dynamic.c
@brief setup the dynamic dump stuff, this enables things like average charge density to be stored for each simulation stip and then wrtten to disk
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


void dump_dynamic_free(struct simulation *sim,struct device *in,struct dynamic_store *store)
{
if (get_dump_status(sim,dump_dynamic)==TRUE)
{
	struct dimensions *dim=&in->ns.dim;

	//recombination
	inter_free(&(store->R_nfree_to_pfree));
	inter_free(&(store->R_srh_nfree));
	inter_free(&(store->R_srh_pfree));
	inter_free(&(store->R_srh_nfree_to_ptrap));
	inter_free(&(store->R_srh_pfree_to_ntrap));
	inter_free(&(store->T_srh_pfree_to_ptrap));
	inter_free(&(store->T_srh_nfree_to_ntrap));
	inter_free(&(store->G_n));
	inter_free(&(store->G_p));
	inter_free(&(store->R_surface_y0));
	inter_free(&(store->R_surface_y1));

	//charge
	inter_free(&(store->Q_ntrap));
	inter_free(&(store->Q_ptrap));
	inter_free(&(store->Q_nfree));
	inter_free(&(store->Q_pfree));
	inter_free(&(store->Q_nfree_and_ntrap));
	inter_free(&(store->Q_pfree_and_ptrap));

	//inter_free(&(store->charge_change));
	//inter_free(&(store->ntrap_delta_out));
	//inter_free(&(store->ptrap_delta_out));

	//inter_free(&(store->nfree_delta_out));
	//inter_free(&(store->pfree_delta_out));
	//inter_free(&(store->tpc_filledn));
	//inter_free(&(store->tpc_filledp));
	//inter_free(&(store->dynamic_np));
	//inter_free(&(store->dynamic_charge_tot));

	//mobility
	inter_free(&(store->mu_n));
	inter_free(&(store->mu_p));
	inter_free(&(store->mu_n_p_avg));

	//Thermal
	inter_free(&(store->H_joule));
	inter_free(&(store->H_recombination));
	inter_free(&(store->H_parasitic));

	//srh rates
	inter_free(&(store->srh_n_r1));
	inter_free(&(store->srh_n_r2));
	inter_free(&(store->srh_n_r3));
	inter_free(&(store->srh_n_r4));

	inter_free(&(store->srh_p_r1));
	inter_free(&(store->srh_p_r2));
	inter_free(&(store->srh_p_r3));
	inter_free(&(store->srh_p_r4));

	//J
	inter_free(&(store->J_y0_n));
	inter_free(&(store->J_y0_p));
	inter_free(&(store->J_y1_n));
	inter_free(&(store->J_y1_p));

	inter_free(&(store->jout));
	inter_free(&(store->jn_avg));
	inter_free(&(store->jp_avg));
	inter_free(&(store->dynamic_jn));
	inter_free(&(store->dynamic_jp));
	inter_free(&(store->jnout_mid));
	inter_free(&(store->jpout_mid));
	inter_free(&(store->dynamic_jn_drift));
	inter_free(&(store->dynamic_jn_diffusion));
	inter_free(&(store->dynamic_jp_drift));
	inter_free(&(store->dynamic_jp_diffusion));

	inter_free(&(store->iout));

	//pl
	if (in->emission_enabled==TRUE)
	{
		inter_free(&(store->dynamic_pl));
		inter_free(&(store->dynamic_pl_tot));
	}

	//field
	inter_free(&(store->E_field));
	inter_free(&(store->dynamic_Vapplied));
	inter_free(&(store->band_bend));

	//other
	inter_free(&(store->dynamic_qe));
	free_zxy_long_double(dim,&store->band_snapshot);
}
}

