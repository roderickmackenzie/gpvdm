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


long double inter_get_diff(char *out_path,struct math_xy* one,struct math_xy* two,long double start,long double stop,struct math_xy* mull)
{
FILE *out;
if (out_path!=NULL) out=fopen(out_path,"w");
long double error=0.0;
if (one->x[0]>start) start=one->x[0];

long double points_max=400.0;
long double points=0.0;
long double dx=(stop-start)/points_max;
long double pos=start;
long double etemp=0.0;
do
{

	if (pos>=start)
	{
		etemp=fabs(inter_get_noend(one,pos)-inter_get_noend(two,pos))*inter_get_noend(mull,pos);
		if (out_path!=NULL) fprintf(out,"%Le %Le\n",pos,etemp);
		error+=etemp;
		points+=1.0;
	}

	pos+=dx;
}while(pos<stop);
if (out_path!=NULL) fclose(out);

return error/points;
}

long double math_xy_get_delta(struct math_xy* one,struct math_xy* two)
{

int x;
long double sum=0.0;
for (x=0;x<one->len;x++)
{
	sum+=fabsl(one->data[x]-two->data[x]);
}

sum/=(long double)one->len;

return sum;
}



