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
#include <device_fun.h>
#include <math.h>

void dim_init_xyz(struct dimensions *dim,char xyz)
{
	if (xyz=='x')
	{
		dim->x= NULL;
		dim->dX= NULL;
		dim->xlen=-1;
	}else
	if (xyz=='y')
	{
		dim->y= NULL;
		dim->dY= NULL;
		dim->ylen=-1;
	}else
	if (xyz=='z')
	{
		dim->z= NULL;
		dim->dZ= NULL;
		dim->zlen=-1;
	}

}

void dim_init(struct dimensions *dim)
{
	dim_init_xyz(dim,'x');
	dim_init_xyz(dim,'y');
	dim_init_xyz(dim,'z');
	dim->srh_bands=-1;
}

void dim_free_xyz(struct dimensions *dim,char xyz)
{
	if (xyz=='x')
	{
		if (dim->x!=NULL)
		{
			free(dim->x);
			free(dim->dX);
			dim_init_xyz(dim,'x');
		}
	}else
	if (xyz=='y')
	{
		//printf("free y: %p",dim->y);
		if (dim->y!=NULL)
		{
			free(dim->y);
			free(dim->dY);
			dim_init_xyz(dim,'y');
		}
	}else
	if (xyz=='z')
	{
		if (dim->z!=NULL)
		{
			free(dim->z);
			free(dim->dZ);
			dim_init_xyz(dim,'z');
		}
	}

}

void dim_free(struct dimensions *dim)
{
	dim_free_xyz(dim,'x');
	dim_free_xyz(dim,'y');
	dim_free_xyz(dim,'z');
	dim_init(dim);
}

void dim_malloc_xyz(struct dimensions *dim,char xyz)
{

	if (xyz=='x')
	{
		if (dim->xlen>0)
		{
			dim->x = (gdouble *) malloc(dim->xlen * sizeof(gdouble));
			memset(dim->x, 0, dim->xlen * sizeof(gdouble));

			dim->dX = (gdouble *) malloc(dim->xlen * sizeof(gdouble));
			memset(dim->dX, 0, dim->xlen * sizeof(gdouble));
		}
	}else
	if (xyz=='y')
	{
		if (dim->ylen>0)
		{
			dim->y = (gdouble *) malloc(dim->ylen * sizeof(gdouble));
			memset(dim->y, 0, dim->ylen * sizeof(gdouble));

			dim->dY = (gdouble *) malloc(dim->ylen * sizeof(gdouble));
			memset(dim->dY, 0, dim->ylen * sizeof(gdouble));
		}
	}else
	if (xyz=='z')
	{
		if (dim->zlen>0)
		{
			dim->z = (gdouble *) malloc(dim->zlen * sizeof(gdouble));
			memset(dim->z, 0, dim->zlen * sizeof(gdouble));

			dim->dZ = (gdouble *) malloc(dim->zlen * sizeof(gdouble));
			memset(dim->dZ, 0, dim->zlen * sizeof(gdouble));
		}
	}


}

void dim_set_simple_mesh_x(struct dimensions *dim, long double start, long double stop)
{

	int x=0;
	long double xpos=start;
	long double dx=(stop-start)/dim->xlen;

	for (x=0;x<dim->xlen;x++)
	{
		dim->x[x]=xpos;
		xpos+=dx;
	}
}

void dim_set_simple_mesh_z(struct dimensions *dim, long double start, long double stop)
{
	int z=0;
	long double zpos=start;
	long double dz=(stop-start)/dim->zlen;

	for (z=0;z<dim->zlen;z++)
	{
		dim->z[z]=zpos;
		zpos+=dz;
	}
}

void dim_malloc(struct dimensions *dim)
{
	dim_malloc_xyz(dim,'x');
	dim_malloc_xyz(dim,'y');
	dim_malloc_xyz(dim,'z');

}

void dim_swap(struct dimensions *out,struct dimensions *in)
{
	struct dimensions tmp;
	dim_init(&tmp);
	dim_cpy(&tmp,out);
	dim_cpy(out,in);
	dim_cpy(in,&tmp);
	dim_free(&tmp);

}

void dim_cpy(struct dimensions *out,struct dimensions *in)
{

	int x=0;
	int y=0;
	int z=0;

	dim_free(out);
	out->xlen=in->xlen;
	out->ylen=in->ylen;
	out->zlen=in->zlen;

	dim_malloc(out);

	for (x=0;x<out->xlen;x++)
	{
		out->x[x]=in->x[x];
		out->dX[x]=in->dX[x];
	}

	for (y=0;y<out->ylen;y++)
	{
		out->y[y]=in->y[y];
		out->dY[y]=in->dY[y];
	}

	for (z=0;z<out->zlen;z++)
	{
		out->z[z]=in->z[z];
		out->dZ[z]=in->dZ[z];
	}

	out->srh_bands=in->srh_bands;
}

void dim_info_to_buf(struct dat_file *buf,struct dimensions *dim)
{
	long double mul_x=0.0;
	long double mul_y=0.0;
	long double mul_z=0.0;

	get_meter_dim(buf->x_units,&mul_x,dim->x[dim->xlen-1]);
	get_meter_dim(buf->y_units,&mul_y,dim->y[dim->ylen-1]);
	get_meter_dim(buf->z_units,&mul_z,dim->z[dim->zlen-1]);

	buf->x=dim->xlen;
	buf->y=dim->ylen;
	buf->z=dim->zlen;

	if (dim->zlen==1)
	{
		buf->z_offset=dim->z[0];
	}

	if (dim->xlen==1)
	{
		buf->x_offset=dim->x[0];
	}

	if (dim->ylen==1)
	{
		buf->y_offset=dim->y[0];
	}

	buf->y_mul=mul_y;
	buf->x_mul=mul_x;
	buf->z_mul=mul_z;

	strcpy(buf->x_label,_("x-position"));
	strcpy(buf->y_label,_("y-position"));
	strcpy(buf->z_label,_("z-position"));

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
	if ((dim->zlen>1)&&(dim->xlen>1))
	{
		strcpy(buf->type,"3d");
	}else
	if ((dim->zlen>1)&&(dim->ylen>1))
	{
		strcpy(buf->type,"3d");
	}else
	{
		strcpy(buf->type,"xy");
	}
}



long double dim_dl_xyz(struct dimensions *dim, int x0, int y0, int z0,int x1,int y1, int z1)
{
	long double pos_x0=dim->x[x0];
	long double pos_y0=dim->y[y0];
	long double pos_z0=dim->z[z0];

	long double pos_x1=dim->x[x1];
	long double pos_y1=dim->y[y1];
	long double pos_z1=dim->z[z1];

	return sqrtl(powl(pos_x0-pos_x1,2.0)+powl(pos_y0-pos_y1,2.0)+powl(pos_z0-pos_z1,2.0));
}


