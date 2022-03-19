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

struct snapshots
{
	char icon[100];
	char type[100];
	char plot_type[100];
	char name[100];
	char path[PATH_MAX];
};

void dump_init(struct simulation *sim,struct device* in);
void dump_load_config(struct simulation* sim,struct device *in);
void dump_remove_snapshots(struct simulation* sim, char *output_path);
void dump_dynamic_init(struct simulation *sim,struct dynamic_store *store,struct device *in);
void dump_dynamic_save(struct simulation *sim,struct device *in,char *outputpath,struct dynamic_store *store);
void dump_dynamic_save_j(struct simulation *sim,struct device *in,char *outputpath,struct dynamic_store *store);
void dump_dynamic_add_data(struct simulation *sim,struct dynamic_store *store,struct device *in, gdouble x_value);
void dump_dynamic_free(struct simulation *sim,struct device *in,struct dynamic_store *store);
void dump_slice(struct device *in,char *prefix);
void dump_1d_slice(struct simulation *sim,struct device *in,char *out_dir);
void dump_zx(struct simulation *sim,struct device *in,char *out_dir);
void dump_zxy_charge(struct simulation *sim,struct device *in,char *out_dir);
void dump_zxy_singlet(struct simulation *sim,struct device *dev,char *out_dir);
void dump_zxy_interfaces(struct simulation *sim,struct device *dev,char *out_dir);
void dump_write_to_disk(struct simulation *sim,struct device* in);
void dat_file_write_zxy_snapshot_as_slices(struct simulation *sim,char * path, struct dat_file *buf,struct dimensions *dim,long double ***data);

//energy cut through
void dump_energy_slice(struct simulation *sim,struct device *in,int x, int y, int z);
void dump_device_map(struct simulation *sim,char* out_dir,struct device *in);

void dump_clean_cache_files(struct simulation* sim);


void dynamic_info_to_buf(struct simulation *sim,struct dat_file *buf, struct device *dev,struct math_xy* data);

//snapshots
void snapshots_init(struct snapshots *snap);
void dump_make_snapshot_dir(struct simulation *sim,char *ret_path, int number,struct snapshots *snap);

//light
int dump_can_i_dump(struct simulation *sim,struct device *dev, char *file_name);
void dump_dynamic_save_thermal(struct simulation *sim,struct device *dev,char *outputpath,struct dynamic_store *store);
#endif
