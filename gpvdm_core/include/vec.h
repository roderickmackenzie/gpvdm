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



/** @file vec.h
	@brief Header file for vec.c
*/
#ifndef vech
#define vech
///Structure to hold vector
struct vec
{
	double x;
	double y;
	double z;
};

//Vector routines

double vec_mod(struct vec *my_vec);
void vec_plus(struct vec *my_vec1);
void vec_fprintf(FILE *out,struct vec *my_vec);
double deg(double in);
double vec_ang(struct vec *in_v0,struct vec *in_v1);
void vec_sub(struct vec *my_vec1,struct vec *my_vec2);
void vec_add(struct vec *my_vec1,struct vec *my_vec2);
void vec_div(struct vec *my_vec1,double n);
void vec_mul(struct vec *my_vec1,double n);
void vec_div_vec(struct vec *my_vec1,struct vec *my_vec2);
void vec_mul_vec(struct vec *my_vec1,struct vec *my_vec2);
double vec_fabs(struct vec *my_vec);
void vec_rotate(struct vec *my_vec,double ang);
void vec_rotate_y(struct vec *my_vec,double ang);
void vec_rotate_x(struct vec *my_vec,double ang);
double vec_dot(struct vec *a,struct vec *b);
double vec_dist(struct vec *a,struct vec *b);
double vec_dist_zx(struct vec *a,struct vec *b);
void vec_init(struct vec *my_vec);
void vec_cross(struct vec *ret,struct vec *a,struct vec *b);
void vec_swap(struct vec *my_vec);
void vec_cpy(struct vec *my_vec1,struct vec *my_vec2);
void vec_norm(struct vec *my_vec);
void vec_print(struct vec *my_vec);
void vec_set(struct vec *my_vec,double x, double y, double z);
double overlap(double x0,double x1);
double vec_overlap(struct vec *a,struct vec *b);
double vec_get_dihedral(struct vec *a,struct vec *b,struct vec *c,struct vec *d);
void vec_rot(struct vec *in,struct vec *unit,struct vec *base, double ang);
void vec_add_values(struct vec *my_vec, double x,double y, double z);
int vec_cmp(struct vec *my_vec1,struct vec *my_vec2);
void rotx_vec(struct vec *out, struct vec *in,double a);
void roty_vec(struct vec *out, struct vec *in,double a);
void rotz_vec(struct vec *out, struct vec *in,double a);
double vec_ang(struct vec *one,struct vec *two);
double vec_overlap_test(struct vec *a,struct vec *b);
void vec_flip_y_axis(struct vec *one,double y);
int vec_within_box(struct vec *l,struct vec *h,struct vec *v);
void vec_dump_to_file(char *file_name,struct vec *one);
int vec_cmp_exact(struct vec *my_vec1,struct vec *my_vec2);
#endif
