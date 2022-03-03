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

/** @file josn.c
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
#include <ctype.h>

void json_obj_init(struct json_obj *obj)
{
	obj->type=0;
	strcpy(obj->name,"");
	obj->len=0;
	obj->max_len=0;
	obj->objs=NULL;
	obj->data=NULL;
	obj->data_len=0;
	obj->node=FALSE;
}

void json_obj_realloc(struct json_obj *obj)
{
	if (obj->len>obj->max_len-5)
	{
		//printf("realloc\n");
		obj->max_len+=100;
		obj->objs=(struct json_obj*)realloc(obj->objs,sizeof(struct json_obj)*obj->max_len);
	}

}

struct json_obj * json_obj_add(struct json_obj *obj,char *name,char *data)
{
	struct json_obj *objs;
	struct json_obj *obj_next;

	json_obj_realloc(obj);

	objs=(struct json_obj* )obj->objs;
	obj_next=&(objs[obj->len]);
	json_obj_init(obj_next);

	strcpy(obj_next->name,name);
	if (obj_next->data!=NULL)
	{
		printf("json memory error\n");
		getchar();
	}
	obj_next->data_len=strlen(data)+10;
	obj_next->data=malloc((obj_next->data_len)*sizeof(char));
	strcpy(obj_next->data,data);

	obj->len++;

	return obj_next;
}


void remove_space_after(char *in)
{
	int i;
	for (i=strlen(in)-1;i>0;i--)
	{
		if ((in[i]==' ')||(in[i]=='\t')||(in[i]=='\n'))
		{
			in[i]=0;
		}else
		{
			break;
		}
	}	
}

void gobble(struct json *j)
{
	while(j->pos<j->raw_data_len)
	{
		if (isspace(j->raw_data[j->pos])==0)	//If it is not white space
		{
			return;
		}

		j->pos++;

	}

	j->pos=j->raw_data_len-1;
	
}

void get_name(char *out,struct json *j)
{
	int i=0;
	strcpy(out,"");
	if (j->raw_data[j->pos]=='\"')
	{
		j->pos++;
	}else
	{
		printf("error no \" found '%c'\n",j->raw_data[j->pos]);
	}

	while(j->pos<j->raw_data_len)
	{
	//printf("%c\n",j->raw_data[j->pos]);
	//getchar();
		if (j->raw_data[j->pos]=='\"')
		{
			j->pos++;
			gobble(j);
			//getchar();
			if (j->raw_data[j->pos]==':')
			{
				j->pos++;
				return;
			}else
			{
				printf("error '%s'\n",out);
				exit(0);
			}
		}

		out[i]=j->raw_data[j->pos];
		out[i+1]=0;
		i++;
		j->pos++;

	}

	return;
}

void get_value(char *out,struct json *j)
{
	int i=0;
	int add=TRUE;
	int comma_open=FALSE;
	int quotes_found=0;
	strcpy(out,"");

	while(j->pos<j->raw_data_len)
	{
		add=TRUE;

		if (j->raw_data[j->pos]=='\"')
		{
			if (comma_open==TRUE)
			{
				comma_open=FALSE;
			}
			quotes_found++;
		}

		//printf("%d %c %d\n",i,j->raw_data[j->pos],comma_open);
		if (comma_open==FALSE)
		{
			if ((j->raw_data[j->pos]==',')||(j->raw_data[j->pos]=='}'))
			{
				remove_space_after(out);
				//printf("'%s'\n",out);
				int len=strlen(out);
				if (len>0)
				{
					if (out[len-1]=='\"')
					{
						out[len-1]=0;
					}
				}
				return;
			}
		}

		if ((quotes_found==1)&&(j->raw_data[j->pos]=='\"'))
		{
			add=FALSE;
			comma_open=TRUE;
		}

		if (add==TRUE)
		{	
			out[i]=j->raw_data[j->pos];
			out[i+1]=0;
			i++;
		}

		j->pos++;

	}

	return;
}


int json_decode(struct json *j,struct json_obj *obj)
{
	int i;
	char token[100];
	char out[PATH_MAX];
	struct json_obj *next_obj;
	gobble(j);

	if (j->raw_data[j->pos]=='{')
	{
		j->pos++;
	}else
	{
		j->level++;
	}
	for (i=0;i<100;i++)
	{
		gobble(j);
		get_name(token,j);

		gobble(j);
		if (j->raw_data[j->pos]=='{')
		{

			if (strlen(j->path)!=0)
			{
				strcat(j->path,".");
			}
			strcat(j->path,token);

			j->pos++;
			gobble(j);
			//printf("%s %s\n",token,j->path);
			//getchar();
			next_obj=json_obj_add(obj,token,"");
			//printf("I am about to decode another\n");
			//getchar();
			next_obj->node=TRUE;
			json_decode(j,next_obj);
			//get_name(token,j);
			strcpy(out,"none");
		}else
		{
			//printf("t='%s'\n",token);
			get_value(out,j);
			//printf("v='%s'\n",out);
			//getchar();
			json_obj_add(obj,token,out);
		}

		if (j->level==-1)
		{
			//printf("level=0 exit!!!!!!!!!\n");
			return 0;
		}

		//printf("%d: path=%s%s=%s\n",j->level,j->path,token,out);
		
		gobble(j);
		if (j->raw_data[j->pos]==',')
		{
			j->pos++;
		}

		if (j->raw_data[j->pos]=='}')
		{

			j->pos++;
			if (j->raw_data[j->pos]==',')
			{
				j->pos++;
			}
			j->level--;
			int ff;
			int removed=0;
			for (ff=strlen(j->path)-1;ff>0;ff--)
			{
				if (j->path[ff]=='.')
				{
					j->path[ff]=0;
					removed=1;
					break;
				}
			}

			if (removed==0)
			{
				strcpy(j->path,"");
			}
			//printf("exit here!!!!!!!!!\n");

			return 0;
		}


	}
	return 0;

}

int json_load_from_path(struct simulation *sim,struct json *j,char *path,char *file_name)
{
	char full_file_name[PATH_MAX];
	join_path(2,full_file_name,path,file_name);
	return json_load(sim,j,full_file_name);
}

void json_from_buffer(struct simulation *sim,struct json *j,char *buf,int len)
{
	j->raw_data_len = len;
	j->raw_data = malloc(((j->raw_data_len) + 1)*sizeof(char));
	memcpy(j->raw_data, buf, j->raw_data_len);
	j->raw_data[j->raw_data_len]=0;

	json_decode(j,&(j->obj));
}

int json_load(struct simulation *sim,struct json *j,char *full_file_name)
{
	strcpy(j->file_path,full_file_name);
	FILE *f = fopen(full_file_name, "rb");
	if (f!=NULL)
	{
		sim->files_read++;
		int err=0;
		fseek(f, 0, SEEK_END);
		j->raw_data_len = ftell(f);
		fseek(f, 0, SEEK_SET);

		sim->bytes_read+=j->raw_data_len;

		log_write_file_access(sim,full_file_name,'r');

		j->raw_data = malloc(((j->raw_data_len) + 1)*sizeof(char));
		//memset(*buf, 0, ((*len) + 1)*sizeof(char));
		if (fread(j->raw_data, j->raw_data_len, 1, f)==0)
		{
			err= -1;
		}


		j->raw_data[j->raw_data_len]=0;
		fclose(f);
		if (err==0)
		{
			json_decode(j,&(j->obj));
			//json_dump_obj(&(j->obj));
			//getchar();
		}
		return err;

	}else
	{
		char zip_path[1000];
		char file_path[1000];
		char file_name[1000];

		get_dir_name_from_path(file_path,full_file_name);
		get_file_name_from_path(file_name,full_file_name);

		join_path(2,zip_path,file_path,"sim.gpvdm");

		int err = 0;
		struct zip *z = zip_open(zip_path, 0, &err);

		if (z!=NULL)
		{
			//Search for the file of given name
			struct zip_stat st;
			zip_stat_init(&st);
			int ret=zip_stat(z, file_name, 0, &st);

			if (ret==0)
			{
				//Alloc memory for its uncompressed contents
				j->raw_data_len=st.size*sizeof(char);
				j->raw_data = (char *)malloc((j->raw_data_len+1)*sizeof(char));
				sim->bytes_read+=j->raw_data_len;

				//Read the compressed file
				struct zip_file *f = zip_fopen(z, file_name, 0);
				sim->files_read++;
				if (f==NULL)
				{
					free(j->raw_data);
					zip_close(z);
					return -1;
				}

				ret=zip_fread(f, j->raw_data, st.size);
				if (ret==-1)
				{
					free(j->raw_data);
					zip_fclose(f);
					zip_close(z);
					return -1;
				}

				zip_fclose(f);
				j->raw_data[j->raw_data_len]=0;
			}else
			{

				zip_close(z);
			 	return -1;
			}

			zip_close(z);

			json_decode(j,&(j->obj));

			return 0;
		}else
		{
			return -1;
		}

	}

}
