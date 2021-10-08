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

