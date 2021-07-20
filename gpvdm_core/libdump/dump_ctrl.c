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

/** @file dump_ctrl.c
@brief set/get what is allowed to be written to disk, writing to disk is bad.
*/

#include <dump_ctrl.h>
#include <gpvdm_const.h>
#include <sim.h>
#include <json.h>

int dump_can_i_dump(struct simulation *sim,struct device *dev, char *file_name)
{
	if (sim->fitting==FIT_NOT_FITTING)
	{
		return 0;
	}

	int i;
	int data_sets;
	char temp[200];
	char sim_file[200];
	struct json_obj *json_data_set;
	struct json_obj *json_config;
	struct json_obj *json_fits;
	json_fits=json_obj_find(&(dev->config.obj), "fits");
	if (json_fits==NULL)
	{
		ewe(sim,"Object fits not found\n");
	}

	json_fits=json_obj_find(json_fits, "fits");
	if (json_fits==NULL)
	{
		ewe(sim,"Object fits not found\n");
	}


	json_get_int(sim,json_fits, &data_sets,"data_sets");
	//printf("data sets=%d\n",data_sets);
	for (i=0;i<data_sets;i++)
	{
		sprintf(temp,"data_set%d",i);
		json_data_set=json_obj_find(json_fits, temp);
		if (json_data_set==NULL)
		{
			ewe(sim,"Object fits not found\n");
		}

		json_config=json_obj_find(json_data_set, "config");
		if (json_config==NULL)
		{
			ewe(sim,"Object fits not found\n");
		}
		json_get_string(sim,json_config, sim_file,"sim_data");
		if (strcmp(file_name,sim_file)==0)
		{
			return 0;
		}
		

	}

return -1;
}

int get_dump_status(struct simulation *sim,int a)
{
	return sim->dump_array[a];
}

void set_dump_status(struct simulation *sim,int name, int a)
{
if ((sim->dump_array[dump_lock]==FALSE)||(name==dump_lock)) sim->dump_array[name]=a;
}

