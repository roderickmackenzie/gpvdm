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

/** @file dll_export.h
	@brief An interface to the dlls.
*/


#ifndef h_dll_export
#define h_dll_export
#include <enabled_libs.h>
	#define EXPORT

#include <sim_struct.h>
#include <light.h>
#include <device.h>
#include <matrix_solver_memory.h>

extern struct dll_interface *fun;

EXPORT void set_interface();

//Matrix solver
EXPORT void dll_matrix_init(struct matrix_solver_memory *msm);
EXPORT void dll_matrix_solve(struct matrix_solver_memory *msm,int col,int nz,int *Ti,int *Tj, long double *Tx,long double *b);
EXPORT void dll_matrix_dump(struct matrix_solver_memory *msm,int col,int nz,int *Ti,int *Tj, long double *Tx,long double *b,char *index);
EXPORT void dll_matrix_solver_free(struct matrix_solver_memory *msm);

//Complex matrix solver
EXPORT void dll_complex_matrix_solve(struct matrix_solver_memory *msm,int col,int nz,int *Ti,int *Tj, long double *Tx,long double *Txz,long double *b,long double *bz);
EXPORT void dll_complex_matrix_solver_free(struct matrix_solver_memory *msm);

//Light
EXPORT void light_dll_init(struct simulation *sim);
EXPORT int light_dll_solve_lam_slice(struct simulation *sim,struct device *cell,struct light *in, long double *sun_E,int z, int x, int l, int w);
EXPORT void light_dll_ver(struct simulation *sim);
EXPORT void light_fixup(char *name,void (*in));

//Newton solver
EXPORT void dll_newton_set_min_ittr(int ittr);
EXPORT int dll_solve_cur(struct simulation *sim,struct device *in, int z, int x);
EXPORT void dll_solver_realloc(struct simulation *sim,struct device *in);
EXPORT void dll_solver_free_memory(struct simulation *sim,struct device *in);

//electrical plugin
EXPORT void dll_run_simulation(struct simulation *sim,struct device *in);

#endif
