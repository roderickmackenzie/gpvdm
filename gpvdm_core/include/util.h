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

/** @file util.h
@brief util functions
*/


#ifndef h_util
#define h_util
#include <enabled_libs.h>
#include <stdio.h>
#include <sim_struct.h>


#include <colors.h>

void set_ewe_lock_file(char *lockname,char *data);
void print_hex(struct simulation *sim,unsigned char *data);
void remove_dir(struct simulation *sim,char* dir_name);
int ewe(struct simulation *sim, const char *format, ...);
void write_lock_file( struct simulation *sim);
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
void str_to_lower(char *out, char *in);

//zip_buffer
void write_zip_buffer(struct simulation *sim,char *outfile,long double *buf,int buf_len);
int read_zip_buffer(struct simulation *sim,char *file_name,long double **buf);
int hashget(long double *x,int N,long double find);
int gpvdm_fgets(char *buf,int len,FILE *file);
#endif
