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

	//Auger
	out->auger_enabled=in->auger_enabled;
	out->Cn=in->Cn;
	out->Cp=in->Cp;

	//SS SRH
	out->ss_srh_enabled=in->ss_srh_enabled;
	out->n1=in->n1;
	out->p1=in->p1;
	out->tau_n=in->tau_n;
	out->tau_p=in->tau_p;

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


