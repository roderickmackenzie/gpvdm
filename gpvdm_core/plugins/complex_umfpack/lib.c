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
	@brief Call the complex UMFPACK solver.
*/

#include <enabled_libs.h>
#include <stdio.h>
#include <stdlib.h>
#include <lang.h>
#include <umfpack.h>
#include <util.h>
#include <matrix_solver_memory.h>

void complex_error_report(int status, const char *file, const char *func, int line)
{
	fprintf(stderr, "in %s: file %s, line %d: ", func, file, line);

	switch (status) {
		case UMFPACK_ERROR_out_of_memory:
			fprintf(stderr, _("out of memory!\n"));
			break;
		case UMFPACK_WARNING_singular_matrix:
			fprintf(stderr, _("matrix is singular!\n"));
			break;
		default:
			fprintf(stderr, "UMFPACK error code %d\n", status);
	}
}



int complex_umfpack_solver(struct matrix_solver_memory *msm,int c_col,int c_nz,int *c_Ti,int *c_Tj, long double *c_Tx,long double *c_Txz,long double *c_b,long double *c_bz)
{
	int i;
	void *Symbolic, *Numeric;
	int status;
	double *dtemp;
	int *itemp;

	if (msm->c_last_col!=c_col)
	{
		dtemp = realloc(msm->c_x,c_col*sizeof(double));	
		if (dtemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->c_x=dtemp;
		}

		dtemp = realloc(msm->c_xz,c_col*sizeof(double));
		if (dtemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->c_xz=dtemp;
		}

		dtemp = realloc(msm->c_b,c_col*sizeof(double));
		if (dtemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->c_b=dtemp;
		}

		dtemp = realloc(msm->c_bz,c_col*sizeof(double));
		if (dtemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->c_bz=dtemp;
		}

		itemp = realloc(msm->c_Ap,(c_col+1)*sizeof(int));
		if (itemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->c_Ap=itemp;
		}
	}

	if (msm->c_last_nz!=c_nz)
	{
		itemp = realloc(msm->c_Ai,(c_nz)*sizeof(int));
		if (itemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->c_Ai=itemp;
		}

		dtemp  = realloc(msm->c_Ax,(c_nz)*sizeof(double));
		if (dtemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->c_Ax=dtemp;
		}

		dtemp  = realloc(msm->c_Az,(c_nz)*sizeof(double));
		if (dtemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->c_Az=dtemp;
		}

		dtemp  = realloc(msm->c_Tx,(c_nz)*sizeof(double));
		if (dtemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->c_Tx=dtemp;
		}


		dtemp  = realloc(msm->c_Txz,(c_nz)*sizeof(double));
		if (dtemp==NULL)
		{
			printf("realloc failed\n");
		}else
		{
			msm->c_Txz=dtemp;
		}
	}

	msm->c_last_col=c_col;
	msm->c_last_nz=c_nz;


	for (i=0;i<c_col;i++)
	{
		msm->c_b[i]=(double)c_b[i];
		msm->c_bz[i]=(double)c_bz[i];

	}

	for (i=0;i<c_nz;i++)
	{
		msm->c_Tx[i]=(double)c_Tx[i];
		msm->c_Txz[i]=(double)c_Txz[i];
	}


	double Info [UMFPACK_INFO], Control [UMFPACK_CONTROL];

	// get the default control parameters
	umfpack_zi_defaults (Control) ;

	//change the default print level for this demo
	//(otherwise, nothing will print)
	Control [UMFPACK_PRL] = 1 ;

	//print the license agreement
	//umfpack_zi_report_status (Control, UMFPACK_OK) ;
	Control [UMFPACK_PRL] = 0 ;

	// print the control parameters
	umfpack_zi_report_control (Control) ;

	status = umfpack_zi_triplet_to_col (c_col, c_col, c_nz, c_Ti, c_Tj, msm->c_Tx, msm->c_Txz, msm->c_Ap, msm->c_Ai, msm->c_Ax, msm->c_Az, NULL) ;


	if (status != UMFPACK_OK) {
		complex_error_report(status, __FILE__, __func__, __LINE__);
		return EXIT_FAILURE;
	}

	// symbolic analysis
	//status = umfpack_di_symbolic(col, col, msm->complex_Ap, msm->complex_Ai, Ax, &Symbolic, NULL, NULL);
	status = umfpack_zi_symbolic(c_col, c_col, msm->c_Ap, msm->c_Ai, msm->c_Ax, msm->c_Az, &Symbolic, Control, Info) ;
	umfpack_zi_report_status (Control, status) ;

	if (status != UMFPACK_OK) {
		complex_error_report(status, __FILE__, __func__, __LINE__);
		return EXIT_FAILURE;
	}

	// LU factorization
	//umfpack_di_numeric(msm->complex_Ap, msm->complex_Ai, msm->complex_Ax, Symbolic, &Numeric, NULL, NULL);
	umfpack_zi_numeric (msm->c_Ap, msm->c_Ai, msm->c_Ax, msm->c_Az, Symbolic, &Numeric, Control, Info) ;

	if (status != UMFPACK_OK) {
		complex_error_report(status, __FILE__, __func__, __LINE__);
		return EXIT_FAILURE;
	}
	// solve system

	//umfpack_di_free_symbolic(&Symbolic);

		    // umfpack_di_solve(UMFPACK_A, msm->complex_Ap, msm->complex_Ai, msm->complex_Ax, x, b, Numeric, NULL, NULL);
	status = umfpack_zi_solve(UMFPACK_A, msm->c_Ap, msm->c_Ai, msm->c_Ax, msm->c_Az, msm->c_x, msm->c_xz, msm->c_b, msm->c_bz, Numeric, Control, Info) ;
	if (status != UMFPACK_OK) {
		complex_error_report(status, __FILE__, __func__, __LINE__);
		return EXIT_FAILURE;
	}

		(void) umfpack_zi_report_vector (c_col, msm->c_x, msm->c_xz, Control) ;

	umfpack_zi_free_symbolic (&Symbolic) ;
	umfpack_di_free_numeric(&Numeric);

	for (i = 0; i < c_col; i++)
	{
		c_b[i]=msm->c_x[i];
		c_bz[i]=msm->c_xz[i];
	}

return 0;
}

void complex_umfpack_solver_free(struct matrix_solver_memory *msm)
{
	
}
