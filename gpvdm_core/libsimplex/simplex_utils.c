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
