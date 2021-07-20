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

/** @file lib.c
	@brief Call the UMFPACK solver.
*/

#include <enabled_libs.h>
#include <stdio.h>
#include <stdlib.h>
#include <umfpack.h>

#include <util.h>

#include <matrix_solver_memory.h>

	pthread_mutex_t lock;

void error_report(int status, const char *file, const char *func, int line)
{
	fprintf(stderr, "in %s: file %s, line %d: ", func, file, line);

	switch (status) {
		case UMFPACK_ERROR_out_of_memory:
			fprintf(stderr, "out of memory!\n");
			break;
		case UMFPACK_WARNING_singular_matrix:
			fprintf(stderr, "matrix is singular!\n");
			break;
		default:
			fprintf(stderr, "UMFPACK error code %d\n", status);
	}
}



int umfpack_solver(struct matrix_solver_memory *msm,int col,int nz,int *Ti,int *Tj, long double *lTx,long double *lb)
{
int i;
void *Symbolic, *Numeric;
int status;


for (i=0;i<col;i++)
{
	msm->b[i]=(double)lb[i];
}

for (i=0;i<nz;i++)
{
	msm->Tx[i]=(double)lTx[i];
}

double Control [UMFPACK_CONTROL],Info [UMFPACK_INFO];

umfpack_di_defaults (Control) ;
Control[UMFPACK_BLOCK_SIZE]=20;
//Control [UMFPACK_STRATEGY]=UMFPACK_STRATEGY_AUTO;//
//Control [UMFPACK_STRATEGY]=UMFPACK_STRATEGY_SYMMETRIC;
//Control [UMFPACK_STRATEGY]=UMFPACK_STRATEGY_UNSYMMETRIC;
//Control [UMFPACK_ORDERING]=UMFPACK_ORDERING_NONE;//UMFPACK_ORDERING_BEST;//UMFPACK_ORDERING_AMD;//UMFPACK_ORDERING_BEST;//
//Control [UMFPACK_PIVOT_TOLERANCE]=0.0001;
//Control[UMFPACK_SINGLETONS]=1;
//Control[UMFPACK_SCALE]=3;
status = umfpack_di_triplet_to_col(col, col, nz, Ti, Tj, msm->Tx, msm->Ap, msm->Ai, msm->Ax, NULL);

if (status != UMFPACK_OK) {
	error_report(status, __FILE__, __func__, __LINE__);
	return EXIT_FAILURE;
}

// symbolic analysis
status = umfpack_di_symbolic(col, col, msm->Ap, msm->Ai, msm->Ax, &Symbolic, Control, Info);

if (status != UMFPACK_OK) {
	error_report(status, __FILE__, __func__, __LINE__);
	return EXIT_FAILURE;
}

/*#ifdef windows
	 EnterCriticalSection(&lock);
#else
	pthread_mutex_lock(&lock);
#endif*/
// LU factorization
umfpack_di_numeric(msm->Ap, msm->Ai, msm->Ax, Symbolic, &Numeric, Control, Info);

/*#ifdef windows
	 LeaveCriticalSection(&lock);
#else
	pthread_mutex_unlock(&lock);
#endif*/

if (status != UMFPACK_OK) {
	error_report(status, __FILE__, __func__, __LINE__);
	return EXIT_FAILURE;
}
// solve system

umfpack_di_free_symbolic(&Symbolic);


umfpack_di_solve(UMFPACK_A, msm->Ap, msm->Ai, msm->Ax, msm->x, msm->b, Numeric, Control, Info);


if (status != UMFPACK_OK) {
	error_report(status, __FILE__, __func__, __LINE__);
	return EXIT_FAILURE;
}

umfpack_di_free_numeric(&Numeric);



for (i=0;i<col;i++)
{
lb[i]=(long double)msm->x[i];
}

//memcpy(b, x, col*sizeof(double));
//umfpack_toc(stats);


return 0;
}

void umfpack_solver_free(struct matrix_solver_memory *msm)
{
	
}

