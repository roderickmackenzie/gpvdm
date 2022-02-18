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

/** @file dump.c
@brief go and dump stuff, what is dumped depends on which options have been set
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <exp.h>
#include "sim.h"
#include "dump.h"
#include <cal_path.h>
#include <pl.h>
#include <probe.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <inp.h>
#include <color.h>
#include <memory.h>
#include <ray_fun.h>
#include <device_fun.h>
#include <heat_fun.h>
#include <contacts.h>
#include <enabled_libs.h>
#include <circuit.h>

static int unused __attribute__((unused));


void dat_file_add_zxy(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,void ***data,int var_type)
{
	int x=0;
	int y=0;
	int z=0;
	float ***tmp_float;
	double ***tmp_double;
	long double ***tmp_long_double;
	long double tmp;
	char string[200];

	tmp_float=(float ***)data;
	tmp_double=(double ***)data;
	tmp_long_double=(long double ***)data;

	if ((dim->xlen>1)&&(dim->ylen>1)&&(dim->zlen>1))
	{
		strcpy(buf->cols,"xyzd");
		buffer_add_json(sim,buf);
		if (buf->bin==FALSE)
		{
			buffer_add_csv_header(sim,buf);

			for (z=0;z<dim->zlen;z++)
			{
				for (x=0;x<dim->xlen;x++)
				{
					for (y=0;y<dim->ylen;y++)
					{
						if (var_type==GPVDM_FLOAT)
						{
							tmp=tmp_float[z][x][y];
						}else
						if (var_type==GPVDM_DOUBLE)
						{
							tmp=tmp_double[z][x][y];
						}else
						if (var_type==GPVDM_LONG_DOUBLE)
						{
							tmp=tmp_long_double[z][x][y];
						}

						sprintf(string,"%Le %Le %Le %Le\n",dim->x[x],dim->y[y],dim->z[z],tmp);
						buffer_add_string(buf,string);
					}
				}
			}
		}
	}else
	if ((dim->xlen>1)&&(dim->ylen>1))
	{
		strcpy(buf->cols,"xyd");
		buffer_add_json(sim,buf);
		if (buf->bin==FALSE)
		{
			buffer_add_csv_header(sim,buf);

			z=0;
			for (x=0;x<dim->xlen;x++)
			{
				for (y=0;y<dim->ylen;y++)
				{
					if (var_type==GPVDM_FLOAT)
					{
						tmp=tmp_float[z][x][y];
					}else
					if (var_type==GPVDM_DOUBLE)
					{
						tmp=tmp_double[z][x][y];
					}else
					if (var_type==GPVDM_LONG_DOUBLE)
					{
						tmp=tmp_long_double[z][x][y];
					}
					sprintf(string,"%Le %Le %Le\n",dim->x[x],dim->y[y],tmp);
					buffer_add_string(buf,string);
				}
				buffer_add_string(buf,"\n");
			}
		}
	}else
	if ((dim->zlen>1)&&(dim->ylen>1))
	{
		strcpy(buf->cols,"yzd");
		buffer_add_json(sim,buf);
		if (buf->bin==FALSE)
		{

			buffer_add_csv_header(sim,buf);
			for (z=0;z<dim->zlen;z++)
			{
				for (y=0;y<dim->ylen;y++)
				{
					if (var_type==GPVDM_FLOAT)
					{
						tmp=tmp_float[z][x][y];
					}else
					if (var_type==GPVDM_DOUBLE)
					{
						tmp=tmp_double[z][x][y];
					}else
					if (var_type==GPVDM_LONG_DOUBLE)
					{
						tmp=tmp_long_double[z][x][y];
					}

					sprintf(string,"%Le %Le %Le\n",dim->y[y],dim->z[z],tmp);
					buffer_add_string(buf,string);
				}
				buffer_add_string(buf,"\n");
			}
		}
	}else
	if ((dim->zlen>1)&&(dim->xlen>1))
	{
		strcpy(buf->cols,"xzd");
		buffer_add_json(sim,buf);
		if (buf->bin==FALSE)
		{

			buffer_add_csv_header(sim,buf);
			for (z=0;z<dim->zlen;z++)
			{
				for (x=0;x<dim->xlen;x++)
				{

					if (var_type==GPVDM_FLOAT)
					{
						tmp=tmp_float[z][x][y];
					}else
					if (var_type==GPVDM_DOUBLE)
					{
						tmp=tmp_double[z][x][y];
					}else
					if (var_type==GPVDM_LONG_DOUBLE)
					{
						tmp=tmp_long_double[z][x][y];
					}

					sprintf(string,"%Le %Le %Le\n",dim->x[x],dim->z[z],tmp);
					buffer_add_string(buf,string);
				}
				buffer_add_string(buf,"\n");
			}
		}
	}else
	{
		strcpy(buf->cols,"yd");
		buffer_add_json(sim,buf);

		x=0;
		z=0;
		if (buf->bin==FALSE)
		{

			buffer_add_csv_header(sim,buf);
			for (y=0;y<dim->ylen;y++)
			{
				if (var_type==GPVDM_FLOAT)
				{
					tmp=tmp_float[z][x][y];
				}else
				if (var_type==GPVDM_DOUBLE)
				{
					tmp=tmp_double[z][x][y];
				}else
				if (var_type==GPVDM_LONG_DOUBLE)
				{
					tmp=tmp_long_double[z][x][y];
				}

				sprintf(string,"%Le %Le\n",dim->y[y],tmp);
				buffer_add_string(buf,string);
			}
		}
	}

	if (buf->bin==TRUE)
	{
		int pos=buf->len;

		int size=(dim->zlen+dim->xlen+dim->ylen)*sizeof(float);
		size+=dim->zlen*dim->xlen*dim->ylen*sizeof(float);
		dat_file_increase_buffer(buf,size);

		for (z=0;z<dim->zlen;z++)
		{
			*((float *)(buf->buf+pos))=dim->z[z];
			pos+=sizeof(float);
		}

		for (x=0;x<dim->xlen;x++)
		{
			*((float *)(buf->buf+pos))=dim->x[x];
			pos+=sizeof(float);
		}

		for (y=0;y<dim->ylen;y++)
		{
			*((float *)(buf->buf+pos))=dim->y[y];
			pos+=sizeof(float);
		}

		for (z=0;z<dim->zlen;z++)
		{
			for (x=0;x<dim->xlen;x++)
			{
				for (y=0;y<dim->ylen;y++)
				{
					if (var_type==GPVDM_FLOAT)
					{
						*((float *)(buf->buf+pos))=(float)tmp_float[z][x][y];
					}else
					if (var_type==GPVDM_DOUBLE)
					{
						*((float *)(buf->buf+pos))=(float)tmp_double[z][x][y];
					}else
					if (var_type==GPVDM_LONG_DOUBLE)
					{
						*((float *)(buf->buf+pos))=(float)tmp_long_double[z][x][y];
					}
					
					pos+=sizeof(float);
				}
			}
		}

	}

}
