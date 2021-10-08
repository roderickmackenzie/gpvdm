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

/** @file device.h
	@brief The main structure which holds information about the device.
*/

#ifndef device_fun_h
#define device_fun_h
#include <stdio.h>
#include "code_ctrl.h"
#include "light.h"
#include <epitaxy_struct.h>
#include "advmath.h"
#include <dos_struct.h>
#include <contact_struct.h>
#include <perovskite_struct.h>
#include <circuit_struct.h>
#include <dim.h>
#include <matrix.h>
#include <device.h>
#include <shape.h>

void load_sim_file(struct simulation *sim,struct device *in);
void device_init(struct simulation *sim,struct device *in);
void device_alloc_traps(struct device *in);
void device_get_memory(struct simulation *sim,struct device *in);
void device_cpy(struct simulation *sim,struct device *out,struct device *in);
void device_free(struct simulation *sim,struct device *in);
void device_load_math_config(struct simulation *sim,struct device *in);
void device_dump_world_to_file(struct simulation *sim,struct device *dev,char *file_name);
void device_build_scene(struct simulation *sim,struct device *dev);
void device_add_shape_to_world(struct simulation *sim,struct device *dev,struct shape *s);
void device_calculate_joule_heat(struct simulation *sim,struct device *dev);
void device_calculate_recombination_heat(struct simulation *sim,struct device *dev);
void device_build_obj_pointer_array(struct simulation *sim,struct device *dev);

void dim_alloc_zx_epitaxy(struct dim_zx_epitaxy *dim,struct device *dev);

void device_world_stats(struct simulation *sim,struct device *dev);

//cahce
void device_cache_init(struct solver_cache *cache);
void device_cache_cpy(struct solver_cache *out,struct solver_cache *in);
void epitaxy_load_dos_files(struct simulation *sim,struct device *dev, struct epitaxy *epi,struct json_obj *json_epi);
void device_interface_doping(struct simulation *sim,struct device *dev);
char *get_input_path(struct device *dev);
char *get_output_path(struct device *dev);
void set_input_path(struct device *dev,char *in);
void set_output_path(struct device *dev,char *in);
void device_to_dim(struct simulation *sim,struct dimensions *dim,struct device *dev);

#endif
