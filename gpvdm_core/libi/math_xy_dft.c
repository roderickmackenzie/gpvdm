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


/**Do a DFT
@param real real array
@param imag imag array
@param in input data
@param fx frequency
*/

void math_xy_dft_full(struct simulation *sim,struct math_xy *fx_data,struct math_xy* data, int start_fx, int stop_fx)
{

	long double r=0.0;
	long double i=0.0;
	int x=0;
	int fx_i=0;
	long double data_len=(long double)data->len;
	long double fx=0.0;
	long double dc=0.0;
	dc=inter_sum(data)/((long double)data->len);

	long double dt=0.0;
	long double tot_len=0.0;
	long double r0;
	long double r1;
	long double i0;
	long double i1;
	for (fx_i=start_fx;fx_i<stop_fx;fx_i++)
	{
		fx=fx_data->x[fx_i];
		printf("ftw %Le\n",fx);
		r=0.0;
		i=0.0;
		tot_len=0.0;

		for (x=0;x<data->len-1;x++)
		{
			//if (x<data->len-1)
			//{
				dt=(data->x[x+1]-data->x[x]);
			//}
			r0=(data->data[x]-dc)*cos(2.0*M_PI*fx*data->x[x]);
			r1=(data->data[x+1]-dc)*cos(2.0*M_PI*fx*data->x[x+1]);
			i0=-(data->data[x]-dc)*sin(2.0*M_PI*fx*data->x[x]);
			i1=-(data->data[x+1]-dc)*sin(2.0*M_PI*fx*data->x[x+1]);
			r+=(r1+r0)*dt/2.0;
			i+=(i1+i0)*dt/2.0;
			tot_len+=dt;
		}

		fx_data->data[fx_i]=r*2.0/tot_len;
		fx_data->imag[fx_i]=i*2.0/tot_len;
		//printf("DFT2 r= %Le im=%Le\n",fx_data->data[fx_i],fx_data->imag[fx_i]);
	}

}

void math_xy_dft(long double *real,long double *imag,struct math_xy* in,long double fx)
{
long double r=0.0;
long double i=0.0;
int x=0;
long double dt=in->x[1]-in->x[0];
long double len=(long double)in->len;

for (x=0;x<in->len;x++)
{
	r+=in->data[x]*cos(2.0*M_PI*fx*in->x[x]);
	i+=-in->data[x]*sin(2.0*M_PI*fx*in->x[x]);
}
*real=r*2.0/len;
*imag=i*2.0/len;
}


void math_xy_dft_extract(long double * dc,long double *real,long double *imag,struct math_xy* in,long double fx)
{
long double r=0.0;
long double i=0.0;
int x=0;
long double dt=in->x[1]-in->x[0];
long double len=(long double)in->len;
*dc=inter_sum(in)/len;

for (x=0;x<in->len;x++)
{
	r+=(in->data[x]-*dc)*cos(2.0*M_PI*fx*in->x[x]);
	i+=-(in->data[x]-*dc)*sin(2.0*M_PI*fx*in->x[x]);
}
*real=r*2.0/len;
*imag=i*2.0/len;
//printf("DFT r= %Le im=%Le\n",*real,*imag);
}



