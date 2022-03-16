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

/** @file json.h
@brief Json decoder
*/

#ifndef json_h
#define json_h
#define json_enabled

#include <sim_struct.h>
#include <md5.h>
#include <json_struct.h>




void json_obj_init(struct json_obj *obj);
void json_obj_realloc(struct json_obj *obj);
struct json_obj * json_obj_add(struct json_obj *obj,char *name,char *data);

void json_objs_free(struct json_obj *obj);
void gobble(struct json *j);

//used by decode
void json_free(struct json *j);
void json_init(struct json *j);
void get_name(char *out,struct json *j);
void get_value(char *out,struct json *j);
void json_dump_obj(struct json_obj *obj);
void json_dump(struct json_obj *obj,struct json_string *buf, int level);
void json_obj_all_free(struct json *j,struct json_obj *obj);
struct json_obj *json_obj_find(struct json_obj *obj, char *name);
int json_decode(struct json *j,struct json_obj *obj);
int json_load(struct simulation *sim,struct json *j,char *full_file_name);
int json_get_string(struct simulation *sim,struct json_obj *obj, char *out,char *name);
int json_get_int(struct simulation *sim,struct json_obj *obj, int *out,char *name);
int json_get_english(struct simulation *sim,struct json_obj *obj, int *out,char *name);
int json_get_long_double(struct simulation *sim, struct json_obj *obj, long double *out,char *name);
struct json_obj *json_obj_find_by_path(struct simulation *sim,struct json_obj *obj, char *path);
int json_save(struct simulation *sim,struct json *j);
void json_cpy(struct simulation *sim,struct json *out,struct json *in);
void json_obj_cpy_data(struct simulation *sim,struct json_obj *out,struct json_obj *in);
void json_obj_cpy(struct simulation *sim,struct json_obj *out,struct json_obj *in);
int json_load_from_path(struct simulation *sim,struct json *j,char *path,char *file_name);
void json_chk_sum(struct simulation *sim,struct md5 *sum,struct json_obj *json_in);
int json_checksum_check(struct simulation *sim,char *out_check_sum,char *chk_file,struct json_obj *json_in);
int json_get_double(struct simulation *sim,struct json_obj *obj, double *out,char *name);
int json_get_float(struct simulation *sim,struct json_obj *obj, float *out,char *name);
struct json_obj *json_find_sim_struct(struct simulation *sim, struct json *j,char *sim_command);
void remove_comma(struct json_string  *buf);
void json_dump_to_string(struct json_string  *buf,struct json_obj *obj,int level);
int json_save_as(struct simulation *sim,char *file_name,struct json *j);
int json_obj_save_as(struct simulation *sim,char *file_name,struct json_obj *j);

void json_string_init(struct json_string *in);
void json_string_free(struct json_string *in);
void json_cat_str(struct json_string *buf,char *data);
int json_is_token(struct simulation *sim,struct json_obj *obj,char *name);
void json_from_buffer(struct simulation *sim,struct json *j,char *buf,int len);
int json_get_long_long(struct simulation *sim,struct json_obj *obj, long long *out,char *name);
#endif
