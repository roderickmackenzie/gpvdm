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


