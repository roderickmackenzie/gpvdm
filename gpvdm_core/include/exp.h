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
void get_avg_mu(struct device *in,long double *ret_mue,long double *ret_muh);
void get_avg_geom_micro_mu(struct device *in,long double *ret_mu);
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
void get_tau(struct device *dev, long double *ret_tau, long double *ret_tau_all);

//thermal 

long double get_avg_Tl(struct device *in);
long double get_avg_Te(struct device *in);
long double get_avg_Th(struct device *in);
long double cal_contact_charge(struct device *in);
#endif
