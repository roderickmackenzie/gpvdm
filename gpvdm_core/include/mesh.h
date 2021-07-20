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
long double mesh_to_dim_heat(struct simulation *sim,struct dim_heat *dim, struct mesh *in,char xyz);
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
