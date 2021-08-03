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
void device_objects_free(struct simulation *sim,struct device *dev);
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
