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

/** @file hard_limit.c
	@brief Hard limit input values from files, prevents crashes during fitting.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sim_struct.h>
#include <json.h>
#include <hard_limit_struct.h>
#include <hard_limit.h>
#include "gpvdm_const.h"


void hard_limit_init(struct simulation *sim,struct hard_limit *hl)
{
	hl->n_lines=-1;
	hl->lines=NULL;
}

void hard_limit_free(struct simulation *sim,struct hard_limit *hl)
{
	free(hl->lines);
	hl->lines=NULL;
	hl->n_lines=-1;

}

void hard_limit_load(struct simulation *sim,struct hard_limit *hl,struct json_obj *json_hl)
{
	int i;
	char seg_name[100];
	struct json_obj *json_hl_seg;
	hl->n_lines=0;

	json_get_int(sim,json_hl, &(hl->n_lines),"segments");

	hl->lines=malloc(hl->n_lines*sizeof(struct hard_limit_line));
	for (i=0;i<hl->n_lines;i++)
	{
		sprintf(seg_name,"segment%d",i);
		json_hl_seg=json_obj_find(json_hl, seg_name);

		json_get_string(sim,json_hl_seg, hl->lines[i].token,"token");
		json_get_long_double(sim,json_hl_seg, &(hl->lines[i].min),"min");
		json_get_long_double(sim,json_hl_seg, &(hl->lines[i].max),"max");
	}

}

void hard_limit_do(struct simulation *sim,char *token,long double *value)
{
int i;
long double ret= *value;
struct hard_limit *hl=&(sim->hl);
	//printf("== %d %p\n",	sim->hl.n_lines,&(sim->hl.n_lines));

	for (i=0;i<hl->n_lines;i++)
	{
		//printf("found\n %d %s",i,hl->lines[i].token);

		if (strcmp(hl->lines[i].token,token)==0)
		{
			if (ret>hl->lines[i].max)
			{
				ret=hl->lines[i].max;
			}

			if (ret<hl->lines[i].min)
			{
				ret=hl->lines[i].min;
			}
		}
	}
*value=ret;
}

