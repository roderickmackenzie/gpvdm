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

/** @file inp.c
	@brief Input file interface, files can be in .gpvdm files or stand alone files.
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
#include <list.h>

int inp_test_end_of_data(char *line)
{
if ((line==NULL)||(strcmp(line,"#end")==0)||(strcmp(line,"#ver")==0))
{
	return 0;
}

return -1;
}
int search_for_token(struct simulation *sim,char *ret,char *dir_name,char* token,char *search_value)
{
int i=0;
int found=FALSE;
int is_sim_file=FALSE;
char found_value[256];
struct inp_file inp;
struct list a;
inp_listdir(sim,dir_name,&a);


	for (i=0;i<a.len;i++)
	{
		if ((strcmp(a.names[i],".")!=0)&&(strcmp(a.names[i],"..")!=0))
		{
			if (strcmp_end(a.names[i],".inp")==0)
			{
				inp_init(sim,&inp);
				inp_load_from_path(sim,&inp,dir_name,a.names[i]);
				is_sim_file=inp_search(sim,found_value,&inp,token);
				inp_free(sim,&inp);

				if (is_sim_file==0)
				{
						if (strcmp(found_value,search_value)==0)
						{
							strcpy(ret,a.names[i]);
							found=TRUE;
							break;
						}
				}


			}
		}
	}

list_free(&a);

if (found==TRUE)
{
	return 0;
}else
{
	strcpy(ret,"");
	return -1;
}

return -1;
}

int guess_whole_sim_name(struct simulation *sim,char *ret,char *dir_name,char* search_name)
{
int i=0;
int found=FALSE;
int is_sim_file=FALSE;
char sim_name[256];
char name[200];
struct inp_file inp;
struct list a;
inp_listdir(sim,dir_name,&a);


	for (i=0;i<a.len;i++)
	{
		if ((strcmp(a.names[i],".")!=0)&&(strcmp(a.names[i],"..")!=0))
		{
			if (strcmp_end(a.names[i],".inp")==0)
			{
				inp_init(sim,&inp);
				inp_load_from_path(sim,&inp,dir_name,a.names[i]);
				is_sim_file=inp_search(sim,sim_name,&inp,"#sim_menu_name");
				inp_free(sim,&inp);

				if (is_sim_file==0)
				{
					if (strextract_name(name,sim_name)!= -1)
					{
						if (strcmp(name,search_name)==0)
						{
							strcpy(ret,sim_name);
							found=TRUE;
							break;
						}
					}
				}
				//if (strcmp(sim_name,search_name)==0)
				//{
				//	strcpy(ret,a.names[i]);
				//	found=TRUE;
				//	break;
				//}

			}
		}
	}

list_free(&a);

if (found==TRUE)
{
	return 0;
}else
{
	strcpy(ret,"");
	return -1;
}

return -1;
}

int find_config_file(struct simulation *sim,char *ret,char *dir_name,char* search_name,char *start_of_name)
{
int i=0;
int found=FALSE;
char sim_name[256];
struct inp_file inp;
inp_init(sim,&inp);

struct list a;
inp_listdir(sim,dir_name,&a);


	for (i=0;i<a.len;i++)
	{
		if ((strcmp(a.names[i],".")!=0)&&(strcmp(a.names[i],"..")!=0))
		{
			//printf(">>%s %d\n",a.names[i],strlen(a.names[i]));
			if ((cmpstr_min(a.names[i],start_of_name)==0)&&(strcmp_end(a.names[i],".inp")==0))
			{

				if (inp_load_from_path(sim,&inp,dir_name,a.names[i])==0)
				{
					//printf(">>loaded\n");

					strcmp(sim_name,"");

					if (inp_is_token(sim,&inp,"#sim_menu_name")==0)
					{
						inp_search_string(sim,&inp,sim_name,"#sim_menu_name");
					}

					inp_free(sim,&inp);
					//printf(">>freed\n");

					if (strcmp(sim_name,search_name)==0)
					{
						strcpy(ret,a.names[i]);
						found=TRUE;
						break;
					}


				}

			}
		}
	}

list_free(&a);

if (found==TRUE)
{
	return 0;
}else
{
	strcpy(ret,"");
	return -1;
}

return -1;
}


void inp_listdir(struct simulation *sim, char *dir_name,struct list *out)
{
char sim_file[PATH_MAX];
int mylen=0;
int i=0;
int err = 0;
char temp[200];

join_path(2,sim_file,dir_name,"sim.gpvdm");

out->names=(char**)malloc(sizeof(char*)*2000);
out->len=0;

struct zip *z = zip_open(sim_file, 0, &err);

if (z!=NULL)
{
	int files=zip_get_num_files(z);
	for (i=0;i<files;i++)
	{
		strcpy(temp,zip_get_name(z, i, ZIP_FL_UNCHANGED));
		if (list_cmp(out,temp)!=0)
		{
			mylen=strlen(temp);
			out->names[out->len]=(char*)malloc(sizeof(char)*(mylen+1));
			strcpy(out->names[out->len],temp);
			out->len++;
		}

	}

	zip_close(z);

}
struct dirent *next_file;
DIR *theFolder;

theFolder = opendir(dir_name);
if (theFolder!=NULL)
{
	while((next_file=readdir(theFolder))!=NULL)
	{
		mylen=strlen(next_file->d_name);
		out->names[out->len]=(char*)malloc(sizeof(char)*(mylen+1));
		strcpy(out->names[out->len],next_file->d_name);
		out->len++;
	}

closedir (theFolder);

}

}


int zip_is_in_archive(char *full_file_name)
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
		zip_close(z);

		if (ret!=0)
		{
		 	return -1;
		}

		return 0;
	}else
	{
		return -1;
	}
}

int inp_isfile(struct simulation *sim,char *full_file_name)
{
FILE *f = fopen(full_file_name, "rb");
if (f!=NULL)
{
	sim->files_read++;
	fclose(f);
	return 0;
}else
{
	return zip_is_in_archive(full_file_name);
}
//#endif

}

int inp_search_pos(struct simulation *sim,struct inp_file *in,char *token)
{
	int pos=0;
	inp_reset_read(sim,in);
	char * line=NULL;
	do
	{
		line  = inp_get_string(sim,in);
		if (line==NULL)
		{
			break;
		}

		if (strcmp(line,token)==0)
		{
			return pos;
		}

		pos++;

	}while(1);

return -1;
}

void inp_reset_read(struct simulation *sim,struct inp_file *in)
{
in->pos=0;
}

char* inp_get_string(struct simulation *sim,struct inp_file *in)
{
int i;
static char ret[4000];
memset(ret, 0, 400);
int ii=0;
if (in->pos>=in->fsize)
{
	return NULL;
}

get_line(ret,in->data,in->fsize,&in->pos);

return ret;
}




int inp_read_buffer(struct simulation *sim,char **buf, long *len,char *full_file_name)
{
FILE *f = fopen(full_file_name, "rb");
if (f!=NULL)
{
	sim->files_read++;
	int err=0;
	fseek(f, 0, SEEK_END);
	*len = ftell(f);
	fseek(f, 0, SEEK_SET);

	sim->bytes_read+=*len;

	log_write_file_access(sim,full_file_name,'r');

	*buf = malloc(((*len) + 1)*sizeof(char));
	//memset(*buf, 0, ((*len) + 1)*sizeof(char));
	if (fread(*buf, *len, 1, f)==0)
	{
		err= -1;
	}


	(*buf)[*len]=0;
	fclose(f);
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
			*len=st.size*sizeof(char);
			*buf = (char *)malloc((*len+1)*sizeof(char));
			sim->bytes_read+=*len;

			//Read the compressed file
			struct zip_file *f = zip_fopen(z, file_name, 0);
			sim->files_read++;
			if (f==NULL)
			{
				free(buf);
				zip_close(z);
				return -1;
			}

			ret=zip_fread(f, *buf, st.size);
			if (ret==-1)
			{
				free(buf);
				zip_fclose(f);
				zip_close(z);
				return -1;
			}

			zip_fclose(f);
			(*buf)[*len]=0;
		}else
		{

			zip_close(z);
		 	return -1;
		}

		zip_close(z);

		return 0;
	}else
	{
		return -1;
	}

}

}

void inp_init(struct simulation *sim,struct inp_file *in)
{
strcpy(in->full_name,"");
in->data=NULL;
in->fsize=0;
in->pos=0;
in->edited=FALSE;
}

int inp_load_from_path(struct simulation *sim,struct inp_file *in,char *path,char *file)
{
int ret=0;
char full_path[1000];
join_path(2,full_path,path,file);
ret=inp_load(sim,in,full_path);
return ret;
}

void inp_load_from_buffer(struct simulation *sim,struct inp_file *in,char *file,char *buffer,int len)
{
	in->pos=0;


	if (in->data!=NULL)
	{
		inp_free(sim,in);
	}

	strcpy(in->full_name,file);

	in->fsize=len;
	in->data=malloc((len+1)*sizeof(char));
	memcpy(in->data, buffer, len*sizeof(char));
	in->data[len]=0;
	in->edited=FALSE;

}

int inp_load(struct simulation *sim,struct inp_file *in,char *file)
{
int ret=0;
in->pos=0;
if (strcmp(in->full_name,file)!=0)
{

	if (in->data!=NULL)
	{
		inp_free(sim,in);
	}

	strcpy(in->full_name,file);
	if (inp_read_buffer(sim,&(in->data),&(in->fsize),file)!=0)
	{
		ret= -1;
	}

	in->edited=FALSE;
}

return ret;
}

void inp_replace_double_offset_col(struct simulation *sim,struct inp_file *in,char *token, double value,int offset,int col)
{
	char temp[200];
	char new[200];
	inp_search(sim,temp,in,token);
	replace_number_in_string(new, temp, value, col);
	inp_replace_offset(sim,in,token,new,offset);
}

void inp_replace_double_offset(struct simulation *sim,struct inp_file *in,char *token, double value,int offset)
{
	char temp[100];
	sprintf(temp,"%le",value);
	inp_replace_offset(sim,in,token,temp,offset);
}

void inp_replace_double(struct simulation *sim,struct inp_file *in,char *token, double value)
{
	char temp[100];
	sprintf(temp,"%le",value);
	inp_replace(sim,in,token,temp);
}

void inp_replace(struct simulation *sim,struct inp_file *in,char *token, char *text)
{
inp_replace_offset(sim,in,token, text,0);
}


void inp_replace_offset(struct simulation *sim,struct inp_file *in,char *token, char *text,int offset)
{
int i=0;
char *temp = malloc(in->fsize + 100);
memset(temp, 0, in->fsize + 100);
//char *line;
int len=0;
int pos=0;
int ret=0;
char line[in->fsize + 100];

ret=get_line(line,in->data,in->fsize,&pos);

int found=FALSE;

while(ret!=-1)
{
	if (strcmp(line,token)!=0)
	{
		strcat(temp,line);
		strcat(temp,"\n");
	}else
	{
		strcat(temp,line);
		strcat(temp,"\n");

		for (i=0;i<offset;i++)
		{
			ret=get_line(line,in->data,in->fsize,&pos);
			strcat(temp,line);
			strcat(temp,"\n");
		}

		strcat(temp,text);
		strcat(temp,"\n");
		ret=get_line(line,in->data,in->fsize,&pos);
		found=TRUE;
	}
	ret=get_line(line,in->data,in->fsize,&pos);
}


len=strlen(temp);
in->fsize=len;

if (found==TRUE)
{
	in->edited=TRUE;
}

in->data=realloc(in->data,(len+1)*sizeof(char));
memcpy(in->data, temp, (len+1)*sizeof(char));

if (in->data[len]!=0)
{
	ewe(sim,"String not ended\n");
}
free(temp);
}

int zip_write_buffer(struct simulation *sim,char *full_file_name,char *buffer, int len)
{
int	in_zip_file= -1;
int	outside_zip_file= -1;

in_zip_file=zip_is_in_archive(full_file_name);
outside_zip_file=isfile(full_file_name);

//printf_log(sim,"here1:%s %d %d\n",full_file_name,in_zip_file,outside_zip_file);
if ((in_zip_file!=0)||(outside_zip_file==0))
{
	int out_fd = open(full_file_name, O_WRONLY | O_CREAT | O_TRUNC,0644);
	sim->files_written++;
	if (out_fd== -1)
	{
		ewe(sim,"File %s can not be opened\n",full_file_name);
	}

	if (ftruncate(out_fd, 0)== -1)
	{
		ewe(sim,"Error with truncate command\n");
	}

	if (write(out_fd, buffer, len*sizeof(char))== -1)
	{
		ewe(sim,"Error writing data to file.\n");
	}
	close(out_fd);
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
	zip_close(z);

	z = zip_open(zip_path, 0, &err);
	zip_close(z);

	z = zip_open(zip_path, 0, &err);
	zip_close(z);

	z = zip_open(zip_path, 0, &err);
	zip_close(z);

	z = zip_open(zip_path, 0, &err);
	zip_close(z);

	z = zip_open(zip_path, 0, &err);

	if(!z || err != ZIP_ER_OK)
	{
		exit(0);
	}


	//printf_log(sim,"archive:%s\n", zip_path);
	//printf_log(sim,"here:%s\n",file_name);
	if (z!=NULL)
	{
		struct zip_source *s;
		s=zip_source_buffer(z, buffer, len,0);
		//printf_log(sim,"writing:%s\n",file_name);
		err=-1;
		#ifdef ZIP_FL_OVERWRITE				//For cluster compatability
		err=zip_file_add(z, file_name, s, ZIP_FL_OVERWRITE);
		#endif
		if (err==-1)
		{
			ewe(sim,"zip write error");
		}
		zip_close(z);
		//zip_fclose(err);
		/*int index=zip_name_locate(z,  file_name,  0);

		if (index== -1)
		{
			printf_log(sim,"writing:%s %d\n",file_name,index);

			zip_add(z, file_name, s);
			sim->files_written++;

		}else
		{
			printf_log(sim,"replace:%s %d\n",file_name,index);

			err=zip_replace(z, index, s);
			if (err==-1)
			{
				ewe(sim,"zip write error");
			}
			sim->files_written++;
		}*/




	}else
	{
		ewe(sim,"zip write error");
		return -1;
	}

}
return 0;
}

int inp_save(struct simulation *sim,struct inp_file *in)
{
int ret=0;
if (in->edited==TRUE)
{
	ret=zip_write_buffer(sim,in->full_name,in->data, in->fsize);
	in->edited=FALSE;
}

return ret;
}

void inp_free(struct simulation *sim,struct inp_file *in)
{

	inp_save(sim,in);

	free(in->data);
	inp_init(sim,in);
}

void inp_free_no_save(struct simulation *sim,struct inp_file *in)
{

	free(in->data);
	inp_init(sim,in);
}

int inp_search_gdouble(struct simulation *sim,struct inp_file *in,gdouble* out,char* token)
{
char temp[200];
if (inp_search(sim,temp,in,token)==0)
{
	if (str_isnumber(temp)==FALSE)
	{
		printf_log(sim,"warning:  '%s' %s %s is not a number.\n",temp,token,in->full_name);
		getchar();
	}

	sscanf(temp,"%Le",out);
	//printf("oh %Le %s\n",*out,token);
	return 0;
}
return -1;
//ewe(sim,"token %s not found in file %s\n",token,in->full_name);
}

int inp_search_double(struct simulation *sim,struct inp_file *in,double* out,char* token)
{
char temp[200];
if (inp_search(sim,temp,in,token)==0)
{
	if (str_isnumber(temp)==FALSE)
	{
		printf_log(sim,"'%s' is not a number.\n",temp);
	}
	sscanf(temp,"%le",out);
	return 0;
}
return -1;
}

int inp_search_float(struct simulation *sim,struct inp_file *in,float* out,char* token)
{
char temp[200];
if (inp_search(sim,temp,in,token)==0)
{
	if (str_isnumber(temp)==FALSE)
	{
		printf_log(sim,"'%s' is not a number.\n",temp);
	}
	sscanf(temp,"%e",out);
	return 0;
}
return -1;
}


void inp_search_double_offset(struct simulation *sim,struct inp_file *in,double* out,char* token,int offset)
{
char temp[200];
if (inp_search(sim,temp,in,token)==0)
{
	if (str_isnumber(temp)==FALSE)
	{
		printf_log(sim,"'%s' is not a number.\n",temp);
	}

	sscanf(temp,"%le",out);
	return;
}
ewe(sim,"token %s not found in file %s\n",token,in->full_name);
}


void inp_search_int(struct simulation *sim,struct inp_file *in,int* out,char* token)
{
char temp[200];
if (inp_search(sim,temp,in,token)==0)
{
	if (str_isnumber(temp)==FALSE)
	{
		printf_log(sim,"'%s' is not a number.\n",temp);
	}

	sscanf(temp,"%d",out);
	return;
}
ewe(sim,"token %s not found in file %s\n",token,in->full_name);
}

void inp_search_long_long(struct simulation *sim,struct inp_file *in,long long* out,char* token)
{
char temp[200];
if (inp_search(sim,temp,in,token)==0)
{
	if (str_isnumber(temp)==FALSE)
	{
		printf_log(sim,"'%s' is not a number.\n",temp);
	}

	sscanf(temp,"%lld",out);
	return;
}
ewe(sim,"token %s not found in file %s\n",token,in->full_name);
}

int inp_is_token(struct simulation *sim,struct inp_file *in,char* token)
{
	char out[100];
	if (inp_search(sim,out,in,token)==0)
	{
		return 0;
	}
	return -1;
}

void inp_search_string(struct simulation *sim,struct inp_file *in,char* out,char* token)
{
if (inp_search(sim,out,in,token)==0)
{
	return;
}
ewe(sim,"token %s not found in file %s\n",token,in->full_name);
}

void inp_check(struct simulation *sim,struct inp_file *in,double ver)
{
	char line[4000];
	double read_ver=0.0;
	int pos=0;
	int ret=0;
	inp_reset_read(sim,in);

	ret=get_line(line,in->data,in->fsize,&pos);

	while(ret!=-1)
	{
		//printf(">>>>>>>%s\n",line);
		if (strcmp(line,"#ver")==0)
		{
			ret=get_line(line,in->data,in->fsize,&pos);

			//printf(">>>>>>>%s\n",line);
			sscanf(line,"%lf",&(read_ver));

			if (ver!=read_ver)
			{
				ewe(sim,"File compatibility problem %s >%s< >%s< >%lf<\n",in->full_name,in->data,line,ver);

			}
			ret=get_line(line,in->data,in->fsize,&pos);

			if ((ret==-1)||(strcmp(line,"#end")!=0))
			{
				ewe(sim,"#end token missing %s\n",in->full_name);
			}

			inp_reset_read(sim,in);
			return;
		}

		ret=get_line(line,in->data,in->fsize,&pos);
	}

ewe(sim,"Token #ver not found in %s\n",in->full_name);
return;
}

char* inp_search_part(struct simulation *sim,struct inp_file *in,char *token)
{
	inp_reset_read(sim,in);
	char * line = inp_get_string(sim,in);
	while(line!=NULL)
	{

		if (cmpstr_min(line,token)==0)
		{
			return line;
		}

		line  = inp_get_string(sim,in);
	}

return NULL;
}


int inp_search(struct simulation *sim,char* out,struct inp_file *in,char *token)
{
	char line[2000];
	int i;
	int pos=0;
	int ret=0;
	inp_reset_read(sim,in);

	if (in->fsize>0)
	{
		if (in->data[0]!='#')
		{
			return -1;
		}
	}

	ret=get_line(line,in->data,in->fsize,&pos);

	while(ret!=-1)
	{

		if (strcmp(line,token)==0)
		{
			ret=get_line(line,in->data,in->fsize,&pos);
			//printf("1:%s %d\n",line,ret);
			if (ret==-1)
			{
				ewe(sim,"inp_search_offset");
			}
			strcpy(out,line);
			return 0;
		}

		ret=get_line(line,in->data,in->fsize,&pos);
		//printf("2:%d\n",ret);

	}

return -1;
}


int inp_get_array_len(struct simulation *sim,struct inp_file *in,char *token)
{
	int ret=-1;
	inp_reset_read(sim,in);
	char * line = inp_get_string(sim,in);
	while(line!=NULL)
	{
		if (strcmp(line,token)==0)
		{
			ret=0;
			do
			{
				line  = inp_get_string(sim,in);
				if (line==NULL)
				{
					break;
				}

				if (line[0]=='#')
				{
					break;
				}
				ret++;

			}while(1);

			return ret;
		}

		line  = inp_get_string(sim,in);
	}

return ret;
}

int inp_get_array_gdouble(struct simulation *sim,long double * out,struct inp_file *in,char *token)
{
	int ret=-1;
	inp_reset_read(sim,in);
	char * line = inp_get_string(sim,in);
	gdouble value;
	while(line!=NULL)
	{
		if (strcmp(line,token)==0)
		{
			ret=0;
			do
			{
				line  = inp_get_string(sim,in);

				if (line==NULL)
				{
					break;
				}

				if (line[0]=='#')
				{
					break;
				}

				sscanf(line,"%Le",&value);
				out[ret]=value;

				ret++;

			}while(1);

			return ret;
		}

		line  = inp_get_string(sim,in);
	}

return ret;
}

int inp_count_hash_tags(struct simulation *sim,struct inp_file *in)
{
	int count=0;
	inp_reset_read(sim,in);
	char *line;
	do
	{
		line  = inp_get_string(sim,in);

		if (line==NULL)
		{
			break;
		}

		if (strlen(line)>0)
		{

			if (line[0]=='#')
			{
				count++;
			}

		}

	}while(1);



return count;
}

int inp_get_array(struct simulation *sim,char ** out,struct inp_file *in,char *token)
{
	int ret=-1;
	inp_reset_read(sim,in);
	char * line = inp_get_string(sim,in);
	while(line!=NULL)
	{
		if (strcmp(line,token)==0)
		{
			ret=0;
			do
			{
				line  = inp_get_string(sim,in);

				if (line==NULL)
				{
					break;
				}

				if (line[0]=='#')
				{
					break;
				}

				strcpy(out[ret],line);

				ret++;

			}while(1);

			return ret;
		}

		line  = inp_get_string(sim,in);
	}

return ret;
}

int inp_search_english(struct simulation *sim,struct inp_file *in,char *token)
{
	char line[4000];
	int ret=0;
	int pos=0;
	inp_reset_read(sim,in);
	ret=get_line(line,in->data,in->fsize,&pos);

	while(ret!=-1)
	{

		if (strcmp(line,token)==0)
		{
			ret=get_line(line,in->data,in->fsize,&pos);
			return english_to_bin(sim,line);
		}

		ret=get_line(line,in->data,in->fsize,&pos);
	}
ewe(sim,"Token %s not found in file %s",token,in->full_name);
exit(0);
return -1;
}

