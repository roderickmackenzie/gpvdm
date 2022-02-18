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

/** @file fit_run.c
@brief run the fit to expeimental data
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <i.h>
#include <util.h>
#include <unistd.h>
#include <server.h>
#include <sim.h>
#include <dump.h>
#include "fit.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <log.h>
#include <gui_hooks.h>
#include <cal_path.h>
#include <dos.h>
#include <lock.h>
#include <memory.h>
#include <device_fun.h>

void fit_cmp_sim_and_exp(struct simulation *sim,struct json_obj *json_config, struct json_obj *json_import_config, struct math_xy *sim_data,struct math_xy *exp_data,char *sim_name)
{
}

double fit_get_sim_error(struct simulation *sim,struct fitvars *config, int fit_number,int force_dump)
{
	gdouble error=0.0;

	struct math_xy exp;
	struct math_xy sim_data;
	struct math_xy delta;
	struct math_xy mul_data;

	inter_init(sim,&exp);
	inter_init(sim,&sim_data);
	inter_init(sim,&delta);
	inter_init(sim,&mul_data);

	struct dat_file buf;

	dat_file_init(&buf);

	long double mul;

	char out_name_exp[PATH_MAX];
	char out_name_sim[PATH_MAX];
	char out_name_delta[PATH_MAX];
	char sim_name[PATH_MAX];
	char sim_data_file_name[PATH_MAX];

	//char out_sim_info[PATH_MAX];

	long double fit_set_error_to_zero_before=0.0;
	struct json_obj *json_config;
	struct json_obj *json_import_config;

	printf("in plugin\n");

	json_config=config->data_set[fit_number].json_fit_config;
	json_import_config=config->data_set[fit_number].json_fit_import_config;


	json_get_string(sim,json_config, sim_data_file_name,"sim_data");
	join_path(4,sim_name,sim->root_simulation_path,"sim",config->data_set[fit_number].fit_agaist,sim_data_file_name);
	fit_cmp_sim_and_exp(sim,json_config,json_import_config,&sim_data,&exp,sim_name);
	json_get_long_double(sim,json_config, &fit_set_error_to_zero_before,"fit_set_error_to_zero_before");
	json_get_long_double(sim,json_config, &(mul),"fit_error_mul");
	join_path(4,out_name_exp,sim->root_simulation_path,"sim",config->data_set[fit_number].fit_name,"fit_error_exp.dat");
	join_path(4,out_name_sim,sim->root_simulation_path,"sim",config->data_set[fit_number].fit_name,"fit_error_sim.dat");
	join_path(4,out_name_delta,sim->root_simulation_path,"sim",config->data_set[fit_number].fit_name,"fit_error_delta.dat");

	inter_copy(&mul_data,&exp,TRUE);
	math_xy_set_value(&mul_data,mul);
	error=inter_get_diff(&delta,&exp,&sim_data,&mul_data,fit_set_error_to_zero_before);

	if ((sim->fitting==FIT_SINGLE_FIT)||(force_dump==TRUE))
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		strcpy(buf.title,"x - y");
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,"x");
		strcpy(buf.data_label,"y");

		strcpy(buf.y_units,"au");
		strcpy(buf.data_units,"au");

		buf.logscale_data=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,exp.x, exp.data, exp.len);
		buffer_dump(sim,out_name_exp,&buf);
		buffer_free(&buf);



		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		strcpy(buf.title,"x - y");
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,"x");
		strcpy(buf.data_label,"y");
		strcpy(buf.y_units,"au");
		strcpy(buf.data_units,"au");
		buf.logscale_data=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,sim_data.x, sim_data.data, sim_data.len);
		buffer_dump(sim,out_name_sim,&buf);
		buffer_free(&buf);

		//inter_save(&exp,"a.dat");
		//inter_save(&sim_data,"b.dat");

		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		strcpy(buf.title,"x - y");
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,"x");
		strcpy(buf.data_label,"y");
		strcpy(buf.y_units,"au");
		strcpy(buf.data_units,"au");
		buf.logscale_data=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,delta.x, delta.data, delta.len);
		buffer_dump(sim,out_name_delta,&buf);
		buffer_free(&buf);

		fit_gen_plot(sim,config);

	}

	inter_free(&exp);
	inter_free(&sim_data);
	inter_free(&mul_data);
	inter_free(&delta);
	printf("exit plugin\n");
	return (double)error;
}
