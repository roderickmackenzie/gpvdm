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

/** @file util.h
@brief util functions
*/


#ifndef h_util
#define h_util
#include <stdio.h>
#include <sim_struct.h>


#include <colors.h>

void set_ewe_lock_file(char *lockname,char *data);
void print_hex(struct simulation *sim,unsigned char *data);
void remove_dir(struct simulation *sim,char* dir_name);
int ewe(struct simulation *sim, const char *format, ...);
double read_value(struct simulation *sim,char *file,int skip,int line);

void randomprint(struct simulation *sim,char *in);
int scanarg( char* in[],int count,char * find);
int get_arg_plusone_pos( char* in[],int count,char * find);
char * get_arg_plusone( char* in[],int count,char * find);
FILE *fopena(char *path,char *name,const char *mode);

void copy_file(struct simulation *sim,char *output,char *input);
int get_file_len(struct simulation *sim,char *file_name);
int cmpstr_min(char * in1,char *in2);
int english_to_bin(struct simulation *sim,char * in);
void write_x_y_to_file(struct simulation *sim,char *name,double *x,double *y,int len);
void write_x_y_z_to_file(struct simulation *sim,char *name,double *x,double *y,double *z,int len);
int get_dir_name_from_path(char *out, char *in);
void string_to_hex(char* out,char* in);
int strextract_name(char *out,char * in);
int strextract_int(char * in);
char* strextract_domain(char * in);
int find_config_file(struct simulation *sim,char *ret,char *dir_name,char* search_name,char *start_of_name);
void fx_with_units(char *out,double number);
void time_with_units(char *out,double number);
int is_domain(char * in);
int isdir(const char *path);
int path_up_level(char *out, char *in);
int fnmatch2(char *pat,char *in);
void split_dot(char *out, char *in);
void str_split(char *in_string, ...);
int isfile(char *in);
int replace_number_in_string(char *buf, char* in, double replace, int n);

void remove_file(struct simulation *sim,char* file_name);
void poll_gui(struct simulation *sim);
void gpvdm_mkdir(char *file_name);

//util_str
int get_number_in_string(double *out, char* in, int n);
int is_number(char a);
int str_isnumber(char *input) ;
int is_numbered_file(char *in,char *root);
int strcmp_end(char * str,char *end);
int extract_str_number(char * in,char *cut);
int strcmp_begin(char * str,char *begin);
int strcmp_begin_safe(char * str,int str_len,char *begin,int begin_len);
void get_meter_dim(char *unit,long double *mul,long double max_val);
void get_time_dim(char *unit,long double *mul,long double max_val);
void get_wavelength_dim(char *unit,long double *mul,long double max_val);
void str_strip(char *in_string);

//zip_buffer
void write_zip_buffer(struct simulation *sim,char *outfile,long double *buf,int buf_len);
int read_zip_buffer(struct simulation *sim,char *file_name,long double **buf);
int hashget(long double *x,int N,long double find);
int gpvdm_fgets(char *buf,int len,FILE *file);
#endif
