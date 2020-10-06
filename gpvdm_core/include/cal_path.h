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

/** @file cal_path.h
	@brief Header file for the functions which calculate in which file location to find stuff.
*/


#ifndef cal_path_h
#define cal_path_h
#include <sim_struct.h>

void cal_path(struct simulation *sim);
char *get_plugins_path(struct simulation *sim);
char *get_lang_path(struct simulation *sim);
char *get_input_path(struct simulation *sim);
char *get_output_path(struct simulation *sim);
char *get_cache_path(struct simulation *sim);
char *get_materials_path(struct simulation *sim);
char *get_cie_color_path(struct simulation *sim);
char *get_shape_path(struct simulation *sim);
char *get_spectra_path(struct simulation *sim);
void set_input_path(struct simulation *sim,char *in);
void set_output_path(struct simulation *sim,char *in);
int find_dll(struct simulation *sim, char *lib_path,char *lib_name);
void join_path(int max, ...);
void assert_platform_path(char * path);
void get_file_name_from_path(char *out,char *in);
void get_nth_dir_name_from_path(char *out,char *in,int n);
int is_dir_in_path(char *long_path, char* search_dir);
int get_delta_path(struct simulation *sim,char *out, char *root,char *file_name);
char *get_gpvdm_local_path(struct simulation *sim);
char *get_tmp_path(struct simulation *sim);

//<strip>
void set_key_path(struct simulation *sim);
//</strip>

#endif
