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

/** @file light_zxy_long_double.c
@brief memory functions for light zxy arrays
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lang.h>
#include "sim.h"
#include "dump.h"
#include "mesh.h"
#include <math.h>
#include "log.h"
#include <solver_interface.h>
#include "memory.h"


void malloc_light_zxy_long_double(struct dimensions *dim, long double * (***var))
{
	malloc_3d((void ****)var,dim->zlen, dim->xlen, dim->ylen, sizeof(long double));

}


void free_light_zxy_long_double(struct dimensions *dim, long double * (***var))
{
	free_3d((void ****)var,dim->zlen, dim->xlen, dim->ylen, sizeof(long double));
}

void cpy_light_zxy_long_double(struct dimensions *dim, long double * (***out), long double * (***in))
{
	free_3d((void ****)out,dim->zlen, dim->xlen, dim->ylen, sizeof(long double));
	malloc_3d((void ****)out,dim->zlen, dim->xlen, dim->ylen, sizeof(long double));
	cpy_3d((void ****)out,(void ****)in,dim->zlen, dim->xlen, dim->ylen,sizeof(long double));
}

void light_zxy_mul_long_double(struct simulation *sim, struct dimensions *dim,long double *** data,long double mul)
{
	int x=0;
	int y=0;
	int z=0;

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				data[z][x][y]*=mul;
			}
		}
	}

}

void flip_light_zxy_long_double_y(struct simulation *sim, struct dimensions *dim,long double *** data)
{
	int x=0;
	int y=0;
	int z=0;


	long double ***temp=NULL;

	malloc_light_zxy_long_double(dim, &temp);


	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				temp[z][x][y]=data[z][x][y];
			}
		}
	}


	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				data[z][x][dim->ylen-y-1]=temp[z][x][y];
			}
		}
	}


	free_light_zxy_long_double(dim, &temp);
}

void memset_light_zxy_long_double(struct dimensions *dim, long double ***data,int val)
{
	int x=0;
	int z=0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			memset(data[z][x], val, dim->ylen * sizeof(long double ));
		}
	}

}

void div_light_zxy_long_double(struct dimensions *dim, long double ***data,long double val)
{
	int x=0;
	int y=0;
	int z=0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				data[z][x][y]/=val;
			}
		}
	}

}

//This shoudl be 3D interpolation but we are assuming the meshes are aligned.
long double interpolate_light_zxy_long_double(struct dimensions *dim, long double ***data,int z, int x, long double y_in)
{
	int y=0;
	long double x0=0.0;
	long double x1=0.0;
	long double y0=0.0;
	long double y1=0.0;

	long double ret;

	if (y_in<dim->y[0])
	{
		return 0.0;
	}else
	if (y_in>=dim->y[dim->ylen-1])
	{
		//printf("here %Le %Le\n",y_in,dim->y[dim->ylen-1]);
		//y=dim->ylen-1;
		//x0=dim->y[y-1];
		//x1=dim->y[y];
		//y0=data[z][x][y-1];
		//y1=data[z][x][y];
		return 0.0;
	}else
	{
		y=search(dim->y,dim->ylen,y_in);
		//printf("%d\n",y);
		x0=dim->y[y];
		x1=dim->y[y+1];

		y0=data[z][x][y];
		y1=data[z][x][y+1];
	}
	ret=y0+((y1-y0)/(x1-x0))*(y_in-x0);

return ret;

}

//This shoudl be 3D interpolation but we are assuming the meshes are aligned.
long double interpolate_light_zxy_long_double_intergral(struct dimensions *dim, long double ***data,int z, int x, long double y_start,long double y_stop)
{
	int y=0;
	int yy=0;
	long double x0=0.0;
	long double x1=0.0;
	long double y0=0.0;
	long double y1=0.0;
	long double sum=0.0;
	long double ret;
	long double dy_tot=0.0;

	if (y_start<dim->y[0])
	{
		return 0.0;
	}else
	if (y_start>=dim->y[dim->ylen-1])
	{
		return 0.0;
	}else
	{
		y=search(dim->y,dim->ylen,y_start);
		if (y+3<dim->ylen)
		{
			//printf("%Le %Le\n",dim->y[y+3],y_stop);
			//getchar();
			if (dim->y[y+3]<y_stop)
			{
				yy=y;
				sum=0.0;
				while(yy<dim->ylen)
				{
					sum+=data[z][x][yy]*dim->dy;
					dy_tot+=dim->dy;
					if (dim->y[yy]>=y_stop)
					{
						break;
					}
					//printf("%Le %Le\n",data[z][x][yy],dim->y[yy]);
					//getchar();

					yy++;
				}
				//printf("%Le %Le %Le %Le\n",sum,y_start,y_stop,dy_tot);
				//getchar();
				return sum/(y_stop-y_start);
			}

		}

		//printf("%d\n",y);
		x0=dim->y[y];
		x1=dim->y[y+1];

		y0=data[z][x][y];
		y1=data[z][x][y+1];

		ret=y0+((y1-y0)/(x1-x0))*(y_start-x0);

		return ret;
	}



return 0.0;

}

