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

/** @file exciton_fun.h
@brief exciton functions from libexciton
*/

#ifndef h_exciton_fun
#define h_exciton_fun
#include <complex.h>
#include "advmath.h"
#include "i.h"
#include <sim_struct.h>
#include <epitaxy_struct.h>
#include <exciton.h>
#include <device.h>
#include <json.h>

//General
int exciton_solve(struct simulation *sim, struct exciton *ex,struct device *dev);
void exciton_load_config(struct simulation *sim,struct exciton *ex, struct device *dev);
void exciton_load_config_file(struct simulation *sim,struct exciton *ex, struct json_obj *json_config);
void exciton_setup_dump_dir(struct simulation *sim, char *path,struct exciton *ex);
void exciton_dump(struct simulation *sim,char *path,struct exciton *ex);
void exciton_free_memory(struct simulation *sim,struct exciton *ex);
void exciton_malloc(struct simulation *sim,struct exciton *ex);
void exciton_cpy(struct simulation *sim,struct exciton *out,struct exciton *in);
void exciton_init(struct exciton *ex);

//Transfer between meshes
void exciton_transfer_device_exciton_to_exciton_mesh(struct simulation *sim,struct exciton *ex, struct device *dev);
void exciton_transfer_optical_exciton_to_exciton_mesh(struct exciton *ex,struct device *dev);
void exciton_transfer_temperatures_to_device(struct device *dev,struct exciton *ex);

//New ex model
void exciton_build_obj_pointer_array(struct simulation *sim,struct exciton *ex, struct device *dev);
void exciton_build_materials_arrays(struct simulation *sim,struct exciton *ex, struct device *dev);
int exciton_sim_info(struct simulation *sim, struct exciton *ex,struct device *dev);

//Exciton model
long double exciton_get_error(struct exciton *ex);
int exciton_newton_solve(struct simulation *sim, struct exciton *ex,struct device *dev);
void exciton_set_initial_distribution(struct exciton *ex);

//exciton material
void exciton_material_init(struct exciton_material *mat);
void exciton_material_cpy(struct exciton_material *out,struct exciton_material *in);
void exciton_material_free(struct exciton_material *mat);
void exciton_material_load_from_json(struct simulation *sim,struct exciton_material *mat, struct json_obj *json_exciton_material);
void exciton_cal_get_excitoning_sources_percent(struct simulation *sim,struct device *dev,struct exciton *ex,long double *H_joule,long double *H_recombination,long double *H_parasitic);
#endif
