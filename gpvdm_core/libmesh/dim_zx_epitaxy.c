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

/** @file dim_zx_epitaxy.c
@brief Dimension object for zx_epitaxy
*/

#include <device.h>
#include "mesh.h"
#include "inp.h"
#include "util.h"
#include "gpvdm_const.h"
#include "hard_limit.h"
#include <log.h>
#include <cal_path.h>
#include <lang.h>
#include <shape.h>
#include <device_fun.h>
#include <math.h>

void dim_init_zx_epitaxy(struct dim_zx_epitaxy *dim)
{
	dim->zlen=-1;
	dim->xlen=-1;
	dim->ylen=-1;
}

void dim_free_zx_epitaxy(struct dim_zx_epitaxy *dim)
{
	dim_init_zx_epitaxy(dim);
}

void dim_alloc_zx_epitaxy(struct dim_zx_epitaxy *dim,struct device *dev)
{
	struct dimensions *dev_dim=&(dev->ns.dim);
	struct epitaxy *epi=&(dev->my_epitaxy);
	dim->xlen=dev_dim->xlen;
	dim->zlen=dev_dim->zlen;
	dim->ylen=epi->layers;

}


