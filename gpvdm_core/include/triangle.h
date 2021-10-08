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

/** @file triangle.h
@brief triangle structure
*/
#ifndef triangle_h
#define triangle_h

#include <vec.h>

struct triangle
{
	struct vec xy0;
	struct vec xy1;
	struct vec xy2;
	int object_uid;

	int object_type;

	//pre calculation
	struct vec edge1;
	struct vec edge2;

	int obj_left;
	int obj_right;

	struct vec cog;
	int flag;

	//not used

	int deleted;
};

struct triangles
{
	struct triangle *data;
	int edges_calculated;
	int max_len;
	int len;
};

void triangle_init(struct triangle *tri);
void triangle_print(struct triangle *in);
void triangle_cal_cog(struct triangle *in);
void triangle_norm(struct vec *ret,struct triangle *my_obj);
void triangle_dump(char *file_name,struct triangle *tri);
int triangle_vec_within (struct triangle *tri,struct vec *pt);
double triangle_get_y_from_xz(struct triangle *tri,double x, double z);

double triangle_get_min_y(struct triangle* tri);
double triangle_get_max_y(struct triangle* tri);
void ray_tri_flip_y_axis(struct triangle* tri,double y);
void ray_tri_sub_y(struct triangle* tri,double y);
void ray_tri_add_y(struct triangle* tri,double y);
void triangle_cal_edges(struct triangle* tri);
double triangle_cal_area(struct triangle* tri);
int triangle_contains_zx_line(struct triangle* tri,struct vec *vec0,struct vec *vec1);
struct vec * triangle_get_furthest_point(struct triangle* tri,struct vec *vec0);
int triangle_contains_vec(struct triangle* tri,struct vec *vec0);
double triangle_cal_area_zx(struct triangle* tri);
int triangle_cmp(struct triangle* tri0,struct triangle* tri1);
void triangle_cpy(struct triangle* tri0,struct triangle* tri1);
void triangle_cal_angles(struct vec *angles,struct triangle* tri);

#endif
