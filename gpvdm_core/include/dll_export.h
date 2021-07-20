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
