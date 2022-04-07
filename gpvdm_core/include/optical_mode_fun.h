// 
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solardevs.
// The model can simulate OLEDs, Perovskite devs, and OFETs.
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


/** @file optical_mode_fun.h
	@brief Mode solver functions
*/


#ifndef h_optical_mode_fun
#define h_optical_mode_fun
#include <enabled_libs.h>
#include "advmath.h"
#include <sim_struct.h>
#include <device.h>
#include <dim.h>
#include <optical_mode.h>

int mode_free(struct simulation *sim,struct optical_mode *in);
void mode_init(struct simulation *sim,struct optical_mode *in);
int mode_dump(struct simulation *sim,struct device *dev,struct optical_mode *in,char *path);
int mode_cpy(struct simulation *sim,struct optical_mode *out,struct optical_mode *in);
int mode_load(struct simulation *sim,struct optical_mode *in,struct device *dev);
int mode_norm(struct simulation *sim,struct optical_mode *in,long double density);

#endif
