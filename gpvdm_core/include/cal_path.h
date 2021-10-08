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

/** @file cal_path.h
	@brief Header file for the functions which calculate in which file location to find stuff.
*/


#ifndef cal_path_h
#define cal_path_h
#include <sim_struct.h>

void cal_path(struct simulation *sim);
char *get_plugins_path(struct simulation *sim);
char *get_lang_path(struct simulation *sim);
char *get_cache_path(struct simulation *sim);
char *get_materials_path(struct simulation *sim);
char *get_filter_path(struct simulation *sim);
char *get_cie_color_path(struct simulation *sim);
char *get_shape_path(struct simulation *sim);
char *get_spectra_path(struct simulation *sim);
int find_dll(struct simulation *sim, char *lib_path,char *lib_name);
void join_path(int max, ...);
void assert_platform_path(char * path);
void get_file_name_from_path(char *out,char *in);
void get_nth_dir_name_from_path(char *out,char *in,int n);
int is_dir_in_path(char *long_path, char* search_dir);
int get_delta_path(struct simulation *sim,char *out, char *root,char *file_name);
char *get_gpvdm_local_path(struct simulation *sim);
char *get_tmp_path(struct simulation *sim);
void remove_file_ext(char * path);
void get_dir_name(char *out,char *in);
void mkdirs(char *dir);

//<strip>
void set_key_path(struct simulation *sim);
//</strip>

#endif
