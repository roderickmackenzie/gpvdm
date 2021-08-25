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

/** @file detector.h
@brief header file for the optical detectors
*/
#ifndef detector_h
#define detector_h
#include <enabled_libs.h>
#include <vec.h>
#include <i.h>
#include <sim_struct.h>
#include <triangle.h>
#include <dim.h>
#include <shape_struct.h>
#include <detector_struct.h>
#include <ray.h>
#include <vectors.h>

void detector_init(struct simulation *sim,struct detector *d);
void detector_free(struct simulation *sim,struct detector *d);
void detector_cpy(struct simulation *sim,struct detector *out,struct detector *in);
void detectors_init(struct simulation *sim,struct world *w);
void detectors_load(struct simulation *sim,struct world *w, struct json_obj *json_detectors);
void detectors_free(struct simulation *sim,struct world *w);
void detectors_cpy(struct simulation *sim,struct world *out,struct world *in);
void dectors_add_to_scene(struct simulation *sim,struct world *w, struct image *my_image,struct vec *min, struct vec *max);
#endif
