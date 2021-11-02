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

/** @file dump_ctrl.c
@brief set/get what is allowed to be written to disk, writing to disk is bad.
*/

#include <dump_ctrl.h>
#include <gpvdm_const.h>
#include <sim.h>
#include <json.h>

int dump_can_i_dump(struct simulation *sim,struct device *dev, char *file_name)
{
	int i;
	int data_sets;
	char temp[200];
	char sim_file[200];
	struct json_obj *json_data_set;
	struct json_obj *json_config;
	struct json_obj *json_fits;

	struct json_obj *json_dump;
	struct json_obj *json_banned_files;
	struct json_obj *json_banned_file;
	int banned_files;
	char banned_file_name[200];

	if (sim->fitting==FIT_NOT_FITTING)
	{
		json_dump=json_obj_find(&(dev->config.obj), "dump");
		if (json_dump==NULL)
		{
			ewe(sim,"Object dump not found\n");
		}

		json_banned_files=json_obj_find(json_dump, "banned_files");
		if (json_banned_files==NULL)
		{
			ewe(sim,"Object banned_files not found\n");
		}

		json_get_int(sim,json_banned_files, &banned_files,"segments");

		for (i=0;i<banned_files;i++)
		{
			sprintf(temp,"segment%d",i);
			json_banned_file=json_obj_find(json_banned_files, temp);
			if (json_banned_file==NULL)
			{
				ewe(sim,"Object band_file segment not found\n");
			}

			json_get_string(sim,json_banned_file, banned_file_name,"banned_file_name");
			if (strcmp(file_name,banned_file_name)==0)
			{
				return -1;
			}
			

		}


		return 0;
	}


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

