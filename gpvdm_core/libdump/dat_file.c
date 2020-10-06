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

/** @file buffer.c
@brief used to save output files to disk with a nice header, so the user knows what was writtne to them
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "dat_file.h"
#include "gpvdm_const.h"
#include "code_ctrl.h"
#include "cal_path.h"
#include "dump.h"
#include <log.h>
#include <cache.h>
#include <enabled_libs.h>
//<strip>
#include <lock.h>
//</strip>

void buffer_zip_set_name(struct dat_file *in,char * name)
{
	strcpy(in->zip_file_name,name);
	in->write_to_zip=TRUE;
}

void buffer_init(struct dat_file *in)
{
in->write_to_zip=FALSE;
in->norm_x_axis=FALSE;
in->norm_y_axis=FALSE;
in->data_max=NAN;
in->data_min=NAN;
}

void buffer_malloc(struct dat_file *in)
{
in->len=0;
in->max_len=0;
in->max_len+=10240;
strcpy(in->title,"");
strcpy(in->type,"xy");
strcpy(in->title,"");
in->x_mul=1.0;
in->y_mul=1.0;
in->z_mul=1.0;
in->x_offset=0.0;
in->y_offset=0.0;
in->z_offset=0.0;
in->data_mul=1.0;
strcpy(in->x_label,"");
strcpy(in->y_label,"");
strcpy(in->z_label,"");
strcpy(in->data_label,"");
strcpy(in->x_units,"");
strcpy(in->y_units,"");
strcpy(in->z_units,"");
strcpy(in->rgb,"");
strcpy(in->data_units,"");
strcpy(in->section_one,"");
strcpy(in->section_two,"");
in->logscale_x=0;
in->logscale_y=0;
in->logscale_z=0;
in->logscale_data=0;
in->x=0;
in->y=0;
in->z=0;
in->time= -1.0;
in->Vexternal= -1.0;
in->dump_gnu_plot_file=FALSE;
in->buf=(char*)malloc(sizeof(char)*in->max_len);
memset(in->buf, 0, in->max_len);
}

void buffer_add_xy_data_shift(struct simulation *sim,struct dat_file *in,gdouble *x, gdouble *y, int len,long double shift)
{
int i;
long double *temp;
temp=malloc(len*sizeof(long double));

for (i=0;i<len;i++)
{
	temp[i]=y[i]+shift;
}

buffer_add_xy_data(sim,in,x,temp, len);

free(temp);
}

void buffer_add_xy_data(struct simulation *sim,struct dat_file *in,gdouble *x, gdouble *y, int len)
{
int i;
char string[200];
gdouble x_out=0.0;
gdouble y_out=0.0;
gdouble max=0.0;
gdouble min=0.0;

if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#data\n");
	buffer_add_string(in,string);
}

if (len>0)
{
	max=y[0];
	min=y[0];

	for (i=0;i<len;i++)
	{
		if (y[i]<min) min=y[i];
		if (y[i]>max) max=y[i];
	}


	for (i=0;i<len;i++)
	{
		x_out=x[i];
		y_out=y[i];
		if (in->norm_x_axis==TRUE)
		{
			x_out/=x[len-1];
		}

		if (in->norm_y_axis==TRUE)
		{
			y_out=(y[i]-min)/(max-min);
		}

		sprintf(string,"%Le %Le\n",x_out,y_out);
		buffer_add_string(in,string);
	}
}

if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#end\n");
	buffer_add_string(in,string);
}

}


void buffer_add_zxy_object_uid(struct simulation *sim,struct dat_file *in,struct object ****data, struct dimensions *dim)
{
int z;
int x;
int y;
char string[100];

if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#data\n");
	buffer_add_string(in,string);
}

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				sprintf(string,"%Le %Le %Le %d\n",dim->zmesh[z],dim->xmesh[x],dim->ymesh[y],data[z][x][y]->uid);
				buffer_add_string(in,string);
			}
		}
	}


if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#end\n");
	buffer_add_string(in,string);
}

}

void buffer_add_zxy_heat_data(struct simulation *sim,struct dat_file *in,long double ***data, struct dim_heat *dim)
{
int z;
int x;
int y;
char string[100];

if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#data\n");
	buffer_add_string(in,string);
}

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				sprintf(string,"%Le %Le %Le %Le\n",dim->z[z],dim->x[x],dim->y[y],data[z][x][y]);
				buffer_add_string(in,string);
			}
		}
	}


if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#end\n");
	buffer_add_string(in,string);
}

}

void buffer_add_xy_data_z_label(struct dat_file *in,gdouble *x, gdouble *y, gdouble *z, int len)
{
int i;
char string[200];
gdouble x_out=0.0;
gdouble y_out=0.0;
gdouble max=0.0;
gdouble min=0.0;

sprintf(string,"#data\n");
buffer_add_string(in,string);


if (len>0)
{
	max=y[0];
	min=y[0];

	for (i=0;i<len;i++)
	{
		if (y[i]<min) min=y[i];
		if (y[i]>max) max=y[i];
	}

	for (i=0;i<len;i++)
	{
		x_out=x[i];
		y_out=y[i];
		if (in->norm_x_axis==TRUE)
		{
			x_out/=x[len-1];
		}

		if (in->norm_y_axis==TRUE)
		{
			y_out=(y[i]-min)/(max-min);
		}

		sprintf(string,"%Le %Le #%Le\n",x_out,y_out,z[i]);
		buffer_add_string(in,string);
	}
}
sprintf(string,"#end\n");
buffer_add_string(in,string);

}

void buffer_add_string(struct dat_file *in,char * string)
{
int str_len=strlen(string);
int pos=in->len;
in->len+=str_len;
//the +1 accounts for the zero at the end of a string
if (in->len+100>in->max_len)
{
	in->max_len+=10240;
	in->buf=(char*)realloc((char*)in->buf,sizeof(char)*in->max_len);
	//printf("here\n");
}

strcpy((char*)(in->buf+pos),string);
}

void buffer_add_info(struct simulation *sim,struct dat_file *in)
{
char temp[400];

//if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	buffer_add_string(in,"#gpvdm\n");
	sprintf(temp,"#title %s\n",in->title);
	buffer_add_string(in,temp);

	sprintf(temp,"#type %s\n",in->type);
	buffer_add_string(in,temp);

	sprintf(temp,"#x_mul %Lf\n",in->x_mul);
	buffer_add_string(in,temp);

	sprintf(temp,"#y_mul %Lf\n",in->y_mul);
	buffer_add_string(in,temp);

	sprintf(temp,"#z_mul %Lf\n",in->z_mul);
	buffer_add_string(in,temp);

	sprintf(temp,"#data_mul %Lf\n",in->data_mul);
	buffer_add_string(in,temp);

	if (in->x_offset!=0)
	{
		sprintf(temp,"#x_offset %Le\n",in->x_offset);
		buffer_add_string(in,temp);
	}

	if (in->y_offset!=0)
	{
		sprintf(temp,"#y_offset %Le\n",in->y_offset);
		buffer_add_string(in,temp);
	}

	if (in->z_offset!=0)
	{
		sprintf(temp,"#z_offset %Le\n",in->z_offset);
		buffer_add_string(in,temp);
	}

	sprintf(temp,"#x_label %s\n",in->x_label);
	buffer_add_string(in,temp);

	sprintf(temp,"#y_label %s\n",in->y_label);
	buffer_add_string(in,temp);

	sprintf(temp,"#z_label %s\n",in->z_label);
	buffer_add_string(in,temp);

	sprintf(temp,"#data_label %s\n",in->data_label);
	buffer_add_string(in,temp);

	sprintf(temp,"#x_units %s\n",in->x_units);
	buffer_add_string(in,temp);

	sprintf(temp,"#y_units %s\n",in->y_units);
	buffer_add_string(in,temp);

	sprintf(temp,"#z_units %s\n",in->z_units);
	buffer_add_string(in,temp);

	if (strcmp(in->rgb,"")!=0)
	{
		sprintf(temp,"#rgb %s\n",in->rgb);
		buffer_add_string(in,temp);
	}

	sprintf(temp,"#data_units %s\n",in->data_units);
	buffer_add_string(in,temp);

	sprintf(temp,"#logscale_x %d\n",in->logscale_x);
	buffer_add_string(in,temp);

	sprintf(temp,"#logscale_y %d\n",in->logscale_y);
	buffer_add_string(in,temp);

	sprintf(temp,"#logscale_z %d\n",in->logscale_z);
	buffer_add_string(in,temp);

	sprintf(temp,"#logscale_data %d\n",in->logscale_data);
	buffer_add_string(in,temp);

	sprintf(temp,"#section_one %s\n",in->section_one);
	buffer_add_string(in,temp);

	sprintf(temp,"#section_two %s\n",in->section_two);
	buffer_add_string(in,temp);

	sprintf(temp,"#time %Le\n",in->time);
	buffer_add_string(in,temp);

	sprintf(temp,"#Vexternal %Le\n",in->Vexternal);
	buffer_add_string(in,temp);

	if (isnan(in->data_max)!=1)
	{
		sprintf(temp,"#data_max %Le\n",in->data_max);
		buffer_add_string(in,temp);
	}

	if (isnan(in->data_min)!=1)
	{
		sprintf(temp,"#data_min %Le\n",in->data_min);
		buffer_add_string(in,temp);
	}

	if (in->x!=0)
	{
		sprintf(temp,"#x %d\n",in->x);
		buffer_add_string(in,temp);
	}

	if (in->y!=0)
	{
		sprintf(temp,"#y %d\n",in->y);
		buffer_add_string(in,temp);
	}

	if (in->z!=0)
	{
		sprintf(temp,"#z %d\n",in->z);
		buffer_add_string(in,temp);
	}

	sprintf(temp,"#begin\n");
	buffer_add_string(in,temp);
}
}

void buffer_dump(struct simulation *sim,char * file,struct dat_file *in)
{
	FILE *out;
	sim->files_written++;
	sim->bytes_written+=in->len;

	out = fopen(file, "wb");
	if (out==NULL)
	{
		ewe(sim,"I can't make the file %s\n",file);
	}

	int out_len;
	char *out_data;

	//<strip>
	{
		fwrite(in->buf, in->len, 1, out);
	}

	fclose(out);


	log_write_file_access(sim,file,'w');
}

void dat_file_dump_gnuplot_file(struct simulation *sim,char * path,char * file_name,struct dat_file *in)
{
	FILE *out;
	int ret=0;
	char file_base[200];
	char base[PATH_MAX];
	char out_file[PATH_MAX];

	split_dot(file_base, file_name);

	join_path(2, base ,path,file_base);

	ret=snprintf(out_file,PATH_MAX,"%s.plot",base);
	if (ret<0)
	{
		ewe(sim,"dat_file.c: sprintf error\n");
	}

	out = fopen(out_file, "wb");
	//printf("\n\n%s\n%s\n",out_file,file_name);
	//getchar();
	if (out==NULL)
	{
		ewe(sim,"I can't make the file %s\n",out_file);
	}
	fprintf(out, "set xlabel '%s (%s)'\n", in->y_label, in->y_units );
	fprintf(out, "set ylabel '%s (%s)'\n", in->data_label, in->data_units );
	fprintf(out, "plot '%s.dat' with l\n", base );
	fclose(out);
}

void buffer_dump_cache(struct simulation *sim,char * file_name,struct dat_file *in)
{
	if (dumpfiles_should_dump(sim,file_name)==0)
	{
		if ((get_dump_status(sim,dump_use_cache)==FALSE)||(sim->cache_len==-1))
		{
			buffer_dump(sim,file_name,in);
		}else
		{
			cache_add_item(sim,file_name,in->buf,in->len);
		}
	}
}

void buffer_add_dir(struct simulation *sim,char * file_name)
{
	if ((get_dump_status(sim,dump_use_cache)==FALSE)||(sim->cache_len==-1))
	{
		gpvdm_mkdir(file_name);
	}else
	{
		cache_add_dir(sim,file_name);
	}
}

void buffer_dump_path(struct simulation *sim,char *path,char * file,struct dat_file *in)
{
	char wholename[PATH_MAX];
	join_path(2, wholename,path,file);
	buffer_dump_cache(sim,wholename,in);
}


void buffer_free(struct dat_file *in)
{
free(in->buf);
in->len=0;
in->max_len=0;
}
