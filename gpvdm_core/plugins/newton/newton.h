// 
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// The model can simulate OLEDs, Perovskite cells, and OFETs.
// 
// Copyright (C) 2012-2017 Roderick C. I. MacKenzie info at gpvdm dot com
// 
// https://www.gpvdm.com
// 
// 
// This program is free software; you can redistribute it and/or modify it
// under the terms and conditions of the GNU Lesser General Public License,
// version 2.1, as published by the Free Software Foundation.
// 
// This program is distributed in the hope it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
// 
// 
// 

/** @file newton.h
@brief Header file for newton solver.
*/



#ifndef h_newton
#define h_newton

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <device.h>

void dllinternal_newton_set_min_ittr(int ittr);
void update_solver_vars(struct simulation *sim,struct device *in,int z,int x,int clamp);
void fill_matrix(struct simulation *sim,struct device *in,int z,int x);
gdouble get_cur_error(struct simulation *sim,struct device *in);
gdouble get_abs_error(struct device *in);
void solver_cal_memory(struct device *in,int *ret_N,int *ret_M);
void dllinternal_solver_free_memory(struct simulation *sim,struct device *in);
int dllinternal_solve_cur(struct simulation *sim,struct device *in, int z, int x);
void dllinternal_solver_realloc(struct simulation *sim,struct device *in, int dim);
#endif
