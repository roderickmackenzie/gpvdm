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

/** @file memory_basic.c
@brief memory functions for 3D arrays
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lang.h>
#include "sim.h"
#include "dump.h"
#include "mesh.h"
#include <math.h>
#include "log.h"
#include <solver_interface.h>
#include "memory.h"


void malloc_light_l_long_double(struct dim_light *dim, long double * (*var))
{
	malloc_1d((void **)var,dim->llen, sizeof(long double ));
	//*var = (long double *) malloc(dim->llen * sizeof(long double ));

}


void free_light_l_long_double(struct dim_light *dim, long double * (*in_var))
{
	free_1d((void **)in_var, sizeof(long double ));
	/*int x=0;
	int y=0;
	int z=0;
	int l=0;

	long double *var=*in_var;
	if (var==NULL)
	{
		return;
	}

	free(var);

	*in_var=NULL;*/

}

void cpy_light_l_long_double(struct dim_light *dim, long double * (*out), long double * (*in))
{
	free_1d((void **)out, sizeof(long double ));
	malloc_1d((void **)out,dim->llen, sizeof(long double ));
	cpy_1d((void **)out,(void **)in, dim->llen , sizeof(long double ));
	/*int x=0;
	int y=0;
	int z=0;
	int l=0;

	long double *var=*in_var;
	if (var==NULL)
	{
		return;
	}

	free(var);

	*in_var=NULL;*/

}

long double intergrate_light_l_long_double(struct dim_light *dim, long double *var)
{
	long double sum;
	struct math_xy m;
	m.x=dim->l;
	m.data=var;
	m.len=dim->llen;
	sum=inter_intergrate(&m);
	return sum;
}
