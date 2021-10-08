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


/** @file epitaxy.h
	@brief Read the epitaxy from the epitaxy.inp file.
*/


#ifndef shape_h
#define shape_h
#include "advmath.h"
#include <sim_struct.h>
#include <shape_struct.h>

int shape_load_file(struct simulation *sim,struct epitaxy *in,struct shape *s, char *file_name, long double y_pos);
int shape_get_index(struct simulation *sim,struct epitaxy *in,long double x,long double y,long double z);
void shape_free(struct simulation *sim,struct shape *s);
int shape_in_shape(struct simulation *sim,struct shape *s,long double z,long double x,long double y);
void shape_init(struct simulation *sim,struct shape *s);
void shape_load_materials(struct simulation *sim,struct shape *s);
void shape_cpy(struct simulation *sim,struct shape *out,struct shape *in);
int shape_load_from_json(struct simulation *sim,struct shape *s, struct json_obj *obj ,long double y_pos);
void shape_cal_min_max(struct simulation *sim,struct vec *min,struct vec *max,struct shape *s);
#endif
