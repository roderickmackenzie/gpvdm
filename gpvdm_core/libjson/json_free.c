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

/** @file josn_free.c
	@brief Json free objects
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

void json_free(struct json *j)
{
	json_obj_all_free(j,&(j->obj));
	free(j->raw_data);
	json_init(j);

}

void json_objs_free(struct json_obj *obj)
{
	int i;
	struct json_obj *objs;
	struct json_obj *next_obj;
	objs=(struct json_obj* )obj->objs;
	for (i=0;i<obj->len;i++)
	{
		next_obj=&(objs[i]);
		if (next_obj->data!=NULL)
		{
			free(next_obj->data);
			next_obj->data=NULL;
		}
	}

	if (obj->objs!=NULL)
	{
		free(obj->objs);
		obj->objs=NULL;
	}
	obj->len=0;
	obj->max_len=0;

}

void json_obj_all_free(struct json *j,struct json_obj *obj)
{
	int i;

	struct json_obj *objs;
	struct json_obj *next_obj;
	objs=(struct json_obj* )obj->objs;
	for (i=0;i<obj->len;i++)
	{
		next_obj=&(objs[i]);

		if (next_obj->node==TRUE)
		{
			json_obj_all_free(j,next_obj);

		}
	}

	json_objs_free(obj);

}



