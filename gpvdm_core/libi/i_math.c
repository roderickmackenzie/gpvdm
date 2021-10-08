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




/** @file i_mem.c
	@brief Memory management for i.c
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


static int unused __attribute__((unused));
static char* unused_pchar __attribute__((unused));


/**Norm math_xy to max value
@param in input math_xy
*/
long double inter_norm(struct math_xy* in,long double mul)
{
int i;
long double max=in->data[0];
//if (in->len>0) max=in->data[0];
for (i=0;i<in->len;i++)
{
if (in->data[i]>max) max=in->data[i];
}

for (i=0;i<in->len;i++)
{
in->data[i]*=mul/max;
}

return max;
}

/**Add together math_xy structures
@param out output structure
@param in structure to add
*/
void inter_add(struct math_xy* out,struct math_xy* in)
{
int i;
	for (i=0;i<out->len;i++)
	{
		out->data[i]+=in->data[i];

		if (out->complex_enabled==TRUE)
		{
			out->imag[i]+=in->imag[i];
		}
	}

}

/**Get the average value of the data in a 1D math_xy
@param in input math_xy
*/
long double inter_avg(struct math_xy* in)
{
int i;
long double sum=0.0;

for (i=0;i<in->len;i++)
{
	sum+=in->data[i];
}
return sum/((long double)(in->len));
}

/**Multiply the data in a 1D math_xy by a number
@param in input math_xy
*/
void math_xy_mul_long_double(struct math_xy* in,long double mul)
{
int i;
//long double sum=0.0;

for (i=0;i<in->len;i++)
{
	in->data[i]*=mul;

	if (in->complex_enabled==TRUE)
	{
		in->imag[i]*=mul;
	}
}
}

/**Get maximum value of an math_xy
@param in input math_xy
*/
long double inter_get_fabs_max(struct math_xy* in)
{
int i;
long double max=fabs(in->data[0]);
//if (in->len>0) max=in->data[0];
for (i=0;i<in->len;i++)
{
if (fabs(in->data[i])>max) max=fabs(in->data[i]);
}

return max;
}

/**Make all the data positive
@param in the structure holding the data
*/
void inter_mod(struct math_xy* in)
{
int i;
for  (i=0;i<in->len;i++)
{
if (in->data[i]<0.0) in->data[i]*= -1.0;
}

}

/**Raise the data in an math_xy by a power
@param p power to raise the data by
*/
void inter_pow(struct math_xy* in,long double p)
{
int i;
for  (i=0;i<in->len;i++)
{
in->data[i]=pow(in->data[i],p);
}

}

/**Subtract two arrays they must be of the same length/x-asis
@param in opperand one, then result
@param in opperand two

*/
void inter_sub(struct simulation *sim,struct math_xy* one,struct math_xy* two)
{
if (one->len!=two->len)
{
	printf_log(sim,"The arrays are not the same length\n");
	exit(0);
}

int i;
for  (i=0;i<one->len;i++)
{
	if (one->x[i]!=two->x[i])
	{
		printf_log(sim,"The arrays do not have the same x axis\n");
		exit(0);
	}
	one->data[i]-=two->data[i];

	if (one->complex_enabled==TRUE)
	{
		one->imag[i]-=two->imag[i];
	}
}

}

/**Divide one array by the other they must be of the same length/x-asis
@param in opperand one, then result
@param in opperand two

*/
void math_xy_div(struct simulation *sim,struct math_xy* one,struct math_xy* two)
{
int i;
long double a;
long double b;
long double c;
long double d;

if (one->len!=two->len)
{
	printf_log(sim,"The arrays are not the same length\n");
	exit(0);
}


for  (i=0;i<one->len;i++)
{
	if (one->x[i]!=two->x[i])
	{
		printf_log(sim,"The arrays do not have the same x axis\n");
		exit(0);
	}

	if (one->complex_enabled==TRUE)
	{
		a=one->data[i];
		b=one->imag[i];

		c=two->data[i];
		d=two->imag[i];

		one->data[i]=(a*c+b*d)/(c*c+d*d);
		one->imag[i]=(b*c-a*d)/(c*c+d*d);
	}else
	{
		if (two->data[i]!=0)
		{
			one->data[i]/=two->data[i];
		}
	}
}

}
