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

/** @file simplex_init.c
@brief Init the simplex code
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <simplex.h>
#include <gpvdm_const.h>

void multimin_init(struct multimin *data)
{
	data->ittr=0;
	data->n_max=100;
	data->ndim=0;
	data->nsimplex=data->ndim+1;
	data->stop_error=1e-9;
	data->p=NULL;
	data->center=NULL;
	data->y=NULL;
	data->x=NULL;
	data->s=NULL;
	data->error=0.0;
	data->error_delta=1.0;
	data->error_last=1.0;
	data->fn=NULL;
	data->p0=NULL;
	data->p1=NULL;
}

void multimin_malloc(struct multimin *data)
{
	data->x=(double*)malloc(sizeof(double)*data->ndim);
	data->s=(double*)malloc(sizeof(double)*data->ndim);
	data->center=(double*)malloc(sizeof(double)*data->ndim);

	data->nsimplex=data->ndim+1;

	int s=0;
	int d=0;

	data->p=malloc(sizeof(double)*data->nsimplex);

	for (s=0;s<data->nsimplex;s++)
	{
		data->p[s]=malloc(sizeof(double)*data->ndim);
	}

	data->y=malloc(sizeof(double)*data->nsimplex);

	data->ptry=(double*)malloc(sizeof(double)*data->ndim);

	for (d=0;d<data->ndim;d++)
	{
		data->s[d]=0.0;
	}
}

void multimin_init_simplex(struct multimin *data)
{
	int d;
	int s;
	double delta=0.0;
	if (data->s[0]==0)
	{
		printf("Error: data->s[0]==0\n");
		return;
	}
	//data->p=malloc(sizeof(double)*data->nsimplex);

	for (s=0;s<data->nsimplex;s++)
	{

		for (d=0;d<data->ndim;d++)
		{
			delta=0.0;
			if (s==d)
			{
				delta=data->x[d]*data->s[d];
			}

			data->p[s][d]=data->x[d]+delta;
		}
	}


	for (s=0;s<data->nsimplex;s++)
	{

			data->y[s]=data->fn((void *)data,data->p[s]);
	}

	for (d=0;d<data->ndim;d++)
	{
		data->center[d]=-1;
	}

	//multimin_dump(data);
}

void multimin_free(struct multimin *data)
{
	int s;

	free(data->x);
	free(data->s);

	free(data->center);

	for (s=0;s<data->nsimplex;s++)
	{
		free(data->p[s]);
	}
	free(data->p);


	free(data->y);
	free(data->ptry);
}
