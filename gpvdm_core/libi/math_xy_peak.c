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




/** @file i.c
	@brief Simple functions to read in scientific data from text files and perform simple maths on the data.
*/
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sim_struct.h>

#include "i.h"
#include "util.h"
#include "cal_path.h"
#include "gpvdm_const.h"
#include <log.h>
#include "inp.h"
#include <memory.h>


static int unused __attribute__((unused));
static char* unused_pchar __attribute__((unused));


long double inter_get_center_of_peak(struct math_xy* in,int i,int window)
{
int delta=window/2;

if ((i-delta<0)||(i+delta>in->len-1))
{
return in->x[i];
}
long double top=0.0;
long double btm=0.0;
int ii;
for (ii=i-delta;ii<i+delta;ii++)
{
top+=in->x[ii]*in->data[ii];
btm+=in->data[ii];
}
return top/btm;
}

/**Find the peaks
@param real real array
@param imag imag array
@param in input data
@param fx frequency
*/
void inter_find_peaks(struct math_xy* out,struct math_xy* in,int find_max)
{
int i=0;
int ii=0;
int window=2;
long double yn=0.0;
long double yc=0.0;
long double yp=0.0;
int min=0;
int max=0;
int grad_l=0;
int grad_r=0;
for (i=0;i<in->len;i++)
{
		for (ii=1;ii<=window;ii++)
		{
			if ((i-ii)>=0 && ((i-ii)<in->len)) yn=in->data[i-ii];

			if ((i+ii)>=0 &&((i+ii)<in->len)) yp=in->data[i+ii];

			yc=in->data[i];

			if ((yc-yn)<0.0)
			{
				grad_l= -1;
			}else
			{
				grad_l=1;
			}

			if ((yp-yc)<0.0)
			{
				grad_r= -1;
			}else
			{
				grad_r=1;
			}

			if ((grad_l== -1) && (grad_r==1))
			{
				min++;
			}else
			if ((grad_l==1) && (grad_r== -1))
			{
				max++;
			}
		}

		if (min==window)
		{
			if (find_max==FALSE) inter_append(out,inter_get_center_of_peak(in,i,21),in->data[i]);

		}else
		if (max==window)
		{
			if (find_max==TRUE) inter_append(out,inter_get_center_of_peak(in,i,21),in->data[i]);
		}
		min=0;
		max=0;
	}
}


/**Get position of quartile
@param in input structure
@param value Value to find
*/
long double inter_get_quartile(struct math_xy* in,long double value)
{
int i;
long double sum=0.0;
long double sum2=0.0;
long double dl=0.0;
long double dr=0.0;
long double dx=0.0;

sum=inter_intergrate(in);


for (i=0;i<in->len;i++)
{
	if (i==0)
	{
		dl=in->x[0];
	}else
	{
		dl=in->x[i-1];
	}

	if (i==in->len-1)
	{
		dr=in->x[in->len-1];
	}else
	{
		dr=in->x[i+1];
	}

	dx=dr-dl;
	sum2+=dx*in->data[i];
	if (sum2>(sum*value)) return in->x[i];
}

return 0.0;
}



long double inter_array_get_max(long double *data,int len)
{
int i;
long double max=data[0];
for (i=0;i<len;i++)
{
if (max<data[i]) max=data[i];
}
return max;
}

void math_xy_get_left_right_start(struct math_xy* in,int *left,int *right, long double fraction)
{
int x;
long double sum=0.0;
long double sum_l=0.0;
long double sum_r=0.0;
long double dl=0.0;
long double dr=0.0;
long double dx=0.0;

sum=inter_intergrate(in);

for (x=0;x<in->len;x++)
{
	if (x==0)
	{
		dl=in->x[0];
	}else
	{
		dl=in->x[x-1];
	}

	if (x==in->len-1)
	{
		dr=in->x[in->len-1];
	}else
	{
		dr=in->x[x+1];
	}

	dx=dr-dl;
	sum_l+=dx*in->data[x];

	if (sum_l>(sum*fraction))
	{
		*left=x;
		break;
	}
}

for (x=in->len-1;x>0;x--)
{
	if (x==0)
	{
		dl=in->x[0];
	}else
	{
		dl=in->x[x-1];
	}

	if (x==in->len-1)
	{
		dr=in->x[in->len-1];
	}else
	{
		dr=in->x[x+1];
	}

	dx=dr-dl;
	sum_r+=dx*in->data[x];

	if (sum_r>(sum*fraction))
	{
		*right=x;
		break;
	}
}

return;
}


