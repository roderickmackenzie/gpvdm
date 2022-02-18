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

/** @file fit_config.c
@brief load the fit config
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <config.h>
#include "fit.h"
#include <inp.h>
#include <util.h>
#include <cal_path.h>
#include <sys/stat.h>
#include <log.h>
#include <memory.h>
#include <json.h>

static int unused __attribute__((unused));

void fit_gen_plot(struct simulation *sim,struct fitvars *fitconfig)
{
struct math_xy sim_data;
struct math_xy exp_data;

struct json_obj *json_config;
struct json_obj *json_import_config;


int i;
int log_x;
int log_y;
FILE *out;
char temp[PATH_MAX*2];
char exp_name[PATH_MAX/2];
char sim_data_file_name[PATH_MAX];
char sim_name[PATH_MAX/2];
long double start;
long double stop;
long double fit_shift_y;

char sim_error_file[PATH_MAX];
char exp_error_file[PATH_MAX];

int fit_tot=0;
inter_init(sim,&sim_data);
inter_init(sim,&exp_data);
struct json_string buf;

json_string_init(&buf);

for (i=0;i<fitconfig->data_sets;i++)
{
	if (fitconfig->data_set[i].enabled>0)
	{
		fit_tot++;
	}
}

		json_cat_str(&buf,"set term postscript eps enhanced color \"Helvetica\" 8\n");

	out=fopena(sim->root_simulation_path,"fit.plot","w");
	if (out==NULL)
	{
		return;
	}

	if (fit_tot==2)
	{
		json_cat_str(&buf,"set multiplot layout 2, 1 \n");
	}else
	if (fit_tot==3)
	{
		json_cat_str(&buf,"set multiplot layout 3, 1 \n");
	}else
	if (fit_tot==4)
	{
		json_cat_str(&buf,"set multiplot layout 2, 2 \n");
	}else
	if ((fit_tot==5)||(fit_tot==6))
	{
		json_cat_str(&buf,"set multiplot layout 3, 2 \n");
	}else
	if ((fit_tot==7)||(fit_tot==8))
	{
		json_cat_str(&buf,"set multiplot layout 4, 2 \n");
	}else
	if ((fit_tot==9)||(fit_tot==10))
	{
		json_cat_str(&buf,"set multiplot layout 5, 2 \n");
	}

	for (i=0;i<fitconfig->data_sets;i++)
	{

		if (fitconfig->data_set[i].enabled>0)
		{
			json_config=fitconfig->data_set[i].json_fit_config;
			json_import_config=fitconfig->data_set[i].json_fit_import_config;
			json_get_long_double(sim,json_config, &(start),"start");
			json_get_long_double(sim,json_config, &(stop),"stop");
			json_get_english(sim,json_config, &(log_x),"log_x");
			json_get_english(sim,json_config, &(log_y),"log_y");
			json_get_long_double(sim,json_config, &(fit_shift_y),"fit_shift_y");
			json_get_string(sim,json_import_config, temp, "data_file");
			join_path(2, exp_name, sim->root_simulation_path, temp);

			json_get_string(sim,json_config, sim_data_file_name,"sim_data");
			join_path(4,sim_name,sim->root_simulation_path,"sim",fitconfig->data_set[i].fit_agaist,sim_data_file_name);

			sprintf(temp,"%s_sim.dat",fitconfig->data_set[i].fit_name);
			join_path(3,sim_error_file,sim->root_simulation_path,"sim",temp);

			sprintf(temp,"%s_exp.dat",fitconfig->data_set[i].fit_name);
			join_path(3,exp_error_file,sim->root_simulation_path,"sim",temp);

			remove_file_ext(sim_name);
			strcat(sim_name,".best");
			if (isfile(sim_name)!=0)
			{
				remove_file_ext(sim_name);
				strcat(sim_name,".dat");
			}

			//printf("%s\n",sim_name);
			//getchar();
			if (isfile(sim_name)==0)
			{
				fit_cmp_sim_and_exp(sim,json_config, json_import_config, &sim_data,&exp_data,sim_name);

				inter_save(&sim_data,sim_error_file);
				inter_save(&exp_data,exp_error_file);
			}
			inter_free(&exp_data);
			inter_free(&sim_data);

			if (log_x==FALSE)
			{
				json_cat_str(&buf,"\n");

				sprintf(temp,"unset logscale x\n");
				json_cat_str(&buf,temp);
				sprintf(temp,"set autoscale x\n");
				json_cat_str(&buf,temp);
				sprintf(temp,"set xrange [%Le:%Le]\n",start,stop);
				json_cat_str(&buf,temp);
			}else
			{
				json_cat_str(&buf,"\n");
				sprintf(temp,"set autoscale x\n");
				json_cat_str(&buf,temp);
				sprintf(temp,"set logscale x\n");
				json_cat_str(&buf,temp);


				sprintf(temp,"set xrange [%Le:%Le]\n",start,stop);
				json_cat_str(&buf,temp);
			}

			if (log_y==FALSE)
			{
				json_cat_str(&buf,"\n");

				sprintf(temp,"unset logscale y\n");
				json_cat_str(&buf,temp);
				sprintf(temp,"set autoscale y\n");
				json_cat_str(&buf,temp);
			}else
			{
				json_cat_str(&buf,"\n");
				sprintf(temp,"set autoscale y\n");
				json_cat_str(&buf,temp);
				sprintf(temp,"set logscale y\n");
				json_cat_str(&buf,temp);
			}
			sprintf(temp,"plot \\\n");
			json_cat_str(&buf,temp);

			//sprintf(temp,"'%s' using ($1):($2+%Le) axis x1y1 with lp lt 1 title 'exp',\\\n",exp_name,fit_shift_y);
			//json_cat_str(&buf,temp);
			//sprintf(temp,"'%s' using ($1):($2) axis x1y1 with l lt 2 title '%s'",sim_name,fitconfig->data_set[i].fit_agaist);
			//json_cat_str(&buf,temp);

			//sprintf(temp,",\\\n");
			//json_cat_str(&buf,temp);

			sprintf(temp,"'%s' using ($1):($2) axis x2y2 with lp lt 3 title 'exp',\\\n",exp_error_file);
			json_cat_str(&buf,temp);

			sprintf(temp,"'%s' using ($1):($2) axis x2y2 with l lt 4 title 'sim'\n",sim_error_file);
			json_cat_str(&buf,temp);

		}
	}


fprintf(out,"%s\n",buf.data);
fclose(out);

 json_string_free(&buf);
}



