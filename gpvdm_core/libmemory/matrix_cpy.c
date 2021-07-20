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

/** @file matrix.c
@brief A struct for the matrix solver
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
#include "md5.h"
#include "cal_path.h"
#include <timer.h>

void matrix_cpy(struct simulation *sim,struct matrix *out,struct matrix *in)
{
	int y;
	//printf("matrix copy\n");
	//getchar();
	out->nz=in->nz;
	out->nz_max=in->nz_max;
	out->M=in->M;
	out->complex_matrix=in->complex_matrix;
	out->build_from_non_sparse=in->build_from_non_sparse;
	out->msort_len=in->msort_len;
	out->use_cache=in->use_cache;
	out->ittr=in->ittr;
	strcpy(out->hash,in->hash);
	strcpy(out->cache_file_path,in->cache_file_path);

	if (in->nz_max==0)
	{
		cpy_1d_alloc((void **)&(out->Ti), (void **)&(in->Ti), in->nz, sizeof(int));
		cpy_1d_alloc((void **)&(out->Tj), (void **)&(in->Tj), in->nz, sizeof(int));
		cpy_1d_alloc((void **)&(out->Tx), (void **)&(in->Tx), in->nz, sizeof(long double));
	}else
	{
		out->nz_max=in->nz_max;
		cpy_1d_alloc((void **)&(out->Ti), (void **)&(in->Ti), in->nz_max, sizeof(int));
		cpy_1d_alloc((void **)&(out->Tj), (void **)&(in->Tj), in->nz_max, sizeof(int));
		cpy_1d_alloc((void **)&(out->Tx), (void **)&(in->Tx), in->nz_max, sizeof(long double));
	}
	cpy_1d_alloc((void **)&(out->b), (void **)&(in->b), in->M, sizeof(long double));

	if (in->complex_matrix==TRUE)
	{
		cpy_1d_alloc((void **)&(out->Txz), (void **)&(in->Txz), in->nz, sizeof(long double));
		cpy_1d_alloc((void **)&(out->bz), (void **)&(in->bz), in->M, sizeof(long double));
	}

	if (in->build_from_non_sparse==TRUE)
	{
		cpy_1d_alloc((void **)&(out->msort), (void **)&(in->msort), in->msort_len, sizeof(struct matrix_sort));
	}



}
