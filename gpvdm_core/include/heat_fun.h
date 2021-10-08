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

/** @file heat_fun.h
@brief heat functions from libheat
*/

#ifndef h_heat_fun
#define h_heat_fun
#include <complex.h>
#include "advmath.h"
#include "i.h"
#include <sim_struct.h>
#include <epitaxy_struct.h>
#include <heat.h>
#include <device.h>
#include <json.h>

//General
int heat_solve(struct simulation *sim, struct heat *thermal,struct device *dev, int z, int x);
void heat_load_config(struct simulation *sim,struct heat *thermal, struct device *dev);
void heat_load_config_file(struct simulation *sim,struct heat *thermal, struct json_obj *json_config);
void heat_setup_dump_dir(struct simulation *sim, char *path,struct heat *thermal);
void heat_dump(struct simulation *sim,char *path,struct heat *thermal);
void heat_free_memory(struct simulation *sim,struct heat *thermal);
void heat_malloc(struct simulation *sim,struct heat *thermal);
void heat_cpy(struct simulation *sim,struct heat *out,struct heat *in);
void heat_init(struct heat *thermal);

//Transfer between meshes
void heat_transfer_device_heat_to_heat_mesh(struct simulation *sim,struct heat *thermal, struct device *dev);
void heat_transfer_optical_heat_to_heat_mesh(struct heat *thermal, struct light *li);
void heat_transfer_temperatures_to_device(struct device *dev,struct heat *thermal);

//New thermal model
void heat_build_obj_pointer_array(struct simulation *sim,struct heat *thermal, struct device *dev);
void heat_build_materials_arrays(struct simulation *sim,struct heat *thermal, struct device *dev);

//Hydrodynamic model
void hydrodynamic_transfer_temperatures_to_device(struct simulation *sim,struct device *dev,struct heat *thermal,int z, int x);
void hydrodynamic_update_heat(struct simulation *sim, struct heat *thermal,struct device *dev,int z, int x);
void hydrodynamic_mesh_check(struct simulation *sim, struct heat *thermal,struct device *dev);
int hydrodynamic_solve(struct simulation *sim, struct heat *thermal,struct device *dev, int z, int x);

//Lattice heat model
long double lattice_get_error(struct heat *thermal);
int lattice_solve(struct simulation *sim, struct heat *thermal,struct device *dev, int z, int x);
void heat_set_initial_distribution(struct heat *thermal);

//heat material
void heat_material_init(struct heat_material *mat);
void heat_material_cpy(struct heat_material *out,struct heat_material *in);
void heat_material_free(struct heat_material *mat);
void heat_material_load_from_json(struct simulation *sim,struct heat_material *mat, struct json_obj *json_heat_material);
void heat_cal_get_heating_sources_percent(struct simulation *sim,struct device *dev,struct heat *thermal,long double *H_joule,long double *H_recombination,long double *H_parasitic);
#endif
