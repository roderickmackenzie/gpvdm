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

/** @file josn_cpy.c
	@brief Json copy objects
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include "inp.h"
#include "util.h"
#include "code_ctrl.h"
#include "gpvdm_const.h"
#include <log.h>
#include <cal_path.h>
#include "lock.h"
#include <json.h>

void json_cpy(struct simulation *sim,struct json *out,struct json *in)
{
	out->raw_data_len=in->raw_data_len;
	if (in->raw_data_len>0)
	{
		out->raw_data=(char*)malloc(sizeof(char)*out->raw_data_len);
	}

	out->pos=in->pos;
	out->level=in->level;
	strcpy(out->path,in->path);
	strcpy(out->file_path,in->file_path);
	json_obj_cpy(sim,&(out->obj),&(in->obj));
}

void json_obj_cpy_data(struct simulation *sim,struct json_obj *out,struct json_obj *in)
{
	out->len=in->len;
	out->max_len=in->max_len;
	out->type=in->type;
	strcpy(out->name,in->name);
	//printf("%s\n",out->name);
	if (in->data!=NULL)
	{
		out->data_len=in->data_len;
		out->data=malloc((in->data_len)*sizeof(char));
		strcpy(out->data,in->data);
	}

	out->node=in->node;
	if (out->max_len>0)
	{
		out->objs=(struct json_obj*)malloc(sizeof(struct json_obj)*out->max_len);
	}
}

void json_obj_cpy(struct simulation *sim,struct json_obj *out,struct json_obj *in)
{

	int i;
	json_obj_cpy_data(sim,out,in);

	struct json_obj *objs_in;
	struct json_obj *objs_out;

	struct json_obj *obj_in;
	struct json_obj *obj_out;

	objs_in=(struct json_obj* )in->objs;
	objs_out=(struct json_obj* )out->objs;

	for (i=0;i<in->len;i++)
	{
		obj_in=&(objs_in[i]);
		obj_out=&(objs_out[i]);

		json_obj_cpy(sim,obj_out,obj_in);

	}

}



