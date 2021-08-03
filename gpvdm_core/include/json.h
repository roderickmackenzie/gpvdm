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

/** @file json.h
@brief Json decoder
*/

#ifndef json_h
#define json_h
#include <sim_struct.h>
#include <md5.h>

#define json_enabled

struct json_obj
{
	char type;
	char name[100];
	int len;
	int max_len;
	void *objs;

	char *data;
	int data_len;
	int node;
};

struct json_string
{
	char *data;
	int len;
	int pos;
	int compact;
};

struct json
{
	char *raw_data;
	long raw_data_len;
	int pos;
	int level;
	char path[PATH_MAX];
	struct json_obj obj;
	char file_path[PATH_MAX];
	int compact;
};

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
#endif
