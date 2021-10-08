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

/** @file dim_heat.c
@brief Dimension object for heat
*/

#include <device.h>
#include "mesh.h"
#include "inp.h"
#include "util.h"
#include "gpvdm_const.h"
#include "hard_limit.h"
#include <log.h>
#include <cal_path.h>
#include <lang.h>
#include <shape.h>

void dim_heat_init_xyz(struct dim_heat *dim,char xyz)
{
	if (xyz=='x')
	{
		dim->x= NULL;
		dim->dx= NULL;
		dim->xlen=-1;
	}else
	if (xyz=='y')
	{
		dim->y= NULL;
		dim->dy= NULL;
		dim->ylen=-1;
	}else
	if (xyz=='z')
	{
		dim->z= NULL;
		dim->dz= NULL;
		dim->zlen=-1;
	}

}

void dim_heat_init(struct dim_heat *dim)
{
	dim_heat_init_xyz(dim,'x');
	dim_heat_init_xyz(dim,'y');
	dim_heat_init_xyz(dim,'z');
}

void dim_heat_free_xyz(struct dim_heat *dim,char xyz)
{
	if (xyz=='x')
	{
		if (dim->x!=NULL)
		{
			free(dim->x);
			free(dim->dx);
			dim_heat_init_xyz(dim,'x');
		}
	}else
	if (xyz=='y')
	{
		if (dim->y!=NULL)
		{
			free(dim->y);
			free(dim->dy);
			dim_heat_init_xyz(dim,'y');
		}
	}else
	if (xyz=='z')
	{
		if (dim->z!=NULL)
		{
			free(dim->z);
			free(dim->dz);
			dim_heat_init_xyz(dim,'z');
		}
	}

}

void dim_heat_free(struct dim_heat *dim)
{
	dim_heat_free_xyz(dim,'x');
	dim_heat_free_xyz(dim,'y');
	dim_heat_free_xyz(dim,'z');
	dim_heat_init(dim);
}

void dim_heat_malloc_xyz(struct dim_heat *dim,char xyz)
{

	if (xyz=='x')
	{
		dim->x = (long double *) malloc(dim->xlen * sizeof(long double));
		memset(dim->x, 0, dim->xlen * sizeof(long double));

		dim->dx = (long double *) malloc(dim->xlen * sizeof(long double));
		memset(dim->dx, 0, dim->xlen * sizeof(long double));

	}else
	if (xyz=='y')
	{
		dim->y = (long double *) malloc(dim->ylen * sizeof(long double));
		memset(dim->y, 0, dim->ylen * sizeof(long double));

		dim->dy = (long double *) malloc(dim->ylen * sizeof(long double));
		memset(dim->dy, 0, dim->ylen * sizeof(long double));
	}else
	if (xyz=='z')
	{
		dim->z = (long double *) malloc(dim->zlen * sizeof(long double));
		memset(dim->z, 0, dim->zlen * sizeof(long double));

		dim->dz = (long double *) malloc(dim->zlen * sizeof(long double));
		memset(dim->dz, 0, dim->zlen * sizeof(long double));
	}

}



void dim_heat_malloc(struct dim_heat *dim)
{
	dim_heat_malloc_xyz(dim,'x');
	dim_heat_malloc_xyz(dim,'y');
	dim_heat_malloc_xyz(dim,'z');
}


void dim_heat_info_to_buf(struct dat_file *buf,struct dim_heat *dim)
{
	long double mul_x=0.0;
	long double mul_y=0.0;
	long double mul_z=0.0;

	get_meter_dim(buf->x_units,&mul_x,dim->x[dim->xlen-1]);
	get_meter_dim(buf->y_units,&mul_y,dim->y[dim->ylen-1]);
	get_meter_dim(buf->z_units,&mul_z,dim->z[dim->zlen-1]);
	buf->y_mul=mul_y;
	buf->x_mul=mul_x;
	buf->z_mul=mul_z;

	strcpy(buf->x_label,_("x-position"));
	strcpy(buf->y_label,_("y-position"));
	strcpy(buf->z_label,_("z-position"));

	buf->x=dim->xlen;
	buf->y=dim->ylen;
	buf->z=dim->zlen;

	buf->logscale_x=0;
	buf->logscale_y=0;

	if ((dim->xlen>1)&&(dim->ylen>1)&&(dim->zlen>1))
	{
		strcpy(buf->type,"zxy-d");
	}else
	if ((dim->xlen>1)&&(dim->ylen>1))
	{
		strcpy(buf->type,"3d");
	}else
	{
		strcpy(buf->type,"xy");
	}

}
