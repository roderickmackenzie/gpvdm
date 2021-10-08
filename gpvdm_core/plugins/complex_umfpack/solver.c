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

/** @file solver.c
	@brief Complex UMFPACK solver interface.
*/


#include <solver_interface.h>
#include <dll_export.h>
#include <util.h>
#include "lib.h"
#include <log.h>
#include <matrix_solver_memory.h>

EXPORT void dll_complex_matrix_init(struct matrix_solver_memory *msm)
{
//printf("init\n");
}


EXPORT void dll_complex_matrix_solve(struct matrix_solver_memory *msm,int c_col,int c_nz,int *c_Ti,int *c_Tj, long double *c_Tx,long double *c_Txz,long double *c_b,long double *c_bz)
{
	complex_umfpack_solver(msm,c_col,c_nz,c_Ti,c_Tj, c_Tx,c_Txz,c_b,c_bz);
}

EXPORT void dll_complex_matrix_solver_free(struct matrix_solver_memory *msm)
{
	complex_umfpack_solver_free(msm);
}

