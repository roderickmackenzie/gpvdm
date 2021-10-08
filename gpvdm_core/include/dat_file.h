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
