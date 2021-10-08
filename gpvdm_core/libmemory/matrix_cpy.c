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
