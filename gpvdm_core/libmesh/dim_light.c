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

void dim_light_init_xyzl(struct dimensions *dim,char xyzl)
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

void dim_light_init(struct dimensions *dim)
{
	dim_light_init_xyzl(dim,'x');
	dim_light_init_xyzl(dim,'y');
	dim_light_init_xyzl(dim,'z');
	dim_light_init_xyzl(dim,'l');
}

void dim_light_free_xyzl(struct dimensions *dim,char xyzl)
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

void dim_light_free(struct dimensions *dim)
{
	dim_light_free_xyzl(dim,'x');
	dim_light_free_xyzl(dim,'y');
	dim_light_free_xyzl(dim,'z');
	dim_light_free_xyzl(dim,'l');
	dim_light_init(dim);
}

void dim_light_cpy(struct dimensions *out,struct dimensions *in)
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

void dim_light_malloc_xyzl(struct dimensions *dim,char xyzl)
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



void dim_light_malloc(struct dimensions *dim)
{
	dim_light_malloc_xyzl(dim,'x');
	dim_light_malloc_xyzl(dim,'y');
	dim_light_malloc_xyzl(dim,'z');
	dim_light_malloc_xyzl(dim,'l');

}



