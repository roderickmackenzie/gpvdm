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

/** @file object.h
@brief ray tracing header files.
*/
#ifndef object_h
#define object_h

#include <vec.h>
#include <sim_struct.h>
#include <triangle.h>

struct object
{
	int epi_layer;		//delete and don't use info
	char name[100];
	int uid;
	double *n;
	double *alpha;
	struct triangles tri;
	struct vec min;
	struct vec max;
	struct shape* s;		//This is a poinnter to the origonal shape which generated the object
};

//Object
void object_flip_y_axis(struct object *obj);
void object_sub_y(struct object *obj,double y);
void object_add_y(struct object *obj,double y);
double object_get_min_y(struct object *obj);
void object_init(struct object *obj);
void object_free(struct object *obj);
void object_cal_min_max(struct object *obj);
void object_nalpha_malloc(struct object *obj,int l_max);
void object_nalpha_free(struct object *obj);
void object_malloc(struct object *obj);
#endif
