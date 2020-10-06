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
@brief functions to read input files, this is a copy from gpvdm
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
#include <linux/limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



void inp_listdir( char *dir_name,struct inp_list *out)
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
		if (inp_listcmp(out,temp)!=0)
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

void inp_list_free(struct inp_list *in)
{
	int i=0;

	for (i=0;i<in->len;i++)
	{
		free(in->names[i]);
	}

	free(in->names);
}

int inp_listcmp(struct inp_list *in,char *name)
{
	int i=0;

	for (i=0;i<in->len;i++)
	{
		if (strcmp(name,in->names[i])==0)
		{
			return 0;
		}
	}

return -1;
}

int isfile(char *in)
{
    struct stat path_stat;
    if (stat(in, &path_stat)!=0)
	{
		return -1;
	}
	int ret=0;
	ret=S_ISREG(path_stat.st_mode);
	if (ret==0)
	{
		return -1;
	}

    return 0;

}


int zip_is_in_archive(char *full_file_name)
{
	char zip_path[1000];
	char file_path[1000];
	get_dir_name_from_path(file_path,full_file_name);
	char *file_name=get_file_name_from_path(full_file_name);

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

int inp_isfile(char *full_file_name)
{
FILE *f = fopen(full_file_name, "rb");
if (f!=NULL)
{
	fclose(f);
	return 0;
}else
{
	return zip_is_in_archive(full_file_name);
}
//#endif

}

int inp_search_pos(struct inp_file *in,char *token)
{
	int pos=0;
	inp_reset_read(in);
	char * line=NULL;
	do
	{
		line  = inp_get_string(in);
		if (line==NULL)
		{
			break;
		}
		//printf("'%s' '%s'\n", line,token);
		if (strcmp(line,token)==0)
		{
			return pos;
		}

		pos++;

	}while(1);

return -1;
}

void inp_reset_read(struct inp_file *in)
{
in->pos=0;
}

char* inp_get_string(struct inp_file *in)
{
int i;
memset(in->temp, 0, 100);
int ii=0;
if (in->pos>=in->fsize)
{
	return NULL;
}

for (i=in->pos;i<in->fsize;i++)
{
	if ((in->data[i]=='\n')||(in->data[i]==0))
	{
		in->temp[ii]=0;
		in->pos++;
		break;
	}

	in->temp[ii]=in->data[i];
	ii++;
	in->pos++;

}

return in->temp;
}




int inp_read_buffer(char **buf, long *len,char *full_file_name)
{
FILE *f = fopen(full_file_name, "rb");

if (f!=NULL)
{
	int err=0;
	fseek(f, 0, SEEK_END);
	*len = ftell(f);
	fseek(f, 0, SEEK_SET);

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
	get_dir_name_from_path(file_path,full_file_name);

	char *file_name=get_file_name_from_path(full_file_name);

	join_path(2,zip_path,file_path,"sim.gpvdm");

	//printf("1>%s 2>%s 3>%s 4>%s\n",full_file_name,file_path,file_name,zip_path);
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

			//Read the compressed file
			struct zip_file *f = zip_fopen(z, file_name, 0);
			if (f==NULL)
			{
				return -1;
			}

			ret=zip_fread(f, *buf, st.size);
			if (ret==-1)
			{
				return -1;
			}
			zip_fclose(f);
			(*buf)[*len]=0;
		}else
		{
			//printf("can't find rod");
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

void inp_init(struct inp_file *in)
{
strcpy(in->full_name,"");
in->data=NULL;
in->fsize=0;
in->pos=0;
in->edited=FALSE;
}

int inp_load_from_path(struct inp_file *in,char *path,char *file)
{
int ret=0;
char full_path[1000];
join_path(2,full_path,path,file);
ret=inp_load(in,full_path);
return ret;
}

int inp_load(struct inp_file *in,char *file)
{
int ret=0;
in->pos=0;
if (strcmp(in->full_name,file)!=0)
{
	//printf("Reload %s %s\n",in->full_name,file);
	if (in->data!=NULL)
	{
		inp_free(in);
	}

	strcpy(in->full_name,file);
	if (inp_read_buffer(&(in->data),&(in->fsize),file)!=0)
	{
		ret= -1;
	}

	in->edited=FALSE;
}

return ret;
}

void inp_replace_double(struct inp_file *in,char *token, double value)
{
	char temp[100];
	sprintf(temp,"%le",value);
	inp_replace(in,token,temp);
}

void inp_replace(struct inp_file *in,char *token, char *text)
{
char *temp = malloc(in->fsize + 100);
memset(temp, 0, in->fsize + 100);
char *line;
int len=0;

line = strtok(in->data, "\n");
int found=FALSE;

while(line)
{
	if (strcmp(line,token)!=0)
	{
		strcat(temp,line);
		strcat(temp,"\n");
	}else
	{
		strcat(temp,line);
		strcat(temp,"\n");
		strcat(temp,text);
		strcat(temp,"\n");
		line  = strtok(NULL, "\n");
		found=TRUE;
	}
	line  = strtok(NULL, "\n");
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
	//printf("%s %d\n",in->data,len);
	ewe("String not ended\n");
}
free(temp);
}

int zip_write_buffer(char *full_file_name,char *buffer, int len)
{
int	in_zip_file= -1;
int	outside_zip_file= -1;

in_zip_file=zip_is_in_archive(full_file_name);
outside_zip_file=isfile(full_file_name);


if ((in_zip_file!=0)||(outside_zip_file==0))
{
	int out_fd = open(full_file_name, O_WRONLY | O_CREAT | O_TRUNC,0644);

	if (out_fd== -1)
	{
		ewe("File %s can not be opened\n",full_file_name);
	}

	if (ftruncate(out_fd, 0)== -1)
	{
		ewe("Error with truncate command\n");
	}

	if (write(out_fd, buffer, len*sizeof(char))== -1)
	{
		ewe("Error writing data to file.\n");
	}
	close(out_fd);
}else
{
	char zip_path[1000];
	char file_path[1000];
	get_dir_name_from_path(file_path,full_file_name);
	char *file_name=get_file_name_from_path(full_file_name);

	join_path(2,zip_path,file_path,"sim.gpvdm");

	int err = 0;
	struct zip *z = zip_open(zip_path, 0, &err);

	if (z!=NULL)
	{
		struct zip_source *s;
		s=zip_source_buffer(z, buffer, len,0);
		int index=zip_name_locate(z,  file_name,  0);

		if (index== -1)
		{
			zip_add(z, file_name, s);
		}else
		{
			zip_replace(z, index, s);
		}



		zip_close(z);
	}else
	{
		return -1;
	}

}
return 0;
}

int inp_save(struct inp_file *in)
{

if (in->edited==TRUE)
{
	zip_write_buffer(in->full_name,in->data, in->fsize);
	in->edited=FALSE;
}

return 0;
}

void inp_free(struct inp_file *in)
{

	inp_save(in);

	free(in->data);
	inp_init(in);
}


int inp_search_double(struct inp_file *in,double* out,char* token)
{
char temp[200];
if (inp_search(temp,in,token)==0)
{
	sscanf(temp,"%le",out);
	return 0;
}
return -1;
}


int inp_search_int(struct inp_file *in,int* out,char* token)
{
char temp[200];
if (inp_search(temp,in,token)==0)
{
	sscanf(temp,"%d",out);
	return 0;
}
return -1;
}

int inp_search_longint(struct inp_file *in,long int* out,char* token)
{
char temp[200];
if (inp_search(temp,in,token)==0)
{
	sscanf(temp,"%ld",out);
	return 0;
}
return -1;
}

int inp_search_string(struct inp_file *in,char* out,char* token)
{
if (inp_search(out,in,token)==0)
{
	return 0;
}
return -1;
}

void inp_check(struct inp_file *in,double ver)
{
double read_ver=0.0;
	inp_reset_read(in);
	//printf("%s\nname=%s\n",in->data,in->full_name);
	char *line=inp_get_string(in);
	while(line!=NULL)
	{
		if (strcmp(line,"#ver")==0)
		{
			line  = inp_get_string(in);

			sscanf(line,"%lf",&(read_ver));

			if (ver!=read_ver)
			{
				ewe("File compatibility problem %s\n",in->full_name);
			}
			line  = inp_get_string(in);

			if ((line==NULL)||(strcmp(line,"#end")!=0))
			{
				ewe("#end token missing %s\n",in->full_name);
			}

			return;
		}

		line  = inp_get_string(in);
	}

ewe("Token #ver not found in %s\n",in->full_name);
return;
}

char* inp_search_part(struct inp_file *in,char *token)
{
	inp_reset_read(in);
	char * line = inp_get_string(in);
	while(line!=NULL)
	{
		//printf("'%s' '%s'\n", line,token);
		if (cmpstr_min(line,token)==0)
		{
			return line;
		}

		line  = inp_get_string(in);
	}

return NULL;
}


int inp_search(char* out,struct inp_file *in,char *token)
{
	inp_reset_read(in);
	char * line = inp_get_string(in);
	while(line!=NULL)
	{
		//printf("'%s' '%s'\n", line,token);
		if (strcmp(line,token)==0)
		{
			line  = inp_get_string(in);
			strcpy(out,line);
			return 0;
		}

		line  = inp_get_string(in);
	}

return -1;
}

int inp_get_array_len(struct inp_file *in,char *token)
{
	int ret=-1;
	inp_reset_read(in);
	char * line = inp_get_string(in);
	while(line!=NULL)
	{
		if (strcmp(line,token)==0)
		{
			ret=0;
			do
			{
				line  = inp_get_string(in);
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

		line  = inp_get_string(in);
	}

return ret;
}

int inp_get_array(char ** out,struct inp_file *in,char *token)
{
	int ret=-1;
	inp_reset_read(in);
	char * line = inp_get_string(in);
	while(line!=NULL)
	{
		if (strcmp(line,token)==0)
		{
			ret=0;
			do
			{
				line  = inp_get_string(in);

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

		line  = inp_get_string(in);
	}

return ret;
}

int inp_search_english(struct inp_file *in,char *token)
{
	inp_reset_read(in);
	char * line = inp_get_string(in);
	while(line!=NULL)
	{
		//printf("'%s' '%s'\n", line,token);
		if (strcmp(line,token)==0)
		{
			line  = inp_get_string(in);
			return english_to_bin(line);
		}

		line  = inp_get_string(in);
	}
ewe("Token %s not found in file %s",token,in->full_name);
exit(0);
return -1;
}

