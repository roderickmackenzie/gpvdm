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

/** @file fit.h
@brief functions for fitting.
*/

#ifndef fith
#define fith
#include <advmath.h>
#include <sim_struct.h>
#include <simplex.h>
#include <json.h>

#define FIT_RUN 0
#define FIT_FINISH 1
#define FIT_RESET 2

#define FIT_VAR_MAX 2000


struct fit_data
{
	int enabled;
	int run_this_simulation;
	char fit_name[200];
	char fit_agaist[200];
	char fit_path[PATH_MAX];
	char sim_data[200];
	struct json_obj *json_fit_patch;
	struct json_obj *json_fit_config;
	struct json_obj *json_fit_import_config;
	double error;
};

struct fititem
{
	double min;
	double max;
	double add_error;
	int log_fit;
	char json_token[200];
};

struct fitvars
{
	int data_sets;
	struct fit_data *data_set;

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
	double best_ever_error;

	//configure
	struct json config;
};

int fit_simplex(struct simulation *sim,struct fitvars *fitconfig);
void fit_build_jobs(struct simulation *sim,struct fitvars *fitconfig);
double get_all_error(struct simulation *sim,struct fitvars *myfit);
double get_constraints_error(struct simulation *sim,struct fitvars *config);
int fit_read_config(struct simulation *sim,struct fitvars *fitconfig);
double fit_run_sims(struct simulation *fit,struct fitvars *fitconfig);
int fit_now(struct simulation *sim,struct fitvars *fitconfig);
void duplicate(struct simulation *sim,struct fitvars *fitconfig,struct json *j);
void fit_patch(struct simulation *sim,char *path,struct json_obj *json_patch);
int get_fit_crashes(struct simulation *sim,struct fitvars *fitconfig);
void fit_init(struct simulation *sim,struct fitvars *fitconfig);
int fit_newton(struct simulation *sim,struct fitvars *fitconfig);
void fit_free(struct simulation *sim,struct fitvars *fitconfig);

double my_f (void *min,double *p);
//void  my_df (const gsl_vector *v, void *params,  gsl_vector *df);
//void my_fdf (const gsl_vector *x, void *params, double *f, gsl_vector *df) ;
void fit_dump_log(struct simulation *sim,struct fitvars *fitconfig,double error,double size);
void mass_copy_file(struct simulation *sim,struct fitvars *fitconfig,char *input,int n);
void fit_build_jobs(struct simulation *sim,struct fitvars *fitconfig);
void fit_load_vars(struct simulation *sim,struct multimin *data,char *path,struct fitvars *fitconfig);
void fit_save_vars(struct simulation *sim, struct json *j,double *p,struct fitvars *fitconfig);
void fit_save_best_answer(struct simulation *sim,struct fitvars *fitconfig,double *p,double error);
void fit_cmp_sim_and_exp(struct simulation *sim,struct json_obj *json_config, struct json_obj *json_import_config, struct math_xy *sim_data,struct math_xy *exp_data,char *sim_name);
double fit_get_sim_error(struct simulation *sim,struct fitvars *config, int fit_number,int force_dump);
void fit_gen_plot(struct simulation *sim,struct fitvars *fitconfig);
#endif
