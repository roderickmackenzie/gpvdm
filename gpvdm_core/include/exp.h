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

/** @file exp.h
	@brief Functions to meaure physical things from the device, such as current.
*/


#ifndef exp_h
#define exp_h
#include <device.h>

gdouble get_jn_avg(struct device *in);
gdouble get_jp_avg(struct device *in);
gdouble get_charge_change(struct device *in);
void cal_J_drift_diffusion(struct device *in);
gdouble get_Jn_diffusion(struct device *in);
gdouble get_Jn_drift(struct device *in);
gdouble get_Jp_diffusion(struct device *in);
gdouble get_Jp_drift(struct device *in);
gdouble get_avg_field(struct device *in);
gdouble get_np_tot(struct device *in);
void reset_npequlib(struct device *in);
void get_avg_np_pos(struct device *in,gdouble *nx,gdouble *px);
gdouble get_background_charge(struct device *in);
void reset_np_save(struct device *in);
gdouble get_n_trapped_charge(struct device *in);
gdouble get_p_trapped_charge(struct device *in);
gdouble get_avg_recom(struct device *in);
gdouble get_avg_recom_n(struct device *in);
gdouble get_avg_recom_p(struct device *in);
gdouble get_avg_Rn(struct device *in);
gdouble get_avg_Rp(struct device *in);
gdouble get_avg_k(struct device *in);
gdouble get_avg_mue(struct device *in);
gdouble get_avg_muh(struct device *in);
gdouble get_free_n_charge(struct device *in);
gdouble get_free_p_charge(struct device *in);
gdouble get_free_n_charge_delta(struct device *in);
gdouble get_free_p_charge_delta(struct device *in);
gdouble get_total_n_trapped_charge(struct device *in);
gdouble get_total_p_trapped_charge(struct device *in);
gdouble get_n_trapped_charge_delta(struct device *in);
gdouble get_p_trapped_charge_delta(struct device *in);
gdouble get_avg_relax_n(struct device *in);
gdouble get_avg_relax_p(struct device *in);
gdouble get_avg_J(struct device *in);
gdouble get_free_np_avg(struct device *in);
gdouble get_extracted_np(struct device *in);
gdouble get_extracted_k(struct device *in);
gdouble get_charge_delta(struct device *in);
gdouble get_I_recomb(struct device *in);
gdouble get_J_left(struct device *in);
gdouble get_J_right(struct device *in);
gdouble get_J_recom(struct device *in);
gdouble get_I_ce(struct simulation *sim,struct device *in);
gdouble get_equiv_I(struct simulation *sim,struct device *in);
gdouble carrier_count_get_rn(struct device *in);
gdouble carrier_count_get_rp(struct device *in);

gdouble carrier_count_get_n(struct device *in);
gdouble carrier_count_get_p(struct device *in);
void carrier_count_reset(struct device *in);
void carrier_count_add(struct device *in);
gdouble get_extracted_n(struct device *in);
gdouble get_extracted_p(struct device *in);
gdouble get_equiv_V(struct simulation *sim,struct device *in);
gdouble get_equiv_J(struct simulation *sim,struct device *in);
gdouble get_I(struct device *in);
gdouble get_J(struct device *in);
gdouble get_charge(struct device *in);
gdouble get_avg_gen(struct device *in);
void set_orig_charge_den(struct device *in);
gdouble get_total_np(struct device *in);
gdouble get_charge_tot(struct device *in);
gdouble get_tot_photons_abs(struct device *in);
gdouble get_i_intergration(struct device *in);
gdouble get_avg_J_std(struct device *in);
gdouble get_max_Jsc(struct device *in);
#endif
