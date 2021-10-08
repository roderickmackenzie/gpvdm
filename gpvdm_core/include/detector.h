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
void detector_dump_bins(struct simulation *sim,struct image *in);
#endif
