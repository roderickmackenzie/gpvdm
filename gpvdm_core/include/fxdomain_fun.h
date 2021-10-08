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
