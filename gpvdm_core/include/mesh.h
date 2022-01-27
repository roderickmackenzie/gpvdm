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

/** @file mesh.h
@brief meshing functions
*/

#ifndef mesh_h
#define mesh_h
#include <json.h>

void mesh_check_y(struct simulation *sim,struct mesh *in,struct device *dev);
void mesh_remesh(struct simulation *sim,struct mesh *in,struct device *dev);
void mesh_save(struct simulation *sim,char *file_name,struct mesh *in);
void mesh_free(struct mesh *in);
void mesh_build(struct simulation *sim,struct device *in);
void mesh_cal_layer_widths(struct device *in);
void mesh_init(struct mesh *in);
void mesh_load_file(struct simulation * sim, struct mesh *in,struct json_obj *mesh_xyz);
void mesh_cpy(struct simulation *sim,struct mesh *out,struct mesh *in);
void mesh_malloc_sub_mesh(struct simulation * sim, struct mesh *in);
void mesh_gen_simple(struct simulation * sim, struct mesh *in,long double len,int points);
long double mesh_to_dim(struct simulation *sim,struct dimensions *dim, struct mesh *in,char xyz);
long double mesh_to_dim_heat(struct simulation *sim,struct dimensions *dim, struct mesh *in,char xyz);
long double mesh_to_lin_array(struct simulation *sim,long double *mesh, long double *dmesh, struct mesh *in);

//mesh obj
void mesh_obj_cpy(struct simulation *sim,struct mesh_obj *out,struct mesh_obj *in);
void mesh_obj_load(struct simulation *sim,struct mesh_obj *mesh,struct json_obj *json_mesh);
void mesh_obj_free(struct simulation *sim,struct mesh_obj *in);
void mesh_obj_init(struct mesh_obj *in);
void mesh_numerate_points(struct simulation *sim,struct device *in);
void mesh_dump(struct simulation *sim,struct dimensions *dim);
void mesh_dump_y(struct simulation *sim,struct dimensions *dim);
void mesh_obj_apply_srh_contacts(struct simulation *sim,struct mesh_obj *mesh,struct device *dev);
#endif
