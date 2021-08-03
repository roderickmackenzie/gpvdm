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


/** @file device_heat.c
	@brief Malloc for the device structure.
*/

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <device.h>
#include <string.h>
#include <dump.h>
#include <mesh.h>
#include <ray_fun.h>
#include <newton_tricks.h>
#include <memory.h>
#include <circuit.h>
#include <shape.h>
#include <lang.h>
#include <util.h>
#include <heat_fun.h>
#include <gpvdm_const.h>
#include <light_srcs.h>
#include <light_fun.h>

static int unused __attribute__((unused));
static char* unused_pchar __attribute__((unused));

void light_srcs_init(struct simulation *sim,struct light_sources *srcs)
{
	srcs->nlight_sources=-1;
	srcs->light_sources=NULL;
	srcs->lstart=-1;
	srcs->lstop=-1;
	srcs->llen=-1;
}

void light_srcs_free(struct simulation *sim,struct light_sources *srcs)
{
	int i;

	for (i=0;i<srcs->nlight_sources;i++)
	{
		light_src_free(sim, &(srcs->light_sources[i]));
	}

	free(srcs->light_sources);
	light_srcs_init(sim,srcs);

}

void light_srcs_dump(struct simulation *sim,char *path,struct light_sources *srcs)
{
	int i;
	char out_dir[PATH_MAX];

	join_path(2,out_dir,path,"optical_output");
	gpvdm_mkdir(out_dir);

	for (i=0;i<srcs->nlight_sources;i++)
	{
		light_src_dump(sim,out_dir,&(srcs->light_sources[i]));
	}

}
		

void light_srcs_cpy(struct simulation *sim,struct light_sources *out,struct light_sources *in)
{
	int i;
	if (in->nlight_sources>0)
	{
		out->light_sources=malloc(sizeof(struct light_src)*in->nlight_sources);
		out->nlight_sources=in->nlight_sources;
		for (i=0;i<in->nlight_sources;i++)
		{
			light_src_cpy(sim,&(out->light_sources[i]), &(in->light_sources[i]));
		}
	}

	out->lstart=in->lstart;
	out->lstop=in->lstop;
	out->llen=in->llen;

}

void light_srcs_load(struct simulation *sim,struct light_sources *srcs,struct json_obj *json_light_sources)
{
	int i;
	char temp[200];

	struct json_obj *json_lights;
	struct json_obj *json_config;
	struct json_obj *json_seg;
	struct json_obj *json_virtual_spectra;

	json_config=json_obj_find(json_light_sources, "config");

	if (json_config==NULL)
	{
		ewe(sim,"Object lights not found\n");
	}

	json_get_int(sim, json_config, &srcs->llen,"lpoints");
	json_lights=json_obj_find(json_light_sources, "lights");

	if (json_lights==NULL)
	{
		ewe(sim,"Object lights not found\n");
	}

	json_get_int(sim, json_lights, &srcs->nlight_sources,"segments");
	if (srcs->nlight_sources>0)
	{
		srcs->light_sources=malloc(sizeof(struct light_src)*srcs->nlight_sources);
		for (i=0;i<srcs->nlight_sources;i++)
		{
			sprintf(temp,"segment%d",i);
			json_seg=json_obj_find(json_lights, temp);
			json_virtual_spectra=json_obj_find(json_seg, "virtual_spectra");
			if (json_virtual_spectra==NULL)
			{
				ewe(sim,"Object virtual_spectra not found\n");
			}
			light_src_init(sim,&(srcs->light_sources[i]));
			light_src_load(sim,&(srcs->light_sources[i]), json_seg);

		}

		light_srcs_cal_lambda_min_max(sim,srcs);
		for (i=0;i<srcs->nlight_sources;i++)
		{
			light_src_build_spectra_tot(sim,&(srcs->light_sources[i]), srcs->lstart, srcs->lstop, srcs->llen);
		}
	}

	
	
}

void light_srcs_cal_lambda_min_max(struct simulation *sim,struct light_sources *srcs)
{
	int i;
	srcs->lstop=-1.0;
	srcs->lstart=1e6;

	if (srcs->nlight_sources>0)
	{
		for (i=0;i<srcs->nlight_sources;i++)
		{
			if (srcs->light_sources[i].lstart<srcs->lstart)
			{
				srcs->lstart=srcs->light_sources[i].lstart;
			}

			if (srcs->light_sources[i].lstop>srcs->lstop)
			{
				srcs->lstop=srcs->light_sources[i].lstop;
			}
		}
	}

}
