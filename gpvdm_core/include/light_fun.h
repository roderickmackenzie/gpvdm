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

/** @file light.h
@brief light functions from liblight
*/

#ifndef h_light_fun
#define h_light_fun
#include <complex.h>
#include "advmath.h"
#include "i.h"
#include <sim_struct.h>
#include <epitaxy_struct.h>
#include <light.h>
#include <device.h>

void light_init(struct simulation *sim, struct light *in);
void light_norm_photon_density(struct simulation *sim, struct light *li);
void light_malloc(struct simulation *sim,struct light *li);
void light_load_materials(struct simulation *sim,struct light *li, struct device *dev);
long double light_cal_photon_density(struct simulation *sim,struct light *li, struct device *dev);
void light_load_config(struct simulation *sim,struct light *li, struct device *dev);
void light_load_config_file(struct simulation *sim,struct light *li);
void light_build_mesh(struct simulation *sim,struct light *li,struct dimensions *electrical_dim);
void light_build_materials_arrays(struct simulation *sim,struct light *li, struct device *dev);
void light_set_sun_power(struct light *li,long double power, long double laser_eff);
void light_free_memory(struct simulation *sim,struct light *li);
void light_cpy_memory(struct simulation *sim,struct light *out,struct light *in);
void light_get_mode(struct math_xy *mode,int lam,struct light *li);
void light_set_unity_power(struct light *li);
void light_solve_optical_problem(struct simulation *sim,struct device *dev,struct light *li);
void light_solve_all(struct simulation *sim,struct device *dev,struct light *li);
void light_set_dump(struct light *li,int dump);
void light_free(struct simulation *sim,struct light *li);
void light_dump(struct simulation *sim,struct light *li);
int light_solve_lam_slice(struct simulation *sim,struct device *dev, struct light *li,int z, int x,int lam, int w);
void light_dump_1d(struct simulation *sim,struct light *li, int i,char *ext);
void light_dump_verbose_1d(struct simulation *sim,struct light *li, int i,char *ext);
void light_dump_verbose_2d(struct simulation *sim,struct light *li);
void light_get_mode(struct math_xy *mode,int lam,struct light *li);
void light_set_unity_laser_power(struct light *li,int lam);
void light_free_epitaxy(struct light *li);
void light_import_epitaxy(struct simulation *sim,struct light *li,struct epitaxy *my_epitaxy);
void light_calculate_complex_n(struct light *li);
int light_load_laser(struct simulation *sim, struct light *li,char *name);
long double light_get_sun(struct light *li);
void light_set_sun(struct light *li,long double Psun);
void light_set_model(struct light *li,char *model);
void light_set_sun_delta_at_wavelength(struct simulation *sim,struct light *li,long double lam);
int light_get_pos_from_wavelength(struct simulation *sim,struct light *li,double lam);
void light_setup_dump_dir(struct simulation *sim,struct light *li);
long double light_calculate_photons_absorbed_in_active_layer(struct light *li);
void light_dump_sim_info(struct simulation *sim,struct light *li);
void light_transfer_gen_rate_to_device(struct device *dev,struct light *li);
void light_load_filter(struct simulation *sim,struct light *li);
long double light_get_optical_power(struct simulation *sim,struct light *li);
void light_dump_stats(struct simulation *sim,struct light *li);
void light_dump_shapshots(struct simulation *sim,struct light *li);
#endif
