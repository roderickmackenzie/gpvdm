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


int fit_read_config(struct simulation *sim,struct fitvars *fitconfig)
{
	fitconfig->n_constraints=0;
	int i=0;
	int fit_enabled;
	int total_fits=0;
	struct json_obj *json_fits;
	struct json_obj *json_fits_config;
	struct json_obj *json_fits_vars;
	struct json_obj *json_seg;
	struct json_obj *json_fits_fits;
	struct json_obj *json_data_set;
	struct json_obj *json_data_set_config;

	char json_path[PATH_MAX];
	int segments=0;
	char temp[200];

	join_path(2,json_path,sim->root_simulation_path,"sim.json");
	json_load(sim,&(fitconfig->config),json_path);

	json_fits=json_obj_find(&(fitconfig->config.obj), "fits");
	if (json_fits==NULL)
	{
		ewe(sim,"Object fits not found\n");
	}



	json_fits_config=json_obj_find(json_fits, "fit_config");
	if (json_fits_config==NULL)
	{
		ewe(sim,"Object fit_config not found\n");
	}

	json_get_english(sim,json_fits_config, &(fitconfig->randomize),"fit_randomize");
	json_get_int(sim,json_fits_config, &(fitconfig->stall_steps),"fit_stall_steps");
	json_get_int(sim,json_fits_config, &(fitconfig->random_reset_ittr),"fit_random_reset_ittr");
	json_get_double(sim,json_fits_config, &(fitconfig->disable_reset_at),"fit_disable_reset_at");
	json_get_english(sim,json_fits_config, &(fitconfig->enable_simple_reset),"fit_enable_simple_reset");

	json_get_double(sim,json_fits_config, &(fitconfig->converge_error),"fit_converge_error");
	json_get_double(sim,json_fits_config, &(fitconfig->simplexmul),"fit_simplexmul");

	json_get_int(sim,json_fits_config, &(fitconfig->simplexreset),"fit_simplex_reset");
	json_get_english(sim,json_fits_config, &(fitconfig->fit_method),"fit_method");


	//Fit vars
	json_fits_vars=json_obj_find(json_fits, "vars");
	if (json_fits_vars==NULL)
	{
		ewe(sim,"Object vars not found\n");
	}

	
	json_get_int(sim,json_fits_vars, &segments,"segments");
	fitconfig->fitvars=0;
	for (i=0;i<segments;i++)
	{
		sprintf(temp,"segment%d",i);
		json_seg=json_obj_find(json_fits_vars, temp);
		if (json_seg==NULL)
		{
			ewe(sim,"Object %s not found\n",temp);
		}

		json_get_english(sim,json_seg, &(fit_enabled),"fit_var_enabled");
		if (fit_enabled==TRUE)
		{
			json_get_string(sim,json_seg, fitconfig->fititem[fitconfig->fitvars].json_token,"json_var");
			json_get_double(sim,json_seg, &(fitconfig->fititem[fitconfig->fitvars].min),"min");
			json_get_double(sim,json_seg, &(fitconfig->fititem[fitconfig->fitvars].max),"max");
			json_get_english(sim,json_seg, &(fitconfig->fititem[fitconfig->fitvars].log_fit),"log_fit");
			json_get_double(sim,json_seg, &(fitconfig->fititem[fitconfig->fitvars].add_error),"error");
			fitconfig->fitvars++;
		}
		
	}

	//Fit vars
	json_fits_fits=json_obj_find(json_fits, "fits");
	if (json_fits_fits==NULL)
	{
		ewe(sim,"Object fits not found\n");
	}
	json_get_int(sim,json_fits_fits, &(fitconfig->data_sets),"data_sets");

	malloc_1d((void**)&(fitconfig->data_set),fitconfig->data_sets, sizeof(struct fit_data));

	total_fits=0;

	for (i=0;i<fitconfig->data_sets;i++)
	{
		//data set
		fitconfig->data_set[i].enabled=FALSE;
		sprintf(temp,"data_set%d",i);

		json_data_set=json_obj_find(json_fits_fits, temp);
		if (json_data_set==NULL)
		{
			ewe(sim,"Object json_data_set not found\n");
		}

		//data set fit patch
		fitconfig->data_set[i].json_fit_patch=json_obj_find(json_data_set, "fit_patch");
		if (fitconfig->data_set[i].json_fit_patch==NULL)
		{
			ewe(sim,"Object fit_patch not found\n");
		}

		//data set import config
		fitconfig->data_set[i].json_fit_import_config=json_obj_find(json_data_set, "import_config");
		if (fitconfig->data_set[i].json_fit_import_config==NULL)
		{
			ewe(sim,"Object import_config not found\n");
		}
	
		//data set fit config
		json_data_set_config=json_obj_find(json_data_set, "config");
		fitconfig->data_set[i].json_fit_config=json_data_set_config;
		if (json_data_set_config==NULL)
		{
			ewe(sim,"Object json_data_set_config not found\n");
		}

		json_get_english(sim,json_data_set_config, &(fitconfig->data_set[i].enabled),"enabled");
		json_get_string(sim,json_data_set_config, fitconfig->data_set[i].fit_name,"fit_name");
		json_get_string(sim,json_data_set_config, fitconfig->data_set[i].fit_agaist,"fit_against");
		//printf("'%s' '%s'\n",fitconfig->data_set[i].fit_agaist,fitconfig->data_set[i].fit_name);
		//getchar();
		if (strcmp(fitconfig->data_set[i].fit_agaist,"self")==0)
		{
			strcpy(fitconfig->data_set[i].fit_agaist,fitconfig->data_set[i].fit_name);
			fitconfig->data_set[i].run_this_simulation=TRUE;
		}else
		{
			fitconfig->data_set[i].run_this_simulation=FALSE;
		}

		json_get_string(sim,json_data_set_config, fitconfig->data_set[i].sim_data,"sim_data");

		join_path(3,fitconfig->data_set[i].fit_path,sim->root_simulation_path,"sim",fitconfig->data_set[i].fit_name);
		printf_log(sim,"fitconfig->data_set[i].fit_path = %s |sim| %s\n",sim->root_simulation_path,fitconfig->data_set[i].fit_name);

		if (fitconfig->data_set[i].enabled!=0)
		{
			total_fits++;
		}


	}

	if (total_fits==0)
	{
		ewe(sim,"There are no data sets to fit.");
	}



return 0;
}

