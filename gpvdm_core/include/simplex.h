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

/** @file simplex.c
@brief Downhill simplex code
*/
#ifndef simplex_h
#define simplex_h

#define TINY 1.0e-10		//A small number.
#define NMAX 5000			//Maximum allowed number of function evaluations.
#define SIMPLEX_CONVERGED 1
#define  SIMPLEX_MAX 2

#include <sim_struct.h>

struct multimin
{
	int ittr;
	int n_max;
	int nsimplex;
	int ndim;
	double stop_error;
	double *x;
	double **p;
	int i_hi0;
	int i_hi1;
	int i_lo;
	double *y;
	double *center;
	double  *ptry;
	double ytry;
	double *s;
	double error;
	double error_delta;
	double error_last;
	double (*fn)(double *p,int len);
};

void multimin_dump(struct simulation *sim,struct multimin *data);
void multimin_init(struct multimin *data);
void multimin_malloc(struct multimin *data);
void multimin_init_simplex(struct multimin *data);
void multimin_free(struct multimin *data);
void multimin_cal_center(struct multimin *data);
void multimin_find_best(struct multimin *data);
void multimin_sync(struct multimin *data,int s);
void multimin_shrink(struct multimin *data);
double contract(struct multimin *data,double mul);
double expand(struct multimin *data,double mul);
double reflect(struct multimin *data,double mul);
int simplex_iterate(struct simulation *sim,struct multimin *data);
void simplex_copy_ans(struct multimin *data);
void newton_start(struct simulation *sim,struct multimin *data);

#endif
