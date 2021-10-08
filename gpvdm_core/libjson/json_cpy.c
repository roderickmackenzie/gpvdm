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



