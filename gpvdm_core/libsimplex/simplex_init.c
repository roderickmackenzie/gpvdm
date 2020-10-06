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
	data->fn=NULL;
	data->x=NULL;
	data->s=NULL;
	data->error=0.0;
	data->error_delta=1.0;
	data->error_last=1.0;

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

			data->y[s]=data->fn(data->p[s],data->ndim);
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
