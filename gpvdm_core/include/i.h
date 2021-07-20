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


/** @file i.h
	@brief Header file for i.c
*/
#ifndef i_h
#define i_h
#include "advmath.h"
#include <sim_struct.h>
#include <i_struct.h>

void inter_malloc(struct math_xy* in,int len);
void inter_realloc(struct math_xy* in,int len);
int inter_get_col_n(struct simulation *sim,char *name);
void inter_add_to_hist(struct math_xy* in,gdouble pos,gdouble value);
void inter_init_mesh(struct math_xy* in,int len,gdouble min,gdouble max);
void math_xy_smooth_range(struct math_xy* out,struct math_xy* in,int points,gdouble x);
gdouble inter_avg_range(struct math_xy* in,gdouble start,gdouble stop);
gdouble inter_array_get_max(gdouble *data,int len);
void math_xy_div(struct simulation *sim,struct math_xy* one,struct math_xy* two);
void inter_div_long_double(struct math_xy* in,gdouble div);
gdouble inter_get_min_range(struct math_xy* in,gdouble min, gdouble max);
void inter_make_cumulative(struct math_xy* in);
void inter_y_mul_dx(struct math_xy* in);
void inter_add_x(struct math_xy* in,gdouble value);
int inter_sort(struct math_xy* in);
gdouble inter_get_quartile(struct math_xy* in,gdouble value);
void inter_save_seg(struct math_xy* in,char *path,char *name,int seg);
gdouble inter_intergrate(struct math_xy* in);
void inter_to_log_mesh(struct math_xy* out,struct math_xy* in);
void math_xy_smooth(struct math_xy* out,struct math_xy* in,int points);
gdouble inter_sum_mod(struct math_xy* in);
void inter_set_value(struct math_xy* in,gdouble value);
gdouble inter_get_neg(struct math_xy* in,gdouble x);
gdouble inter_get_noend(struct math_xy* in,gdouble x);
void inter_to_new_mesh(struct math_xy* in,struct math_xy* out);
void inter_swap(struct math_xy* in);
void inter_log_y_m(struct math_xy* in);
gdouble inter_get_min(struct math_xy* in);
gdouble inter_get_fabs_max(struct math_xy* in);
gdouble inter_norm_to_one_range(struct math_xy* in,gdouble min,gdouble max);
void inter_chop(struct math_xy* in,gdouble min, gdouble max);
void inter_save_a(struct math_xy* in,char *path,char *name);
void inter_dump(struct simulation *sim,struct math_xy* in);
void inter_purge_zero(struct math_xy* in);
void inter_append(struct math_xy* in,gdouble x,gdouble y);
void inter_init(struct simulation *sim,struct math_xy* in);
void inter_sub_long_double(struct math_xy* in,gdouble value);
void inter_sub(struct simulation *sim,struct math_xy* one,struct math_xy* two);
gdouble inter_sum(struct math_xy* in);
void inter_copy(struct math_xy* in,struct math_xy* orig,int alloc);
int inter_get_col(char *file);
void inter_load_by_col(struct simulation *sim,struct math_xy* in,char *name,int col);
gdouble inter_get_diff(struct math_xy* delta,struct math_xy* one,struct math_xy* two,struct math_xy* mull,long double window_left);
void inter_pow(struct math_xy* in,gdouble p);
gdouble inter_get_raw(gdouble *x,gdouble *data,int len,gdouble pos);
float math_interpolate_raw_float(long double *x,float *data,int len,long double pos);
gdouble inter_norm(struct math_xy* in,gdouble mul);
void inter_log_y(struct math_xy* in);
void math_xy_mul_long_double(struct math_xy* in,gdouble mul);
void inter_log_x(struct math_xy* in);
void inter_save(struct math_xy* in,char *name);
int inter_load(struct simulation *sim,struct math_xy* in,char *name);
gdouble inter_get_hard(struct math_xy* in,gdouble x);
gdouble inter_get(struct math_xy* in,gdouble x);
void inter_print(struct math_xy* in);
void inter_free(struct math_xy* in);
void inter_rescale(struct math_xy* in,gdouble xmul, gdouble ymul);
void inter_mod(struct math_xy* in);
void inter_add(struct math_xy* out,struct math_xy* in);
void inter_norm_area(struct math_xy* in,gdouble mul);
gdouble inter_get_max(struct math_xy* in);
gdouble inter_get_max_range(struct math_xy* in,int start, int stop);
void inter_add_long_double(struct math_xy* in,gdouble value);
gdouble inter_intergrate_lim(struct math_xy* in,gdouble from, gdouble to);
void inter_deriv(struct math_xy* out,struct math_xy* in);
void inter_import_array(struct math_xy* in,gdouble *x,gdouble *y,int len,int alloc);
gdouble inter_avg(struct math_xy* in);
void inter_convolve(struct math_xy* one,struct math_xy* two);
void inter_save_backup(struct math_xy* in,char *name,int backup);
void math_xy_dft(long double *real,long double *imag,struct math_xy* in,gdouble fx);
void math_xy_dft_extract(long double * dc,long double *real,long double *imag,struct math_xy* in,long double fx);
int inter_get_max_pos(struct math_xy* in);
int inter_search_pos(struct math_xy* in,gdouble x);
void inter_join_bins(struct math_xy* in,gdouble delta);
void inter_reset(struct math_xy* in);
void inter_find_peaks(struct math_xy* out,struct math_xy* in,int find_max);
void math_xy_sin(struct math_xy *in,gdouble mag,gdouble fx,gdouble delta);
void math_xy_cos(struct math_xy *in,long double mag,long double fx,long double phi);
void inter_purge_x_zero(struct math_xy* in);
int inter_search_token(struct simulation *sim,long double *value,char *token,char *name);
int inter_get_min_pos(struct math_xy* in);
void math_xy_get_left_right_start(struct math_xy* in,int *left,int *right, long double fraction);

long double math_xy_get_delta(struct math_xy* one,struct math_xy* two);
void math_xy_dft_full(struct simulation *sim,struct math_xy *fx_data,struct math_xy* data, int start_fx, int stop_fx);
float math_interpolate_raw_double(long double *x,double *data,int len,long double pos);
#endif
