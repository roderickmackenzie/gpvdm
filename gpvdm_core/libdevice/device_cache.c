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


/** @file device.c
	@brief Initialize the device structure.
*/

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#include <stdio.h>
#include <device.h>
#include <string.h>
#include <dump.h>
#include <mesh.h>
#include <ray_fun.h>
#include <newton_tricks.h>
#include <memory.h>
#include <circuit.h>
#include <shape.h>
#include <heat.h>
#include <heat_fun.h>
#include <gpvdm_const.h>
#include <lib_fxdomain.h>
#include <fxdomain_fun.h>

static int unused __attribute__((unused));
static char* unused_pchar __attribute__((unused));

void device_cache_init(struct solver_cache *cache)
{
	strcpy(cache->hash,"");
	cache->length=-1;
	cache->enabled=-1;
}

void device_cache_cpy(struct solver_cache *out,struct solver_cache *in)
{
	strcpy(out->hash,in->hash);
	out->length=in->length;
	out->enabled=in->enabled;
}

