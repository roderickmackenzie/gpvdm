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

/** @file dump.h
	@brief Fucntions to write data to disk.
*/


#ifndef h_dump
#define h_dump
#include "device.h"
#include "dump_ctrl.h"
#include "dynamic_store.h"
#include "dat_file.h"
#include <sim_struct.h>

void dump_init(struct simulation *sim,struct device* in);
void dump_load_config(struct simulation* sim,struct device *in);
void dump_remove_snapshots(struct simulation* sim);
void dump_dynamic_init(struct simulation *sim,struct dynamic_store *store,struct device *in);
void dump_dynamic_save(struct simulation *sim,struct device *in,char *outputpath,struct dynamic_store *store);
void dump_dynamic_save_j(struct simulation *sim,struct device *in,char *outputpath,struct dynamic_store *store);
void dump_dynamic_add_data(struct simulation *sim,struct dynamic_store *store,struct device *in, gdouble x_value);
void dump_dynamic_free(struct simulation *sim,struct device *in,struct dynamic_store *store);
void dump_slice(struct device *in,char *prefix);
void dump_energy_slice(struct simulation *sim,char *out_dir,struct device *in);
void dump_device_map(struct simulation *sim,char* out_dir,struct device *in);
void dump_1d_slice(struct simulation *sim,struct device *in,char *out_dir);
void dump_zx(struct simulation *sim,struct device *in,char *out_dir);
void dump_zxy_charge(struct simulation *sim,struct device *in,char *out_dir);
void dump_zxy_interfaces(struct simulation *sim,struct device *dev,char *out_dir);
void dump_write_to_disk(struct simulation *sim,struct device* in);
void buffer_add_3d_data(struct simulation *sim,struct dat_file *buf,struct dimensions *dim, gdouble ***data);
void buffer_add_zx_data(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,gdouble **data);
void buffer_add_zxy_data_y_slice(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,long double ***data,int y);

void dumpfiles_load(struct simulation* sim);
void dumpfiles_free(struct simulation* sim);
int dumpfiles_should_dump(struct simulation* sim,char *name);
void dumpfiles_process(struct simulation* sim,struct math_xy *in,char *name);
void dumpfiles_turn_on_dir(struct simulation* sim,char *in);
void dump_clean_cache_files(struct simulation* sim);
void dump_make_snapshot_dir(struct simulation *sim,char *out_dir ,char *snapshot_name, int number);
void dump_make_snapshot_dir_with_info(struct simulation *sim,char *out_dir ,long double time,long double voltage, long double fx, int number);
void dump_make_snapshot_dir_with_name(struct simulation *sim,char *out_dir ,long double time,long  double voltage, long double fx, int number,char *snapshot_name);
void buffer_add_3d_device_data_int(struct simulation *sim,struct dat_file *buf,struct device *in,int ***data);
void buffer_add_3d_to_2d_projection(struct simulation *sim,struct dat_file *buf,struct device *in,gdouble ***data);
void buffer_add_zxy_rgb_data(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,gdouble ***data);
void dynamic_info_to_buf(struct simulation *sim,struct dat_file *buf, struct device *dev,struct math_xy* data);

//light
void buffer_add_yl_light_data(struct simulation *sim,struct dat_file *buf,struct dim_light *dim,long double ****data,long double shift ,int z, int x);
#endif
