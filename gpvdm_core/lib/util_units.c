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

/** @file util_units.c
	@brief Utility functions.
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include "util.h"
#include "log.h"
#include <gpvdm_const.h>
#include <lang.h>
#include <math.h>
#include <ctype.h>
#include <cal_path.h>

static char* unused_pchar __attribute__((unused));

void get_wavelength_dim(char *unit,long double *mul,long double max_val)
{

if (max_val<1e-6)
{
	strcpy(unit,"nm");
	*mul=1e9;
}else
if (max_val<1e-3)
{
	strcpy(unit,"um");
	*mul=1e6;
}else
if (max_val<1e-1)
{
	strcpy(unit,"mm");
	*mul=1e3;
}else
{
	strcpy(unit,"m");
	*mul=1.0;
}

}

void get_meter_dim(char *unit,long double *mul,long double max_val)
{

if (max_val<1e-6)
{
	strcpy(unit,"nm");
	*mul=1e9;
}else
if (max_val<1e-3)
{
	strcpy(unit,"um");
	*mul=1e6;
}else
if (max_val<1e-1)
{
	strcpy(unit,"mm");
	*mul=1e3;
}else
{
	strcpy(unit,"m");
	*mul=1.0;
}

}

void get_time_dim(char *unit,long double *mul,long double max_val)
{

if (max_val<1e-15)
{
	strcpy(unit,"as");
	*mul=1e18;
}else
if (max_val<1e-12)
{
	strcpy(unit,"fs");
	*mul=1e15;
}else
if (max_val<1e-9)
{
	strcpy(unit,"ps");
	*mul=1e12;
}else
if (max_val<1e-6)
{
	strcpy(unit,"ns");
	*mul=1e9;
}else
if (max_val<1e-3)
{
	strcpy(unit,"us");
	*mul=1e6;
}else
if (max_val<1e-1)
{
	strcpy(unit,"ms");
	*mul=1e3;
}else
{
	strcpy(unit,"s");
	*mul=1.0;
}

}

void fx_with_units(char *out,double number)
{
	if (number<1e3)
	{
		sprintf(out,"%.3lf Hz",number);
	}
	else
	if (number<1e6)
	{
		sprintf(out,"%.3lf KHz",number*1e-3);
	}
	else
	if (number<1e9)
	{
		sprintf(out,"%.3lf MHz",number*1e-6);
	}
	else
	if (number<1e12)
	{
		sprintf(out,"%.3lf GHz",number*1e-9);
	}

}

void time_with_units(char *out,double number)
{
	char unit[100];
	long double mul;
	long double val=fabs(number);
	get_time_dim(unit,&mul,val);
	sprintf(out,"%.3Lf %s",number*mul,unit);

}

