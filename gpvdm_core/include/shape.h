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


/** @file epitaxy.h
	@brief Read the epitaxy from the epitaxy.inp file.
*/


#ifndef shape_h
#define shape_h
#include "advmath.h"
#include <sim_struct.h>
#include <shape_struct.h>

int shape_load_file(struct simulation *sim,struct epitaxy *in,struct shape *s, char *file_name, long double y_pos);
int shape_get_index(struct simulation *sim,struct epitaxy *in,long double x,long double y,long double z);
void shape_free(struct simulation *sim,struct shape *s);
int shape_in_shape(struct simulation *sim,struct shape *s,long double z,long double x,long double y);
void shape_init(struct simulation *sim,struct shape *s);
void shape_load_materials(struct simulation *sim,struct shape *s);
void shape_cpy(struct simulation *sim,struct shape *out,struct shape *in);
int shape_load_from_json(struct simulation *sim,struct shape *s, struct json_obj *obj ,long double y_pos);
void shape_cal_min_max(struct simulation *sim,struct vec *min,struct vec *max,struct shape *s);
#endif
