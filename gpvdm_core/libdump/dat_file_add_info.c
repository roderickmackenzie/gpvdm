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

void buffer_add_json(struct simulation *sim,struct dat_file *in)
{
	char temp[400];

	buffer_add_string(in,"#gpvdm_csv {");
	sprintf(temp,"\"title\": \"%s\",",in->title);
	buffer_add_string(in,temp);

	sprintf(temp,"\"type\": \"%s\",",in->type);
	buffer_add_string(in,temp);

	if (in->x_mul!=1.0)
	{
		sprintf(temp,"\"x_mul\": %Lf,",in->x_mul);
		buffer_add_string(in,temp);
	}

	if (in->y_mul!=1.0)
	{
		sprintf(temp,"\"y_mul\": %Lf,",in->y_mul);
		buffer_add_string(in,temp);
	}

	if (in->z_mul!=1.0)
	{
		sprintf(temp,"\"z_mul\": %Lf,",in->z_mul);
		buffer_add_string(in,temp);
	}

	if (in->data_mul!=1.0)
	{
		sprintf(temp,"\"data_mul\": %Lf,",in->data_mul);
		buffer_add_string(in,temp);
	}

	if (in->x_offset!=0)
	{
		sprintf(temp,"\"x_offset\": %Le,",in->x_offset);
		buffer_add_string(in,temp);
	}

	if (in->y_offset!=0)
	{
		sprintf(temp,"\"y_offset\": %Le,",in->y_offset);
		buffer_add_string(in,temp);
	}

	if (in->z_offset!=0)
	{
		sprintf(temp,"\"z_offset\": %Le,",in->z_offset);
		buffer_add_string(in,temp);
	}

	if (strcmp(in->x_label,"")!=0)
	{
		sprintf(temp,"\"x_label\": \"%s\",",in->x_label);
		buffer_add_string(in,temp);
	}

	if (strcmp(in->y_label,"")!=0)
	{
		sprintf(temp,"\"y_label\": \"%s\",",in->y_label);
		buffer_add_string(in,temp);
	}

	if (strcmp(in->z_label,"")!=0)
	{
		sprintf(temp,"\"z_label\": \"%s\",",in->z_label);
		buffer_add_string(in,temp);
	}

	sprintf(temp,"\"data_label\": \"%s\",",in->data_label);
	buffer_add_string(in,temp);

	if (strcmp(in->x_units,"")!=0)
	{
		sprintf(temp,"\"x_units\":\"%s\",",in->x_units);
		buffer_add_string(in,temp);
	}

	if (strcmp(in->y_units,"")!=0)
	{
		sprintf(temp,"\"y_units\":\"%s\",",in->y_units);
		buffer_add_string(in,temp);
	}

	if (strcmp(in->z_units,"")!=0)
	{
		sprintf(temp,"\"z_units\":\"%s\",",in->z_units);
		buffer_add_string(in,temp);
	}

	if (strcmp(in->rgb,"")!=0)
	{
		sprintf(temp,"\"rgb\":\"%s\",",in->rgb);
		buffer_add_string(in,temp);
	}

	sprintf(temp,"\"data_units\":\"%s\",",in->data_units);
	buffer_add_string(in,temp);

	if (in->logscale_x!=FALSE)
	{
		sprintf(temp,"\"logscale_x\":%d,",in->logscale_x);
		buffer_add_string(in,temp);
	}

	if (in->logscale_y!=FALSE)
	{
		sprintf(temp,"\"logscale_y\":%d,",in->logscale_y);
		buffer_add_string(in,temp);
	}

	if (in->logscale_z!=FALSE)
	{
		sprintf(temp,"\"logscale_z\":%d,",in->logscale_z);
		buffer_add_string(in,temp);
	}

	if (in->logscale_data!=FALSE)
	{
		sprintf(temp,"\"logscale_data\":%d,",in->logscale_data);
		buffer_add_string(in,temp);
	}
	sprintf(temp,"\"time\":%Le,",in->time);
	buffer_add_string(in,temp);

	sprintf(temp,"\"Vexternal\":%Le,",in->Vexternal);
	buffer_add_string(in,temp);

	if (isnan(in->data_max)!=1)
	{
		sprintf(temp,"\"data_max\":%Le,",in->data_max);
		buffer_add_string(in,temp);
	}

	if (isnan(in->data_min)!=1)
	{
		sprintf(temp,"\"data_min\":%Le,",in->data_min);
		buffer_add_string(in,temp);
	}

	if (in->x!=0)
	{
		sprintf(temp,"\"x_len\":%d,",in->x);
		buffer_add_string(in,temp);
	}

	if (in->y!=0)
	{
		sprintf(temp,"\"y_len\":%d,",in->y);
		buffer_add_string(in,temp);
	}

	if (in->z!=0)
	{
		sprintf(temp,"\"z_len\":%d,",in->z);
		buffer_add_string(in,temp);
	}

	if (strcmp(in->cols,"")!=0)
	{
		sprintf(temp,"\"cols\":\"%s\",",in->cols);
		buffer_add_string(in,temp);
	}

	if (in->bin==TRUE)
	{
		sprintf(temp,"\"bin\":%d,",in->bin);
		buffer_add_string(in,temp);
	}

	if (in->srh_bands!=-1)
	{
		sprintf(temp,"\"srh_bands\":%d,",in->srh_bands);
		buffer_add_string(in,temp);
	}

	in->buf[in->len--]=0;
	buffer_add_string(in,"}*\n");
}

void buffer_add_csv_header(struct simulation *sim,struct dat_file *in)
{
int i=0;
char col=0;
char temp[1000];
int len=0;


len=strlen(in->cols);

buffer_add_string(in,"#");
for (i=0;i<len;i++)
{
	col=in->cols[i];
	if (col=='z')
	{
		sprintf(temp,"%s (%s) ",in->z_label,in->z_units);
		buffer_add_string(in,temp);
	}else
	if (col=='x')
	{
		sprintf(temp,"%s (%s) ",in->x_label,in->x_units);
		buffer_add_string(in,temp);
	}else
	if (col=='y')
	{
		sprintf(temp,"%s (%s) ",in->y_label,in->y_units);
		buffer_add_string(in,temp);
	}else
	if (col=='d')
	{
		sprintf(temp,"%s (%s) ",in->data_label,in->data_units);
		buffer_add_string(in,temp);
	}

	if (i!=len-1)
	{
		buffer_add_string(in,", ");
	}else
	{
		buffer_add_string(in,"\n");
	}
}



}
