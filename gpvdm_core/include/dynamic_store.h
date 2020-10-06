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

/** @file dynamic_store.h
	@brief Store information as the simulation progresses such as voltage, current or carrier density, these are 1D arrays as a function of time of simulation step.
*/

#ifndef dynamic_store_h
#define dynamic_store_h
#include "i.h"

struct dynamic_store
{
	//recombination
	struct math_xy R_nfree_to_pfree;
	struct math_xy R_srh_nfree;
	struct math_xy R_srh_pfree;
	struct math_xy R_srh_nfree_to_ptrap;
	struct math_xy R_srh_pfree_to_ntrap;
	struct math_xy T_srh_pfree_to_ptrap;
	struct math_xy T_srh_nfree_to_ntrap;
	struct math_xy G_n;
	struct math_xy G_p;
	struct math_xy R_surface_y0;
	struct math_xy R_surface_y1;

	//charge
	struct math_xy Q_nfree;
	struct math_xy Q_pfree;
	struct math_xy Q_ntrap;
	struct math_xy Q_ptrap;
	struct math_xy Q_nfree_and_ntrap;
	struct math_xy Q_pfree_and_ptrap;

	//struct math_xy charge_change;
	//struct math_xy ntrap_delta_out;
	//struct math_xy ptrap_delta_out;
	//struct math_xy nfree_delta_out;
	//struct math_xy pfree_delta_out;
	//struct math_xy dynamic_charge_tot;
	//struct math_xy tpc_filledn;
	//struct math_xy tpc_filledp;
	//struct math_xy dynamic_np;


	//mobility
	struct math_xy mu_n;
	struct math_xy mu_p;
	struct math_xy mu_n_p_avg;

	//srh rates
	struct math_xy srh_n_r1;
	struct math_xy srh_n_r2;
	struct math_xy srh_n_r3;
	struct math_xy srh_n_r4;

	struct math_xy srh_p_r1;
	struct math_xy srh_p_r2;
	struct math_xy srh_p_r3;
	struct math_xy srh_p_r4;


	//J
	struct math_xy J_y0_n;
	struct math_xy J_y0_p;
	struct math_xy J_y1_n;
	struct math_xy J_y1_p;

	struct math_xy jout;
	struct math_xy jn_avg;
	struct math_xy jp_avg;
	struct math_xy dynamic_jn;
	struct math_xy dynamic_jp;
	struct math_xy jnout_mid;
	struct math_xy jpout_mid;
	struct math_xy iout;
	struct math_xy dynamic_jn_drift;
	struct math_xy dynamic_jn_diffusion;

	struct math_xy dynamic_jp_drift;
	struct math_xy dynamic_jp_diffusion;

	//pl
	struct math_xy dynamic_pl;
	struct math_xy dynamic_pl_tot;

	//field
	struct math_xy E_field;
	struct math_xy dynamic_Vapplied;
	struct math_xy band_bend;

	//other
	struct math_xy dynamic_qe;
	long double ***band_snapshot;
};
#endif
