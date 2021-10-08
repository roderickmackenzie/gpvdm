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

/** @file dat_file_zxyl_lignt.c
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
#include <enabled_libs.h>
//<strip>
#include <lock.h>
//</strip>


void dat_file_add_zxy_long_double_light_data(struct simulation *sim,struct dat_file *in,long double ***data, struct dim_light *dim)
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

void dat_file_add_zxy_from_zxyl_long_double_light_data(struct simulation *sim,struct dat_file *in,long double ****data, struct dim_light *dim,int l)
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
					sprintf(string,"%Le %Le %Le %Le\n",dim->z[z],dim->x[x],dim->y[y],data[z][x][y][l]);
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

void dat_file_add_zxy_from_zxyl_float_light_data(struct simulation *sim,struct dat_file *in,float ****data, struct dim_light *dim,int l)
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
					sprintf(string,"%Le %Le %Le %e\n",dim->z[z],dim->x[x],dim->y[y],data[z][x][y][l]);
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

void dat_file_add_zxy_from_zxyl_double_light_data(struct simulation *sim,struct dat_file *in,double ****data, struct dim_light *dim,int l)
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
					sprintf(string,"%Le %Le %Le %le\n",dim->z[z],dim->x[x],dim->y[y],data[z][x][y][l]);
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
