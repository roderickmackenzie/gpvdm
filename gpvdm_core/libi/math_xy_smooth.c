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
void math_xy_smooth(struct simulation *sim,struct math_xy* out,struct math_xy* in,int points)
{
int i=0;
int ii=0;
int pos=0;
long double tot_point=0.0;
long double tot=0;
struct math_xy store;
inter_init(sim,&store);

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


