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

/** @file dat_file.h
	@brief Strcutr to hold .dat files before they are written to disk.
*/

#ifndef dat_file_h
#define dat_file_h
#include "advmath.h"
#include <dat_file_struct.h>
//#include <zip.h>
#include <device.h>
#include <triangle.h>


void buffer_zip_set_name(struct dat_file *in,char * name);
void buffer_init(struct dat_file *in);
void buffer_malloc(struct dat_file *in);
void buffer_add_xy_data(struct simulation *sim,struct dat_file *in,gdouble *x, gdouble *y, int len);
void buffer_add_string(struct dat_file *in,char * string);
void buffer_add_info(struct simulation *sim,struct dat_file *in);
void buffer_dump(struct simulation *sim,char * file,struct dat_file *in);
void buffer_dump_path(struct simulation *sim,char *path,char * file,struct dat_file *in);
void buffer_free(struct dat_file *in);
void buffer_dump_aes(char *path,char * file,struct dat_file *in,char *key_text);
void buffer_add_xy_data_z_label(struct dat_file *in,gdouble *x, gdouble *y, gdouble *z, int len);
void dat_file_add_zxy_long_double_light_data(struct simulation *sim,struct dat_file *in,long double ***data, struct dim_light *dim);
void buffer_add_zxy_object_uid(struct simulation *sim,struct dat_file *in,struct object ****data, struct dimensions *dim);
void buffer_add_zxy_heat_data(struct simulation *sim,struct dat_file *in,long double ***data, struct dim_heat *dim);
void buffer_dump_cache(struct simulation *sim,char * file,struct dat_file *in);
void buffer_add_dir(struct simulation *sim,char * file_name);
void buffer_add_3d_device_data_including_boundaries(struct simulation *sim,struct dat_file *buf,struct device *in,gdouble ***data,long double **left,long double **right);
void buffer_add_2d_device_data_int(struct simulation *sim,struct dat_file *buf,struct device *in,int **data);
void dat_file_dump_gnuplot_file(struct simulation *sim,char * path,char * file_name,struct dat_file *in);
void dat_file_load_info(struct simulation *sim,struct dat_file *in,char *file_name);
void dat_file_add_zxy_from_zxyl_long_double_light_data(struct simulation *sim,struct dat_file *in,long double ****data, struct dim_light *dim,int l);
void dat_file_add_zxy_from_zxyl_float_light_data(struct simulation *sim,struct dat_file *in,float ****data, struct dim_light *dim,int l);
void dat_file_add_zxy_from_zxyl_double_light_data(struct simulation *sim,struct dat_file *in,double ****data, struct dim_light *dim,int l);
void dat_file_reset(struct dat_file *in);
int buffer_set_file_name(struct simulation *sim,struct device *dev,struct dat_file *in,char * file_name);
#endif
