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

/** @file buffer.c
@brief used to save output files to disk with a nice header, so the user knows what was writtne to them
*/

#include <enabled_libs.h>
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
#include <lang.h>
//<strip>
#include <lock.h>
//</strip>

void buffer_zip_set_name(struct dat_file *in,char * name)
{
	strcpy(in->zip_file_name,name);
	in->write_to_zip=TRUE;
}


void buffer_malloc(struct dat_file *in)
{
	in->buf=(char*)malloc(sizeof(char)*in->max_len);
	memset(in->buf, 0, in->max_len);
}

void buffer_free(struct dat_file *in)
{
	free(in->buf);
	in->len=0;
	in->max_len=0;
}

void dat_file_add_xy_data_shift(struct simulation *sim,struct dat_file *in,gdouble *x, gdouble *y, int len,long double shift)
{
int i;
long double *temp;
temp=malloc(len*sizeof(long double));

for (i=0;i<len;i++)
{
	temp[i]=y[i]+shift;
}

dat_file_add_xy_data(sim,in,x,temp, len);

free(temp);
}

void dat_file_add_xy_data(struct simulation *sim,struct dat_file *in, gdouble *x, gdouble *y, int len)
{
int i;
char string[200];
gdouble x_out=0.0;
gdouble y_out=0.0;
gdouble max=0.0;
gdouble min=0.0;
int csv=FALSE;
in->x=1;
in->y=len;
in->z=1;
strcpy(in->cols,"yd");

if (strcmp_end(in->file_name,"csv")==0)
{
	csv=TRUE;
	buffer_add_json(sim,in);
	buffer_add_csv_header(sim,in);
}else
{
	buffer_add_info(sim,in);
}

if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	if (csv==TRUE)
	{

	}else
	{
		sprintf(string,"#data\n");
		buffer_add_string(in,string);
	}
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

		sprintf(string,"%Le\t%Le\n",x_out,y_out);
		buffer_add_string(in,string);
	}
}

if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	if (csv==TRUE)
	{

	}else
	{
		sprintf(string,"#end\n");
		buffer_add_string(in,string);
	}
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
				sprintf(string,"%Le %Le %Le %d\n",dim->z[z],dim->x[x],dim->y[y],data[z][x][y]->uid);
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

void dat_file_increase_buffer(struct dat_file *in,int size)
{
	in->len+=size;
	if (in->len-in->max_len>0)
	{
		in->max_len+=in->len-in->max_len;
		in->buf=(char*)realloc((char*)in->buf,sizeof(char)*in->max_len);
	}
}

void buffer_add_string(struct dat_file *in,char * string)
{
int str_len=strlen(string);
int pos=in->len;
in->len+=str_len;
int Kb=1024;
//the +1 accounts for the zero at the end of a string
if (in->len+100>in->max_len)
{
	if (in->max_len<Kb*100)	//100k
	{
		in->max_len+=Kb*10;
	}else
	{
		in->max_len+=Kb*100;
	}
	in->buf=(char*)realloc((char*)in->buf,sizeof(char)*in->max_len);
	//printf("here\n");
}

strcpy((char*)(in->buf+pos),string);
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

	//printf("file '%s %p'\n",file,out);

	//printf("write '%d %d'\n",in->len,strlen(in->buf));
	//printf(">>'%s'\n",in->buf);
	fwrite(in->buf, in->len, 1, out);

	fclose(out);


	log_write_file_access(sim,file,'w');
	//printf("wait\n");
	//getchar();
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
	buffer_dump(sim,file_name,in);
}

int buffer_set_file_name(struct simulation *sim,struct device *dev,struct dat_file *in,char * file_name)
{
	if (dev==NULL)
	{
		strcpy(in->file_name,file_name);
		return 0;
	}

	if (dump_can_i_dump(sim,dev, file_name)==0)
	{
		strcpy(in->file_name,file_name);
		return 0;
	}

	return -1;
}

void buffer_dump_path(struct simulation *sim,char *path,char * file,struct dat_file *in)
{
	char wholename[PATH_MAX];
	if (file!=NULL)
	{
		join_path(2, wholename,path,file);
	}else
	{
		if (strcmp(in->file_name,"")==0)
		{
			ewe(sim,_("file name not set\n"));
		}
		join_path(2, wholename,path,in->file_name);
	}

	buffer_dump_cache(sim,wholename,in);
}


void dat_file_reset(struct dat_file *in)
{
	in->len=0;
}
