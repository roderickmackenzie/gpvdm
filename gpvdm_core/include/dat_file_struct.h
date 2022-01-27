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
	int srh_bands;
	int dump_gnu_plot_file;
	gdouble time;
	gdouble Vexternal;
	char *buf;
	struct triangle *data;
	int len;
	int max_len;

	char zip_file_name[400];
	char file_name[400];
	char cols[10];		//These can be zxyd
	int bin;
	//struct zip *zip_file;
};

#endif
