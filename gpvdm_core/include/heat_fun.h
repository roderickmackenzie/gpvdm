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

//General
int heat_solve(struct simulation *sim, struct heat *thermal,struct device *dev, int z, int x);
void heat_load_config(struct simulation *sim,struct heat *thermal, struct device *dev);
void heat_load_config_file(struct simulation *sim,struct heat *thermal);
void heat_setup_dump_dir(struct simulation *sim,struct heat *thermal);
void heat_dump(struct simulation *sim,char *path,struct heat *thermal);
void heat_free_memory(struct simulation *sim,struct heat *thermal);
void heat_malloc(struct simulation *sim,struct heat *thermal);
void heat_cpy(struct simulation *sim,struct heat *out,struct heat *in);
void heat_init(struct heat *thermal);

//Transfer between meshes
void heat_transfer_device_heat_to_heat_mesh(struct heat *thermal, struct device *dev);
void heat_transfer_optical_heat_to_heat_mesh(struct heat *thermal, struct light *li);

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
#endif
