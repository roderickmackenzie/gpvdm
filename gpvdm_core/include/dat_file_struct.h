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

/** @file dat_file_struct.h
	@brief Strcutr to hold .dat files before they are written to disk.
*/

#ifndef dat_file_struct_h
#define dat_file_struct_h
#include "advmath.h"
//#include <zip.h>
#include <triangle.h>

struct dat_file
{
char title[100];
char type[100];
gdouble x_mul;
gdouble y_mul;
gdouble z_mul;
gdouble x_offset;
gdouble y_offset;
gdouble z_offset;
gdouble data_mul;
char x_label[100];
char y_label[100];
char z_label[100];
char data_label[100];
char x_units[100];
char y_units[100];
char z_units[100];
char rgb[100];
char data_units[100];
char section_one[100];
char section_two[100];
int logscale_x;
int logscale_y;
int logscale_z;
int logscale_data;
int write_to_zip;
int norm_x_axis;
int norm_y_axis;
long double data_max;
long double data_min;
int x;
int y;
int z;
int dump_gnu_plot_file;
gdouble time;
gdouble Vexternal;
char *buf;
struct triangle *data;
int len;
int max_len;
char zip_file_name[400];
char file_name[400];
//struct zip *zip_file;
};

#endif
