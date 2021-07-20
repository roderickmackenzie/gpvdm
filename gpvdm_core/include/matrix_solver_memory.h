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

/** @file matrix_solver_memory.h
	@brief matrix_solver_memory
*/

#ifndef matrix_solver_memory_h
#define matrix_solver_memory_h
#include <enabled_libs.h>
#include <stdio.h>
#include "cal_path.h"

struct matrix_solver_memory
{
	//Matrix solver	-	external dll
		int last_col;
		int last_nz;
		double *x;
		int *Ap;
		int *Ai;
		double *Ax;
		double *b;
		double *Tx;
		int x_matrix_offset;

	//Complex matrix solver - external dll
		int c_last_col;
		int c_last_nz;
		double *c_x;
		double *c_xz;
		int *c_Ap;
		int *c_Ai;
		double *c_Ax;
		double *c_Az;
		double *c_b;
		double *c_bz;
		double *c_Tx;
		double *c_Txz;

		int fd_ext_solver;
		int ext_solver_buf_size;
		double *ext_solver_buf;
		char ext_solver_pipe_name[PATH_MAX];
		char fname_from_solver[PATH_MAX];
		char fname_to_solver[PATH_MAX];
		int fd_from_solver;


};

void matrix_solver_memory_init(struct matrix_solver_memory *msm);
void matrix_solver_memory_free(struct simulation *sim,struct matrix_solver_memory *msm);
void matrix_solver_memory_load_dll(struct simulation *sim,struct matrix_solver_memory *msm);
void matrix_solver_memory_check_memory(struct simulation *sim,struct matrix_solver_memory *msm,int col,int nz);
#endif
