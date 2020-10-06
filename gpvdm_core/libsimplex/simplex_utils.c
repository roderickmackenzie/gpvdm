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

/** @file simplex_utils.c
@brief Simplex utils
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <simplex.h>

void multimin_cal_center(struct multimin *data)
{
	#ifdef simplex_verbose
	printf("center\n");
	#endif
	//getchar();
	int s;
	int d;
	for (d=0;d<data->ndim;d++)
	{
		data->center[d]=0.0;
		for (s=0;s<data->nsimplex;s++)
		{
			if (s!=data->i_hi0)
			{
				data->center[d]+=data->p[s][d];
			}
		}

		data->center[d]/=(double)(data->nsimplex-1);
	}

}


void multimin_find_best(struct multimin *data)
{
	int s=0;
	int ss=0;
	
	int *ptrs=malloc(sizeof(int)*data->nsimplex);
	
	for (s=0;s<data->nsimplex;s++)
	{
		ptrs[s]=s;
	}

	int temp=0;
	int i=0;
	int swaped=1;
	while(swaped==1)
	{
		swaped=0;
		for (s=0;s<data->nsimplex-1;s++)
		{
			for (ss=0;ss<data->nsimplex;ss++)
			{
				if (data->y[ptrs[s]] < data->y[ptrs[s+1]])
				{
					temp=ptrs[s];
					ptrs[s]=ptrs[s+1];
					ptrs[s+1]=temp;
					swaped=1;
				}
			}
		}
	}
	//for (s=0;s<data->nsimplex;s++)
	//{
	//	printf("<%f> \n",data->y[ptrs[s]]);
	//}

	data->i_hi0=ptrs[0];
	data->i_hi1=ptrs[1];
	data->i_lo=ptrs[data->nsimplex-1];
	free(ptrs);
	
}

void multimin_sync(struct multimin *data,int s)
{
	int d=0;

	for (d=0;d<data->ndim;d++)
	{
		data->p[s][d] = data->ptry[d];
	}

	data->y[s]=data->ytry;
}

void simplex_copy_ans(struct multimin *data)
{
	int d;
	multimin_find_best(data);
	for (d=0;d<data->ndim;d++)
	{
		data->x[d]=data->p[data->i_lo][d];
	}

	data->error=data->y[data->i_lo];
}
/*
void swap(double *a,double *b)
{
	double temp=0;

	temp=*a;
	*a=*b;
	*b=temp;
}*/
