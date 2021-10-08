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

/** @file ntricks.c
	@brief A collection of helper functions for the newton solver.
*/


#include <stdio.h>
#include <exp.h>
#include "sim.h"
#include "dump.h"
#include "newton_tricks.h"
#include "gui_hooks.h"
#include <plot.h>
#include <cal_path.h>
#include <contacts.h>
#include <dump.h>
#include <log.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inp.h>
#include <device_fun.h>

static int unused __attribute__((unused));

//<stripall>

void state_cache_enable(struct simulation *sim,struct device *in)
{
	char temp[200];
	struct inp_file inp;
	struct json_obj *json_math;
	in->cache.enabled=FALSE;

	json_math=json_obj_find(&(in->config.obj), "math");
	if (json_math==NULL)
	{
		ewe(sim,"Object math not found\n");
	}
	json_get_english(sim, json_math, &(in->cache.enabled),"math_newton_cache");

	if (strcmp(in->newton_name,"newton_simple")==0)
	{
		in->cache.enabled=FALSE;
	}


}
void state_cache_init(struct simulation *sim,struct device *in)
{
	char cache_path[PATH_MAX];

	hash_dir(sim,in->cache.hash);

	gpvdm_mkdir(get_cache_path(sim));

	//join_path(2, cache_path,get_cache_path(sim),in->cache.hash);
	//gpvdm_mkdir(cache_path);

	//state_gen_vector(sim,in);

	in->cache.enabled=FALSE;
}

