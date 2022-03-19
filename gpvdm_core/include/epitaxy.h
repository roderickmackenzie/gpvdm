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
int epitaxy_load(struct simulation *sim,struct epitaxy *in, struct json_obj *obj_epi);
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
void epitaxy_debug(struct simulation *sim,struct epitaxy *in);

//optical
long double epitaxy_get_optical_problem_start(struct epitaxy *in);
long double epitaxy_get_optical_problem_stop(struct epitaxy *in);

//heat
long double epitaxy_get_heat_problem_start(struct epitaxy *in);
long double epitaxy_get_heat_problem_stop(struct epitaxy *in);

//exciton
long double epitaxy_get_exciton_problem_start(struct epitaxy *in);
long double epitaxy_get_exciton_problem_stop(struct epitaxy *in);

//singlet
long double epitaxy_get_singlet_problem_start(struct epitaxy *in);
long double epitaxy_get_singlet_problem_stop(struct epitaxy *in);

#endif
