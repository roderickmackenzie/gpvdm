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
	@brief UMFPACK solver interface.
*/


#include <solver_interface.h>
#include <dll_export.h>
#include <util.h>
#include "lib.h"
#include <log.h>
#include <matrix_solver_memory.h>

struct dll_interface *fun;

EXPORT void dll_matrix_init(struct matrix_solver_memory *msm)
{
//printf("init\n");
}

EXPORT void set_interface(struct dll_interface *in)
{
fun=in;
}

EXPORT void dll_matrix_solve(struct matrix_solver_memory *msm,int col,int nz,int *Ti,int *Tj, long double *Tx,long double *b)
{
//printf("%p %p %p %p\n",Ti,Tj,Tx,b);
umfpack_solver(msm,col,nz,Ti,Tj, Tx,b);
}

EXPORT void dll_matrix_dump(struct matrix_solver_memory *msm,int col,int nz,int *Ti,int *Tj, long double *Tx,long double *b,char *index)
{
//printf_log(sim,"hello\n");
}

EXPORT void dll_matrix_solver_free(struct matrix_solver_memory *msm)
{
umfpack_solver_free(msm);
}

