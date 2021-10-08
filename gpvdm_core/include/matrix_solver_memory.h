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
