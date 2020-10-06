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

