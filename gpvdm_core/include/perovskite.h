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


/** @file perovskite.h
	@brief Header file for perovskite.c
*/
#ifndef perovskite_h
#define perovskite_h

#include <sim_struct.h>
#include <device.h>
#include <perovskite_struct.h>

int perovskite_enabled(struct device *in);
void perovskite_dump(struct simulation *sim,struct device *in);
long double get_perovskite_error(struct device *in,long double *b);
int solve_perovskite(struct simulation *sim,struct device *in, int z, int x);
void perovskite_build_matrix(struct simulation *sim,struct device *in,struct perovskite *config, int z, int x);
void perovskite_init(struct simulation *sim,struct device *in);
void perovskite_free(struct simulation *sim,struct device *in);
void perovskite_load(struct simulation *sim,struct device *in);
void perovskite_cpy(struct simulation *sim,struct perovskite *out,struct perovskite *in);
#endif
