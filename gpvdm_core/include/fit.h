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

/** @file fit.h
@brief functions for fitting.
*/

#ifndef fith
#define fith
#include <advmath.h>
#include <sim_struct.h>


#define FIT_RUN 0
#define FIT_FINISH 1
#define FIT_RESET 2

#define FIT_MAX 100
#define FIT_VAR_MAX 2000

struct fits_struct
{
int enabled;
char fit_name[200];
char fit_path[200];
char fit_plugin[200];
double error;
};

struct fititem
{
	double min;
	double max;
	double add_error;
	char fit_file[100];
	char fit_token[100];
};

struct fitvars
{
	struct fits_struct data_set[FIT_MAX];

	double simplexmul;
	int simplexreset;
	int fitvars;
	struct fititem fititem[FIT_VAR_MAX];
	int randomize;
	int random_reset_ittr;
	double disable_reset_at;
	double converge_error;
	int enable_simple_reset;
	double constraints_error[100];
	int n_constraints;
	int iterations;
	int sub_iterations;
	int sub_iterations_two;
	int stall_steps;
	int fit_method;
};

int fit_simplex(struct simulation *sim,struct fitvars *fitconfig);
void fit_build_jobs(struct simulation *sim,struct fitvars *fitconfig);
double get_all_error(struct simulation *sim,struct fitvars *myfit);
double get_constraints_error(struct simulation *sim,struct fitvars *config);
int fit_read_config(struct simulation *sim,struct fitvars *fitconfig);
double fit_run_sims(struct simulation *fit,struct fitvars *fitconfig);
int fit_now(struct simulation *sim,struct fitvars *fitconfig);
double fit_load_plugin(struct simulation *sim,struct fitvars *config,int i);
void duplicate(struct simulation *sim);
int get_fit_crashes(struct simulation *sim,struct fitvars *fitconfig);
void fit_init(struct simulation *sim,struct fitvars *fitconfig);
void my_f_set_globals(struct simulation *sim, struct fitvars *config);
int fit_newton(struct simulation *sim,struct fitvars *fitconfig);

double my_f (double *p,int len);
//void  my_df (const gsl_vector *v, void *params,  gsl_vector *df);
//void my_fdf (const gsl_vector *x, void *params, double *f, gsl_vector *df) ;
void fit_dump_log(struct simulation *sim,struct fitvars *fitconfig,double error,double size);


#endif
