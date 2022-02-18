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

/** @file fit_vars.c
@brief Load and save fit variables
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <i.h>
#include <server.h>
#include "fit.h"
#include <dump_ctrl.h>
#include <timer.h>
#include <util.h>
#include <rand.h>
#include <gpvdm_const.h>
#include <cal_path.h>
#include <log.h>

#include <gui_hooks.h>

#include <simplex.h>
#include <lock.h>
#include <json.h>

static int unused __attribute__((unused));


void fit_load_vars(struct simulation *sim,struct multimin *data,char *path,struct fitvars *fitconfig)
{
	struct json j;
	struct json_obj *obj;
	char json_path[PATH_MAX];
	int fitcount=0;
	double val;

	json_init(&j);
	join_path(2,json_path,path,"sim.json");
	json_load(sim,&j,json_path);

	for (fitcount=0;fitcount<fitconfig->fitvars;fitcount++)
	{
		obj=json_obj_find_by_path(sim,&(j.obj), fitconfig->fititem[fitcount].json_token);
		sscanf(obj->data,"%le",&val);

		if (fitconfig->fititem[fitcount].log_fit==FALSE)
		{
			data->x[fitcount]=val;
		}else
		{
			data->x[fitcount]=log10(val);
			printf(">>>>>>>>>>>fred >>>%le %le\n",data->x[fitcount],val);
		}

		data->s[fitcount]=fitconfig->simplexmul;
	}

	json_free(&j);

}

void fit_save_vars(struct simulation *sim, struct json *j,double *p,struct fitvars *fitconfig)
{

	struct json_obj *obj;
	//char json_path[PATH_MAX];

	int fitcount=0;
	double val;


	for (fitcount=0;fitcount<fitconfig->fitvars;fitcount++)
	{
		val = p[fitcount];

		obj=json_obj_find_by_path(sim,&(j->obj), fitconfig->fititem[fitcount].json_token);
		if (fitconfig->fititem[fitcount].log_fit==FALSE)
		{
			sprintf(obj->data,"%le",val);
		}else
		{
			sprintf(obj->data,"%le",pow(10,val));
			//printf(">>>>>>>>>>>fred2 >>>%le %le\n",pow(10,val),val);
		}
	}

}

void fit_save_best_answer(struct simulation *sim,struct fitvars *fitconfig,double *p,double error)
{
	char path_in[PATH_MAX];
	char path_out[PATH_MAX];
	char json_path[PATH_MAX];
	struct json j;
	json_init(&j);

	if (fitconfig->iterations>0)
	{
		if (error<fitconfig->best_ever_error)
		{
			//printf("%le %le\n",error,fitconfig->best_ever_error);
			//getchar();
			//fit_save_vars(sim,get_input_path(sim),p,fitconfig);
			join_path(2,json_path,sim->root_simulation_path,"sim.json");
			json_load(sim,&j,json_path);
			j.compact=TRUE;
			fit_save_vars(sim,&j,p,fitconfig);
			duplicate(sim,fitconfig,&j);
			json_save(sim,&j);
			json_free(&j);

			
			//printf("I want to update the answer %s\n",get_input_path(sim));
			//getchar();
			int i;
			for (i=0;i<fitconfig->data_sets;i++)
			{
				if (fitconfig->data_set[i].enabled==1)
				{
					//strcpy(sim->input_path,fitconfig->data_set[i].fit_path);
					join_path(4,path_in,sim->root_simulation_path,"sim",fitconfig->data_set[i].fit_agaist,fitconfig->data_set[i].sim_data);
					//fitconfig->data_set[i].fit_path,fitconfig->data_set[i].sim_data);
					join_path(2,path_out,fitconfig->data_set[i].fit_path,fitconfig->data_set[i].sim_data);
					remove_file_ext(path_out);
					strcat(path_out,".best");
					copy_file(sim,path_out,path_in);

					fit_get_sim_error(sim,fitconfig, i, TRUE);
				}
			}
			//getchar();
			fit_gen_plot(sim,fitconfig);
		}

		fitconfig->best_ever_error=error;
	}
}
