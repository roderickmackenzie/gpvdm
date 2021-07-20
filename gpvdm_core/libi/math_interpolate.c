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
