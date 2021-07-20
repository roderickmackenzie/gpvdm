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


/** @file epitaxy.h
	@brief Read the epitaxy from the epitaxy.inp file.
*/


#ifndef epitaxy_h
#define epitaxy_h
#include "advmath.h"
#include <sim_struct.h>
#include <shape_struct.h>
#include <epitaxy_struct.h>
#include <device.h>
#include <json.h>

void epitaxy_init(struct simulation *sim,struct epitaxy *epi);
void epi_layer_init(struct simulation *sim,struct epi_layer *layer);
void epitaxy_cpy(struct simulation *sim,struct epitaxy *out,struct epitaxy *in);
void epitaxy_load(struct simulation *sim,struct epitaxy *in, struct json_obj *obj_epi);
gdouble epitaxy_get_electrical_length(struct epitaxy *in);
gdouble epitaxy_get_optical_length(struct epitaxy *in);
int epitaxy_get_layer(struct epitaxy *in,gdouble pos);
//int epitaxy_get_electrical_material_layer(struct epitaxy *in,gdouble pos);
gdouble epitaxy_get_device_start(struct epitaxy *in);
gdouble epitaxy_get_device_stop(struct epitaxy *in);
gdouble epitaxy_get_device_start_i(struct epitaxy *in);
int epitaxy_get_epitaxy_layer_using_electrical_pos(struct epitaxy *in,gdouble pos);
void epitaxy_free(struct simulation *sim,struct epitaxy *in);
void epitaxy_free_materials(struct epitaxy *in);
void epitaxy_load_emission(struct simulation *sim,struct epi_layer *layer);
void epitaxy_mask(struct simulation *sim,struct device *dev);
void epitaxy_shapes_load(struct simulation *sim,struct epitaxy *in, struct json_obj *obj_epi);
void epitaxy_load_interface_file(struct simulation *sim, struct epi_layer *layer,struct json_obj *interface_json);
void epitaxy_setup_interfaces(struct simulation *sim,struct device *dev);
long double epitaxy_get_total_length_y(struct epitaxy *in);
void epitaxy_load_pl_file(struct simulation *sim, struct epi_layer *layer,struct json_obj *pl_json);

//optical
long double epitaxy_get_optical_problem_start(struct epitaxy *in);
long double epitaxy_get_optical_problem_stop(struct epitaxy *in);

//heat
long double epitaxy_get_heat_problem_start(struct epitaxy *in);
long double epitaxy_get_heat_problem_stop(struct epitaxy *in);
#endif
