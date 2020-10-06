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

