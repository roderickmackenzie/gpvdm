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
