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

/** @file dim.c
@brief Dimension object
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

void dim_light_init_xyzl(struct dim_light *dim,char xyzl)
{
	if (xyzl=='x')
	{
		dim->x= NULL;
		dim->dx= -1.0;
		dim->xlen=-1;
	}else
	if (xyzl=='y')
	{
		dim->y= NULL;
		dim->dy= -1.0;
		dim->ylen=-1;
	}else
	if (xyzl=='z')
	{
		dim->z= NULL;
		dim->dz= -1.0;
		dim->zlen=-1;
	}else
	if (xyzl=='l')
	{
		dim->l= NULL;
		dim->dl= -1.0;
		dim->llen=-1;
	}

}

void dim_light_init(struct dim_light *dim)
{
	dim_light_init_xyzl(dim,'x');
	dim_light_init_xyzl(dim,'y');
	dim_light_init_xyzl(dim,'z');
	dim_light_init_xyzl(dim,'l');
}

void dim_light_free_xyzl(struct dim_light *dim,char xyzl)
{
	if (xyzl=='x')
	{
		if (dim->x!=NULL)
		{
			free(dim->x);
			dim_light_init_xyzl(dim,'x');
		}
	}else
	if (xyzl=='y')
	{
		if (dim->y!=NULL)
		{
			free(dim->y);
			dim_light_init_xyzl(dim,'y');
		}
	}else
	if (xyzl=='z')
	{
		if (dim->z!=NULL)
		{
			free(dim->z);
			dim_light_init_xyzl(dim,'z');
		}
	}else
	if (xyzl=='l')
	{
		if (dim->l!=NULL)
		{
			free(dim->l);
			dim_light_init_xyzl(dim,'l');
		}
	}

}

void dim_light_free(struct dim_light *dim)
{
	dim_light_free_xyzl(dim,'x');
	dim_light_free_xyzl(dim,'y');
	dim_light_free_xyzl(dim,'z');
	dim_light_free_xyzl(dim,'l');
	dim_light_init(dim);
}

void dim_light_cpy(struct dim_light *out,struct dim_light *in)
{
	dim_light_free(out);
	out->xlen=in->xlen;
	out->dx=in->dx;

	out->ylen=in->ylen;
	out->dy=in->dy;

	out->zlen=in->zlen;
	out->dz=in->dz;

	out->llen=in->llen;
	out->dl=in->dl;

	dim_light_malloc(out);
	if (in->x!=NULL)
	{
		memcpy(out->x, in->x, in->xlen * sizeof(long double));
		memcpy(out->y, in->y, in->ylen * sizeof(long double));
		memcpy(out->z, in->z, in->zlen * sizeof(long double));
		memcpy(out->l, in->l, in->llen * sizeof(long double));
	}
}

void dim_light_malloc_xyzl(struct dim_light *dim,char xyzl)
{

	if (xyzl=='x')
	{
		if (dim->xlen>0)
		{
			dim->x = (long double *) malloc(dim->xlen * sizeof(long double));
			memset(dim->x, 0, dim->xlen * sizeof(long double));
		}
	}else
	if (xyzl=='y')
	{
		if (dim->ylen>0)
		{
			dim->y = (long double *) malloc(dim->ylen * sizeof(long double));
			memset(dim->y, 0, dim->ylen * sizeof(long double));
		}
	}else
	if (xyzl=='z')
	{
		if (dim->zlen>0)
		{
			dim->z = (long double *) malloc(dim->zlen * sizeof(long double));
			memset(dim->z, 0, dim->zlen * sizeof(long double));
		}
	}else
	if (xyzl=='l')
	{
		if (dim->llen>0)
		{
			dim->l = (long double *) malloc(dim->llen * sizeof(long double));
			memset(dim->l, 0, dim->llen * sizeof(long double));
		}
	}


}



void dim_light_malloc(struct dim_light *dim)
{
	dim_light_malloc_xyzl(dim,'x');
	dim_light_malloc_xyzl(dim,'y');
	dim_light_malloc_xyzl(dim,'z');
	dim_light_malloc_xyzl(dim,'l');

}


void dim_light_info_to_buf(struct dat_file *buf,struct dim_light *dim)
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

}
