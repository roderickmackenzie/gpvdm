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


void dat_file_add_zxy_float(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,float ***data)
{
	int x=0;
	int y=0;
	int z=0;

	char string[200];


	if ((dim->xlen>1)&&(dim->ylen>1)&&(dim->zlen>1))
	{
		strcpy(buf->cols,"xyzd");
		buffer_add_json(sim,buf);
		buffer_add_csv_header(sim,buf);

		for (z=0;z<dim->zlen;z++)
		{
			for (x=0;x<dim->xlen;x++)
			{
				for (y=0;y<dim->ylen;y++)
				{
					sprintf(string,"%Le %Le %Le %e\n",dim->x[x],dim->y[y],dim->z[z],data[z][x][y]);
					buffer_add_string(buf,string);
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
					sprintf(string,"%Le %Le %e\n",dim->x[x],dim->y[y],data[z][x][y]);
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
					sprintf(string,"%Le %Le %e\n",dim->y[y],dim->z[z],data[z][x][y]);
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
					sprintf(string,"%Le %Le %e\n",dim->x[x],dim->z[z],data[z][x][y]);
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
				sprintf(string,"%Le %e\n",dim->y[y],data[z][x][y]);
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
					*((float *)(buf->buf+pos))=data[z][x][y];
					pos+=sizeof(float);
				}
			}
		}

	}

}

void dat_file_add_zxy_float_l(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,float ****data, int l)
{
	int x;
	int y;
	int z;
	long double ***temp=NULL;
	malloc_zxy_long_double(dim, &temp);

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				temp[z][x][y]=data[z][x][y][l];
			}
		}
	}

	dat_file_add_zxy_data(sim,buf,dim,temp);
	free_zxy_long_double(dim, &temp);
}
