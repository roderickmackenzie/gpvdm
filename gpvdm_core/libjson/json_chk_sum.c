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

/** @file josn_chk_sum.c
	@brief Json checksum
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

void json_chk_sum(struct simulation *sim,struct md5 *sum,struct json_obj *json_in)
{

	int i;

	struct json_obj *obj;
	struct json_obj *objs;

	objs=(struct json_obj* )json_in->objs;

	for (i=0;i<json_in->len;i++)
	{
		obj=&(objs[i]);
		md5_update(sum,obj->name,strlen(obj->name));

		if (obj->node==TRUE)
		{
			json_chk_sum(sim,sum,obj);
		}else
		{
			if (strcmp_begin(obj->name,"mue_")!=0)
			{
				if (strcmp_begin(obj->name,"muh_")!=0)
				{
					md5_update(sum,obj->data,strlen(obj->data));
				}
			}
		}


	}

}

int json_checksum_check(struct simulation *sim,char *out_check_sum, char *chk_file,struct json_obj *json_in)
{
	FILE *file;
	char newcheck[100];
	char fromfile[100];
	struct md5 sum;
	md5_init(&sum);
	json_chk_sum(sim,&sum,json_in);
	md5_to_str(newcheck,&sum);

	if (out_check_sum!=NULL)
	{
		strcpy(out_check_sum,newcheck);
	}

	file=fopen(chk_file,"r");

	if (!file)
	{
		return FALSE;
	}

	fscanf(file,"%s\n",fromfile);
	fclose(file);
	//printf("%s %s\n",newcheck,fromfile);
	//getchar();

	if (strcmp(newcheck,fromfile)==0)
	{
		return TRUE;
	}else
	{
		return FALSE;
	}

	return 0;

}




