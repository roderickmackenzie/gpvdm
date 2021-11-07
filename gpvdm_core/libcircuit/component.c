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

/** @file component.c
@brief Deals with electrical components
*/

#include "device.h"
#include "util.h"
#include "gpvdm_const.h"
#include <log.h>
#include <cal_path.h>
#include <circuit.h>
#include <matrix.h>
#include <memory.h>
#include <contacts.h>
#include <epitaxy_struct.h>
#include <epitaxy.h>
#include <exp.h>
#include <dump.h>
#include <lang.h>
#include <component.h>
#include <component_fun.h>

void component_init(struct simulation *sim,struct component *com)
{
	strcpy(com->component,"none");
	com->shunt=-1.0;
	com->series_z=-1.0;
	com->series_x=-1.0;
	com->series_y=-1.0;
}

void component_load_electrical_file(struct simulation *sim,struct component *com,struct json_obj *json_obj)
{
	char temp[200];

	if (json_obj==NULL)
	{
		ewe(sim,"Error loading json file for electrical components\n");
		com->shunt=0.0;
		com->series_z=0.0;
		com->series_x=0.0;
		com->series_y=0.0;
		com->n_ideality=0.0;
		com->J0=0.0;
	}else
	{
		json_get_string(sim, json_obj, com->component,"electrical_component");

		json_get_long_double(sim,json_obj, &(com->shunt),"electrical_shunt");

		json_get_string(sim, json_obj, temp,"electrical_symmetrical_resistance");

		json_get_long_double(sim,json_obj, &(com->series_z),"electrical_series_z");

		json_get_long_double(sim,json_obj, &(com->series_x),"electrical_series_x");

		json_get_long_double(sim,json_obj, &(com->series_y),"electrical_series_y");

		if (strcmp(temp,"symmetric")==0)
		{
			com->series_z=com->series_y;
			com->series_x=com->series_y;
		}

		json_get_long_double(sim,json_obj, &(com->n_ideality),"electrical_n");
	
		json_get_long_double(sim,json_obj, &(com->J0),"electrical_J0");

		json_get_english(sim,json_obj, &(com->electrical_enable_generation),"electrical_enable_generation");


	}
}


