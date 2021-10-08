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



/** @file vectors.h
	@brief Header file for vectors.c
*/
#ifndef vectors_h
#define vectors_h

#include <sim_struct.h>

struct vectors
{
	struct vec *data;
	int len;
	int max_len;
	struct vec cog;
	double max_y;
	double min_y;
};

void vectors_load(struct simulation *sim,struct vectors* in,char *file_name);
int vectors_cmp(struct vectors* in,struct vec *test);
void vectors_dump(struct vectors* in);
void vectors_save(struct simulation *sim,char *file_name,struct vectors* in);
void vectors_init(struct vectors* in);
void vectors_malloc(struct vectors* in);
void vectors_pop(struct vectors* in,int index);
void vectors_append_no_rep(struct vectors* in,struct vec *test);
void vectors_append(struct vectors* in,struct vec *in_vec);
void vectors_free(struct vectors* in);
void vectors_cog_cal(struct vectors* in);
void vectors_minmax_y(struct vectors* in);
void vectors_remove_bump_up(struct vectors* in,int start);
void vectors_remove_bump_down(struct vectors* in,int start);
double vectors_min_value(struct vectors* in);
#endif

