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

