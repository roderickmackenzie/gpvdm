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




/** @file i_mem.c
	@brief Memory management for i.c
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


static int unused __attribute__((unused));
static char* unused_pchar __attribute__((unused));


/**Initialize a 1D math_xy
@param in input math_xy
*/
void inter_init(struct simulation *sim,struct math_xy* in)
{
	in->len=-1;
	in->max_len=-1;
	in->data=NULL;
	in->imag=NULL;
	in->complex_enabled=FALSE;
	in->x=NULL;
}


/**Make a copy of one math_xy
@param in input math_xy
@param output math_xy
@param alloc initialize the memory in the output math_xy
*/
void inter_copy(struct math_xy* out,struct math_xy* orig,int alloc)
{
	int i;

	if (alloc==TRUE)
	{
		inter_malloc(out,orig->len);
	}

	out->len=orig->len;

	for  (i=0;i<orig->len;i++)
	{
		out->x[i]=orig->x[i];
		out->data[i]=orig->data[i];
	}


}

/**Free the structure holding the data
@param in The structure holding the data
*/
void inter_free(struct math_xy* in)
{
	in->len=-1;
	in->max_len=-1;

	if (in->x!=NULL)
	{
		free(in->x);
		in->x=NULL;
	}

	if (in->data!=NULL)
	{
		free(in->data);
		in->data=NULL;
	}

	if (in->imag!=NULL)
	{
		free(in->imag);
		in->imag=FALSE;
	}

}


void inter_append(struct math_xy* in,long double x,long double y)
{
	if (in->x==NULL)
	{
		inter_malloc(in,100);
	}

	in->x[in->len]=x;
	in->data[in->len]=y;
	in->len++;

	if ((in->max_len-in->len)<10)
	{
		in->max_len+=100;
		inter_realloc(in,in->max_len);
	}

}


/**Change the size of an allocated math_xy
@param in inout math_xy
@param len new length
*/
void inter_realloc(struct math_xy* in,int len)
{
	in->x=(long double *)realloc (in->x,len*sizeof(long double));
	in->data=(long double *)realloc (in->data,len*sizeof(long double));
}


/**Allocate math_xy as a 1D array
@param in the array to allocate
@param m number of coloums
@param len length of data to store in the array
*/
void inter_malloc(struct math_xy* in,int len)
{
	if (in->x!=NULL)
	{
		printf("inter_malloc error in->x\n");
		getchar();
	}

	if (in->data!=NULL)
	{
		printf("inter_malloc error in->data\n");
		getchar();
	}

	in->len=0;
	in->max_len=len;
	if (in->max_len>0)
	{
		in->x=(long double *)malloc (in->max_len*sizeof(long double));
		in->data=(long double *)malloc (in->max_len*sizeof(long double));
		memset(in->x, 0, in->max_len*sizeof(long double));
		memset(in->data, 0, in->max_len*sizeof(long double));

		if (in->complex_enabled==TRUE)
		{
			in->imag=(long double *)malloc (in->max_len*sizeof(long double));
			memset(in->imag, 0, in->max_len*sizeof(long double));
		}

		strcpy(in->name,"new");
	}
}

void inter_init_mesh(struct math_xy* in,int len,long double min,long double max)
{
int i;
inter_malloc(in,len);
in->len=len;
memset(in->data, 0, in->len*sizeof(long double));
long double pos=min;
long double dx=(max-min)/((long double)in->len);

for (i=0;i<in->len;i++)
{
	in->x[i]=pos;
	pos+=dx;
}

}
