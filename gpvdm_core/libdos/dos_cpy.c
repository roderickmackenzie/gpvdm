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

/** @file dos_cpy.c
	@brief Copy a dos structure
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


void dos_cpy(struct dos *out,struct dos *in)
{

	out->enabled=in->enabled;
	out->xlen=in->xlen;
	out->tlen=in->tlen;
	out->srh_bands=in->srh_bands;

	cpy_3d_alloc((void ****)&(out->srh_r1), (void ****)&(in->srh_r1),out->tlen, out->xlen, out->srh_bands, sizeof(long double));

	cpy_3d_alloc((void ****)&(out->srh_r2), (void ****)&(in->srh_r2),out->tlen, out->xlen, out->srh_bands, sizeof(long double));

	cpy_3d_alloc((void ****)&(out->srh_r3), (void ****)&(in->srh_r3), out->tlen, out->xlen, out->srh_bands, sizeof(long double));

	cpy_3d_alloc((void ****)&(out->srh_r4), (void ****)&(in->srh_r4), out->tlen, out->xlen, out->srh_bands, sizeof(long double));

	cpy_3d_alloc((void ****)&(out->srh_c), (void ****)&(in->srh_c), out->tlen, out->xlen, out->srh_bands, sizeof(long double));

	cpy_2d_alloc((void ***)&(out->c), (void ***)&(in->c), out->tlen, out->xlen, sizeof(long double));

	cpy_2d_alloc((void ***)&(out->w), (void ***)&(in->w), out->tlen, out->xlen, sizeof(long double));

	cpy_1d_alloc((void **)&(out->x), (void **)&(in->x), in->xlen, sizeof (long double) );
	cpy_1d_alloc((void **)&(out->t), (void **)&(in->t), in->tlen, sizeof (long double) );
	cpy_1d_alloc((void **)&(out->srh_E), (void **)&(in->srh_E), in->srh_bands, sizeof (long double) );
	cpy_1d_alloc((void **)&(out->srh_den), (void **)&(in->srh_den), in->srh_bands,sizeof (long double) );

	out->muz=in->muz;
	out->mux=in->mux;
	out->muy=in->muy;

	out->B=in->B;

	out->mobility_symmetric=in->mobility_symmetric;

	struct dosconfig *config_out=&(out->config);
	struct dosconfig *config_in=&(in->config);

	strcpy(config_out->dos_name,config_in->dos_name);

	config_out->dos_number=config_in->dos_number;
	config_out->dostype=config_in->dostype;
	config_out->dos_free_carrier_stats=config_in->dos_free_carrier_stats;
	config_out->Nt=config_in->Nt;
	config_out->Et=config_in->Et;
	config_out->nstart=config_in->nstart;
	config_out->nstop=config_in->nstop;
	config_out->npoints=config_in->npoints;

	config_out->Tstart=config_in->Tstart;
	config_out->Tstop=config_in->Tstop;
	config_out->Tsteps=config_in->Tsteps;
	config_out->traps=config_in->traps;
	config_out->dband=config_in->dband;
	config_out->detrap=config_in->detrap;
	config_out->srh_bands=config_in->srh_bands;
	config_out->srh_start=config_in->srh_start;
	config_out->srh_stop=config_in->srh_stop;


	config_out->srh_sigman=config_in->srh_sigman;
	config_out->srh_sigmap=config_in->srh_sigmap; 
	config_out->srh_vth=config_in->srh_vth;
	config_out->Nc=config_in->Nc;
	config_out->Nv=config_in->Nv;
	config_out->me=config_in->me;
	config_out->mh=config_in->mh;

	config_out->Eg=config_in->Eg;
	config_out->Xi=config_in->Xi;
	config_out->epsilonr=config_in->epsilonr;
	config_out->Na0=config_in->Na0;
	config_out->Na0=config_in->Na0;
	config_out->Nd0=config_in->Nd0;
	config_out->Nd0=config_in->Nd0;

	config_out->Esteps=config_in->Esteps;

	//outside binary dos file
	config_out->ion_density=config_in->ion_density;
	config_out->ion_mobility=config_in->ion_mobility;




}


