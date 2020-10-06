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



