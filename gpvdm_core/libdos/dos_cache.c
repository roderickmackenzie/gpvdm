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

/** @file dos.c
	@brief Reads in the DoS files but does not generate them, also deals with interpolation.
*/

#include <enabled_libs.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

#include "code_ctrl.h"
#include "server.h"
#include "sim.h"
#include "dump.h"
#include "lang.h"
#include "log.h"
#include "cal_path.h"
#include "util.h"

void dos_cache_setup(struct simulation *sim,struct dos_cache *obj,struct json_obj *all_json)
{
	int l;
	int ns;
	int layers;
	int nshape;
	char temp[200];
	int enabled;
	int use_disk;
	struct json_obj *json_server;
	struct json_obj *json_layer;
	struct json_obj *json_shape_dos;
	struct json_obj *json_sub_shape;
	struct json_obj *json_epi;

	if (obj->setup==TRUE)
	{
		return;
	}


	obj->setup=TRUE;

	json_server=json_obj_find(all_json, "server");
	json_get_english(sim,json_server, &(use_disk),"server_use_dos_disk_cache");
	if (use_disk==TRUE)
	{
		obj->enabled=FALSE;
		return;
	}

	obj->enabled=TRUE;


	obj->len_max=100;
	obj->len=0;
	obj->objs=(struct dos_cache_obj*)malloc(obj->len_max*sizeof(struct dos_cache_obj));

	json_epi=json_obj_find(all_json, "epitaxy");

	json_get_int(sim,json_epi, &(layers),"layers");


	for (l=0;l<layers;l++)
	{
		sprintf(temp,"layer%d",l);
		json_layer=json_obj_find(json_epi, temp);
		json_shape_dos=json_obj_find(json_layer, "shape_dos");
		json_get_english(sim,json_shape_dos, &(enabled),"enabled");

		if (enabled==TRUE)
		{
			obj->objs[obj->len].len=0;
			json_get_string(sim,json_shape_dos, obj->objs[obj->len].id,"id");
			strcpy(obj->objs[obj->len].md5,"null");
			obj->objs[obj->len].dosp=NULL;
			obj->objs[obj->len].dosn=NULL;
			obj->len++;
		}

		json_get_int(sim,json_layer, &(nshape),"layer_shapes");
		for (ns=0;ns<nshape;ns++)
		{
			sprintf(temp,"shape%d",ns);
			json_sub_shape=json_obj_find(json_layer, temp);
			json_shape_dos=json_obj_find(json_sub_shape, "shape_dos");
			json_get_english(sim,json_shape_dos, &(enabled),"enabled");
			if (enabled==TRUE)
			{
				obj->objs[obj->len].len=0;
				json_get_string(sim,json_shape_dos, obj->objs[obj->len].id,"id");
				strcpy(obj->objs[obj->len].md5,"null");
				obj->objs[obj->len].dosp=NULL;
				obj->objs[obj->len].dosn=NULL;
				obj->len++;
			}
		}
	}


}

void dos_cache_dump(struct dos_cache *obj)
{
	int o;
	for (o=0;o<obj->len;o++)
	{
		printf("%s %s %d\n",obj->objs[o].id,obj->objs[o].md5,obj->objs[o].len);
	}

}

void dos_cache_init(struct dos_cache *cache)
{
	cache->enabled=FALSE;
	cache->len=0;
	cache->objs=NULL;
	cache->setup=FALSE;
}

void dos_cache_obj_free(struct dos_cache_obj *obj)
{
	if (obj->dosn!=NULL)
	{
		free(obj->dosn);
	}

	if (obj->dosp!=NULL)
	{
		free(obj->dosp);
	}

}

void dos_cache_free(struct dos_cache *cache)
{
	int i;
	if (cache->enabled==FALSE)
	{
		return;
	}

	for (i=0;i<cache->len;i++)
	{
		dos_cache_obj_free(&(cache->objs[i]));
	}

	free(cache->objs);
	dos_cache_init(cache);
}

