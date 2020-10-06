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




/** @file math_xy_smooth.c
	@brief Function to smooth data
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

/**Smooth math_xy with a window
@param points input math_xy
*/
void math_xy_smooth_range(struct math_xy* out,struct math_xy* in,int points,long double x)
{
int i=0;
int ii=0;
int pos=0;
long double tot_point=0.0;
long double tot=0;
	for (i=0;i<in->len;i++)
	{
		for (ii= -points;ii<points+1;ii++)
		{

			pos=i+ii;

			if ((pos<in->len)&&(pos>=0))
			{
				tot+=in->data[pos];//*dx;
				tot_point+=1.0;//dx;
			}
		}

		if (in->x[i]>x)
		{
			out->data[i]=(tot/(long double)tot_point);
		}else
		{
			out->data[i]=in->data[i];
		}
		tot=0.0;
		tot_point=0.0;
	}
}

/**Smooth math_xy with a window
@param points input math_xy
*/
void math_xy_smooth(struct math_xy* out,struct math_xy* in,int points)
{
int i=0;
int ii=0;
int pos=0;
long double tot_point=0.0;
long double tot=0;
struct math_xy store;

if (out==NULL)
{
	inter_copy(&store,in,TRUE);
}

	for (i=0;i<in->len;i++)
	{
		for (ii= -points;ii<points+1;ii++)
		{

			pos=i+ii;

			if ((pos<in->len)&&(pos>=0))
			{
				tot+=in->data[pos];//*dx;
				tot_point+=1.0;//dx;
			}
		}

		if (out==NULL)
		{
			store.data[i]=(tot/(long double)tot_point);
		}else
		{
			out->data[i]=(tot/(long double)tot_point);
		}

		tot=0.0;
		tot_point=0.0;
	}

	if (out==NULL)
	{
		inter_copy(in,&store,FALSE);
		inter_free(&store);
	}
}


