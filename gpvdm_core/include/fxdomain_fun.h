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

/** @file fxdomain_fun.h
@brief Code to read input files.
*/

#ifndef fxdomain_fun_h
#define fxdomain_fun_h
#include "advmath.h"
#include "inp_struct.h"
#include <sim_struct.h>
#include "list_struct.h"
#include <lib_fxdomain.h>
#include <json.h>

//fitting
void fit_cos(struct simulation *sim,int dump_fit_progress_data,long double *fit_error,gdouble *ret_mag,gdouble *ret_delta,struct math_xy *input_data,gdouble fx,char * prefix, char *output_path);
double sin_f (double *p,int len);

//fxdomain
void fxdomain_solve(struct simulation *sim,struct device *in,struct fxdomain *fxdomain_config);

void fxdomain_init(struct simulation *sim,struct fxdomain *fxdomain_config);
void fxdomain_malloc(struct simulation *sim,struct fxdomain *fxdomain_config);
void fxdomain_cpy(struct simulation *sim,struct fxdomain *out,struct fxdomain *in);
void fxdomain_free(struct simulation *sim,struct fxdomain *fxdomain_config);


void fxdomain_dump(struct simulation *sim,struct fxdomain *fxdomain_config);
int fxdomain_fit(struct simulation *sim,struct device *in,struct fxdomain *fxdomain_config);
void fxdomain_reset(struct simulation *sim,struct fxdomain *fxdomain_config);
void fxdomain_load_config(struct simulation *sim,struct fxdomain *fxdomain_config,struct device *dev,struct json_obj *json_is);
void fxdomain_large_signal_solve(struct simulation *sim,struct device *in,struct fxdomain *fxdomain_config);
void fxdomain_small_signal_solve(struct simulation *sim,struct device *in,struct fxdomain *fxdomain_config);
void fxdomain_cal_complex_j(struct simulation *sim,struct device *in,struct newton_state_complex *ns,int z,int x);
void fxdomain_dft(struct simulation *sim,struct device *in,struct fxdomain *fxdomain_config);
int fxdomain_large_signal_test(struct simulation *sim,struct device *in,struct fxdomain *fxdomain_config);

//mesh
void fx_mesh_save(struct simulation *sim,struct fxdomain *config);
void fx_load_mesh(struct simulation *sim,struct fxdomain *config,struct json_obj *json_is);
void fx_mesh_step(struct simulation *sim,struct fxdomain *config);
int fx_mesh_points(struct fxdomain *config);
int fx_mesh_run(struct fxdomain *config);
long double fx_mesh_get_fx(struct fxdomain *config);
int fx_mesh_get_step(struct fxdomain *config);
#endif
