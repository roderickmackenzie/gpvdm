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

/** @file josn_dump.c
	@brief Json file decoder
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

void remove_comma(struct json_string *buf)
{
	int i;
	int len=buf->pos;
	for (i=len-1;i>0;i--)
	{
		if (buf->data[i]==',')
		{
			buf->data[i]=0;
			buf->pos=i;
			return;
		} 
	}

}

int json_save(struct simulation *sim,struct json *j)
{
	int ret;
	struct json_string s;
	json_string_init(&s);

	if (j->compact==TRUE)
	{
		s.compact=TRUE;
		json_dump_to_string(&s,&(j->obj),-1);
	}else
	{
		json_dump_to_string(&s,&(j->obj),0);
	}

	ret=zip_write_buffer(sim,j->file_path,s.data, s.pos);
	json_string_free(&s);
	return ret;
}

int json_save_as(struct simulation *sim,char *file_name,struct json *j)
{
	int ret;
	struct json_string s;
	json_string_init(&s);
	if (j->compact==TRUE)
	{
		s.compact=TRUE;
		json_dump_to_string(&s,&(j->obj),-1);
	}else
	{
		json_dump_to_string(&s,&(j->obj),0);
	}
	ret=zip_write_buffer(sim,file_name,s.data, s.pos);
	json_string_free(&s);

	return ret;
}

int json_obj_save_as(struct simulation *sim,char *file_name,struct json_obj *j)
{
	int ret;
	struct json_string s;
	json_string_init(&s);
	json_dump_to_string(&s,j,0);
	ret=zip_write_buffer(sim,file_name,s.data, s.pos);
	json_string_free(&s);
	return ret;
}



void json_dump_to_string(struct json_string *buf,struct json_obj *obj,int level)
{
	json_cat_str(buf,"{\n");
	json_dump(obj,buf,level);
	remove_comma(buf);
	json_cat_str(buf,"\n}\n");
}

void tabs(struct json_string *buf,int number)
{
	if (buf->compact==TRUE)
	{
		return;
	}

	int i;
	char temp[200];
	if (number!=-1)
	{
		for (i=0;i<number;i++)
		{
			temp[i]='\t';
		}
		temp[number]=0;
		json_cat_str(buf,temp);
	}
}

void json_dump_obj(struct json_obj *obj)
{
	struct json_string s;
	json_string_init(&s);
	json_dump_to_string(&s,obj,0);
	printf("%s\n",s.data);
	json_string_free(&s);
}


void json_dump(struct json_obj *obj,struct json_string *buf, int level)		//delete buf_pos later
{
	int i;
	char temp[400];
	if (level!=-1)
	{
		level++;
	}
	struct json_obj *objs;
	struct json_obj *next_obj;
	objs=(struct json_obj* )obj->objs;
	for (i=0;i<obj->len;i++)
	{
		next_obj=&(objs[i]);

		if (next_obj->node==TRUE)
		{
			tabs(buf,level);
			sprintf(temp,"\"%s\": {\n",next_obj->name);
			json_cat_str(buf,temp);
			json_dump(next_obj,buf,level);
			remove_comma(buf);
			if (buf->compact==FALSE)
			{
				json_cat_str(buf,"\n");
				tabs(buf,level);
				json_cat_str(buf,"\t},\n");
			}else
			{
				json_cat_str(buf,"},");
			}
			//printf("'%s' %d\n",buf,*buf_pos);
			//getchar();
		}else
		{
			tabs(buf,level);
			char quote[10];
			if (str_isnumber(next_obj->data))
			{
				strcpy(quote,"");
			}else
			{
				strcpy(quote,"\"");
			}
			sprintf(temp,"\"%s\":%s%s%s,\n",next_obj->name,quote,next_obj->data,quote);
			json_cat_str(buf,temp);
		}
	}
	if (level!=-1)
	{
		level--;
	}
}

