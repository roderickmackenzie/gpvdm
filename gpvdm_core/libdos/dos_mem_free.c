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

/** @file dos.c
	@brief Reads in the DoS files but does not generate them, also deals with interpolation.
*/

#include <enabled_libs.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

	#include <zlib.h>
#include "code_ctrl.h"
#include "server.h"
#include "sim.h"
#include "dump.h"
#include "lang.h"
#include "log.h"
#include "cal_path.h"
#include "util.h"
#include "inp.h"
#include <memory.h>


void dos_free(struct dos *mydos)
{

	free_3d((void ****)&(mydos->srh_r1), mydos->tlen, mydos->xlen, mydos->srh_bands, sizeof(long double));
	free_3d((void ****)&(mydos->srh_r2), mydos->tlen, mydos->xlen, mydos->srh_bands, sizeof(long double));
	free_3d((void ****)&(mydos->srh_r3), mydos->tlen, mydos->xlen, mydos->srh_bands, sizeof(long double));
	free_3d((void ****)&(mydos->srh_r4), mydos->tlen, mydos->xlen, mydos->srh_bands, sizeof(long double));
	free_3d((void ****)&(mydos->srh_c), mydos->tlen, mydos->xlen, mydos->srh_bands, sizeof(long double));

	free_2d_long_double(mydos->tlen, mydos->xlen, &(mydos->c));
	free_2d_long_double(mydos->tlen, mydos->xlen, &(mydos->w));

	free_1d( (void **)&(mydos->x),sizeof(long double));
	free_1d( (void **)&(mydos->t),sizeof(long double));
	free_1d( (void **)&(mydos->srh_E),sizeof(long double));
	free_1d( (void **)&(mydos->srh_den),sizeof(long double));

	mydos->xlen=0;
	mydos->tlen=0;
	mydos->srh_bands=0;


}


