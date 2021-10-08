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

/** @file triangles.h
@brief Functions to manipulate lots of triangles
*/
#ifndef triangles_io_h
#define triangles_io_h

#include <vec.h>
#include <triangle.h>
#include <vectors.h>
void triangle_load_from_file(struct simulation *sim,struct triangles *in,char *file_name);
void triangle_print(struct triangle *in);
void triangles_print(struct triangles *in);
void triangles_free(struct triangles *in);
void triangles_cpy(struct triangles *out,struct triangles *in);
void triangles_find_min(struct vec *out,struct triangles *in);
void triangles_find_max(struct vec *out,struct triangles *in);
void triangles_sub_vec(struct triangles *in,struct vec *v);
void triangles_add_vec(struct triangles *in,struct vec *v);
void triangles_div_vec(struct triangles *in,struct vec *v);
void triangles_mul_vec(struct triangles *in,struct vec *v);
void triangles_cal_edges(struct triangles *in);
void triangles_init(struct triangles *tri);
void triangles_malloc(struct triangles *tri);
void triangles_save(struct simulation *sim,char *file_name,struct triangles *in);
void triangles_add_triangle(struct triangles *obj, struct triangle *tri);
void triangles_set_object_type(struct triangles *in,int object_type);
double triangles_interpolate(struct triangles *in,struct vec *p);
void triangles_to_dat_file(struct dat_file *buf,struct triangles *in);
void triangles_reduce(struct simulation *sim,struct triangles *in, double min_allowable_ang);
struct triangle *triangles_find_by_zx(struct triangles *in,double z0,double x0,double z1,double x1,double z2,double x2);
void triangles_calculate_cog(struct triangles *in);
int triangles_count(struct triangles *in);
void triangles_find_by_vec(	struct triangles *out, struct triangles *in,struct vec *search);
void triangles_dump(struct triangles *in);
void triangles_to_vectors(struct vectors* out,struct triangles *in);
void triangles_vectors_to_triangles(struct triangles *out,struct vectors* in, int start);
int triangles_reduce_nodes(struct simulation *sim,struct triangles *in, int pass, double min_allowable_ang);
void triangles_replace_vec(	struct triangles *in,struct vec *find,struct vec *replace);
void triangles_delete_zero_area_triangles(struct triangles *in);
double triangles_get_area(struct triangles *in);
double triangles_get_area_zx(struct triangles *in);
double triangles_get_delta(struct triangles *many,struct triangles *few);
void triangles_remove_triangles(struct triangles *full_list, struct triangles *to_remove);
void triangles_add_many(struct triangles *full_list, struct triangles *in);
void triangles_cal_angles(struct vectors* out,struct triangles* in);
void triangles_remove_y_zero_triangles(struct triangles *in);
void triangles_make_btm_layer_from_top(struct triangles *in);
void triangles_rotate_y(struct triangles *in,double ang);
void triangles_rotate_x(struct triangles *in,double ang);

//flags
void triangles_set_flag(struct triangles *obj,int flag);
void triangles_unset_flags_for_flat(struct triangles *obj);
double triangle_Ra(struct simulation *sim,struct triangles *obj);


//Roughness
double triangle_Rq(struct simulation *sim,struct triangles *obj);
#endif
