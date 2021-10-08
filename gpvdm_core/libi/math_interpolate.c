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
#include <enabled_libs.h>
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


long double inter_get_raw(long double *x,long double *data,int len,long double pos)
{
long double x0;
long double x1;
long double y0;
long double y1;

long double ret;
int i=0;

if (pos<x[0])
{

return 0.0;
}


if (pos>=x[len-1])
{
	i=len-1;
	x0=x[i-1];
	x1=x[i];

	y0=data[i-1];
	y1=data[i];

}else
{
	i=search(x,len,pos);
	x0=x[i];
	x1=x[i+1];

	y0=data[i];
	y1=data[i+1];
}
ret=y0+((y1-y0)/(x1-x0))*(pos-x0);
return ret;
}

float math_interpolate_raw_float(long double *x,float *data,int len,long double pos)
{
long double x0;
long double x1;
float y0;
float y1;

float ret;
int i=0;

if (pos<x[0])
{

	return 0.0;
}


if (pos>=x[len-1])
{
	i=len-1;
	x0=x[i-1];
	x1=x[i];

	y0=data[i-1];
	y1=data[i];

}else
{
	i=search(x,len,pos);
	x0=x[i];
	x1=x[i+1];

	y0=data[i];
	y1=data[i+1];
}
ret=y0+((y1-y0)/(x1-x0))*(pos-x0);
return ret;
}

float math_interpolate_raw_double(long double *x,double *data,int len,long double pos)
{
long double x0;
long double x1;
double y0;
double y1;

double ret;
int i=0;

if (pos<x[0])
{

	return 0.0;
}


if (pos>=x[len-1])
{
	i=len-1;
	x0=x[i-1];
	x1=x[i];

	y0=data[i-1];
	y1=data[i];

}else
{
	i=search(x,len,pos);
	x0=x[i];
	x1=x[i+1];

	y0=data[i];
	y1=data[i+1];
}
ret=y0+((y1-y0)/(x1-x0))*(pos-x0);
return ret;
}
