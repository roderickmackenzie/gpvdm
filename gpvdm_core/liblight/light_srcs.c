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
	//for EQE
	srcs->use_flat_sepctrum=FALSE;			
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
	//eqe
	out->use_flat_sepctrum=in->use_flat_sepctrum;			

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

			srcs->light_sources[i].use_flat_sepctrum=srcs->use_flat_sepctrum;

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
