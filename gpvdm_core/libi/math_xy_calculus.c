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

/**Integrate the data
@param in the structure to integrate
*/
long double inter_intergrate(struct math_xy* in)
{
int i;
long double tn=0.0;
long double tp=0.0;
//long double t=0.0;
long double dt=0.0;
//long double Eomega=0.0;
long double sum=0.0;
long double n;

	for (i=0;i<in->len;i++)
	{

		if (i==0)
		{
			tn=in->x[i];
		}else
		{
			tn=in->x[i-1];
		}

		if (i==in->len-1)
		{
			tp=in->x[i];
		}else
		{
			tp=in->x[i+1];
		}

		n=in->data[i];
		dt=fabs((tp-tn)/2.0);

		sum+=n*dt;


	}
return sum;
}

/**Integrate the data between limits
@param in the structure to integrate
@param from lower limit
@param from upper limit
*/
long double inter_intergrate_lim(struct math_xy* in,long double from, long double to)
{
int i;
long double tn=0.0;
long double tp=0.0;
//long double t=0.0;
long double dt=0.0;
//long double Eomega=0.0;
long double sum=0.0;
long double n;

	for (i=0;i<in->len;i++)
	{

		if (i==0)
		{
			tn=in->x[i];
		}else
		{
			tn=in->x[i-1];
		}

		if (i==in->len-1)
		{
			tp=in->x[i];
		}else
		{
			tp=in->x[i+1];
		}

		n=in->data[i];
		dt=fabs((tp-tn)/2.0);

		if (tn>from) sum+=n*dt;
		if (tn>to) break;

	}

return sum;
}



