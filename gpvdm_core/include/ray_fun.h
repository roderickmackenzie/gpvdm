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

/** @file ray_fun.h
@brief ray tracing header files.
*/
#ifndef ray_fun_h
#define ray_fun_h

#include <vec.h>
#include <sim_struct.h>
#include <device.h>
#include <ray.h>
#include <object.h>

#define WAIT 0
#define READY 1
#define DONE 2

#define TRUE 1
#define FALSE 0

#define RAY_MAX 5000

void light_update_ray_mat(struct simulation *sim,struct device *dev,struct image *my_image);
void ray_engine_init(struct image *in);
int between(double v, double x0, double x1);
void add_triangle(struct device *dev, double x0,double y0,double z0,double x1,double y1,double z1,double x2,double y2,double z2,int object_uid,int edge);
void ray_reset(struct ray_worker *worker);
int add_ray(struct simulation *sim,struct ray_worker *worker,struct vec *start,struct vec *dir,double mag,int obj_uid,int parent);
void ray_populate_with_shapes(struct simulation *sim,struct device *dev,struct epitaxy *in);
void dump_plane_to_file(struct simulation *sim,char *file_name,struct image *in,struct device *dev);
void dump_plane(struct simulation *sim,struct device *dev,struct image *in);
double get_rand();
void obj_norm(struct vec *ret,struct triangle *my_obj);
int ray_intersect(struct vec *ret,struct triangle *my_obj,struct ray *my_ray);
int search_obj(struct simulation *sim,struct image *in,struct ray *my_ray);
struct triangle * search_triangle(struct simulation *sim,struct device *dev,struct ray *my_ray, int do_update);
int activate_rays(struct ray_worker *worker);
int pnpoly(struct image *in, struct vec *xy,int id);
void get_refractive(struct simulation *sim,struct image *in,double *alpha,double *n0,double *n1,struct ray *my_ray);
int propergate_next_ray(struct simulation *sim,struct device *dev,struct image *in,struct ray_worker *w);
double get_eff(struct image *in);
void ray_build_scene(struct simulation *sim,struct device *cell,struct image *my_image,struct epitaxy *my_epitaxy);
void ray_free(struct simulation *sim,struct device *in,struct image *my_image);
void ray_read_config(struct simulation *sim,struct image *my_image,struct json_obj *json_config);
void ray_solve(struct simulation *sim,struct device *dev, double x0, double y0, double z0,double mag,struct ray_worker *worker);
void ray_solve_all(struct simulation *sim,struct device *dev);
void dump_extraction_efficiency(struct simulation *sim,struct device *dev,struct image *in);
void dump_ang_escape(struct simulation *sim,struct image *in);
double ray_cal_escape_angle(struct image *in, struct ray_worker *worker);
void ray_escape_angle_reset(struct image *in,int l);
void dump_ang_escape_as_rgb(struct simulation *sim,struct image *in);
int search_object(struct simulation *sim,struct device *dev,struct ray *my_ray);
void ray_malloc(struct simulation *sim,struct device *in,struct image *my_image);
void ray_escape_angle_norm(struct image *in);
void ray_calculate_avg_extract_eff(struct simulation *sim,struct epitaxy *epi,struct image *in);
double ray_tri_get_min_y(struct triangle* tri);

void photon_extract_eff_reset(struct simulation *sim,struct epitaxy *epi,struct image *in);
void photon_extract_eff_norm(struct simulation *sim,struct epitaxy *epi,struct image *in);

struct object *add_box(struct device *dev,double x0,double y0,double z0,double dx,double dy,double dz,int object_type);
struct object *add_pyramid(struct image *in,double x0,double y0,double z0,double dx,double dy,double dz);
struct object *add_dome(struct image *in,double x0,double y0,double z0,double dx0,double dy0,double dz0);

struct object *ray_add_object(struct device *dev,struct triangles *tri);
void ray_cpy(struct ray *a,struct ray *b);
void ray_dump_triangle(struct simulation *sim,struct device *dev,struct image *in,struct triangle *tri);
void dump_ray_to_file(struct simulation *sim,struct image *in,struct ray *my_ray,struct device *dev);


struct object *add_plane(struct device *dev,double x0,double y0,double z0,double dx,double dz,int object_type);
void dump_rendered_image(struct simulation *sim,char *out_dir,struct image *in);
void dump_rendered_cross_section(struct simulation *sim,char *out_dir, struct image *in);

void ray_label_triangles(struct simulation *sim,struct device *dev);
//search
struct object *ray_obj_search_xyz(struct simulation *sim,struct device *dev,struct vec *xyz);
struct object *ray_obj_search(struct simulation *sim,struct device *dev,struct ray *in_ray);
struct object *ray_obj_search_by_name(struct simulation *sim,struct device *dev,char *serach_name);
//ray functions
void ray_init(struct ray *a);

//Viewpoint
void ray_viewpoint_reset(struct simulation *sim,struct image *my_image);
void ray_read_viewpoint(struct simulation *sim,struct image *my_image, struct json_obj *json_config);
void ray_dump_shapshots(struct simulation *sim,struct device *dev, struct image *my_image ,struct ray_worker *worker,int layer);

#endif
