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

void dos_init(struct dos *mydos)
{
	mydos->used=FALSE;
	mydos->x=NULL;
	mydos->xlen=-1;
	mydos->tlen=-1;
	mydos->srh_bands=-1;
	mydos->t=NULL;
	mydos->srh_E=NULL;
	mydos->srh_den=NULL;
	mydos->c=NULL;
	mydos->w=NULL;
	mydos->srh_r1=NULL;
	mydos->srh_r2=NULL;
	mydos->srh_r3=NULL;
	mydos->srh_r4=NULL;
	mydos->srh_c=NULL;

	mydos->muz=-1;
	mydos->mux=-1;
	mydos->muy=-1;

	mydos->B=-1;

	mydos->mobility_symmetric=-1;
	struct dosconfig *config=&(mydos->config);

	char dos_name[20];
	char analytical_dos_file_name[20];
	config->dos_number=-1;
	config->dostype=-1;
	config->dos_free_carrier_stats=-1;
	config->Nt=-1.0;
	config->Et=-1.0;
	config->nstart=-1.0;
	config->nstop=-1.0;
	config->npoints=-1;

	config->Tstart=-1.0;
	config->Tstop=-1.0;
	config->Tsteps=-1;
	config->traps=-1;
	config->dband=-1.0;
	config->detrap=-1.0;
	config->srh_bands=-1;
	config->srh_start=-1.0;
	config->srh_stop=-1.0;


	config->srh_sigman=-1.0;
	config->srh_sigmap=-1.0; 
	config->srh_vth=-1.0;
	config->Nc=-1.0;
	config->Nv=-1.0;
	config->me=-1.0;
	config->mh=-1.0;

	config->Eg=-1.0;
	config->Xi=-1.0;
	config->epsilonr=-1.0;
	config->doping_start=-1.0;
	config->doping_stop=-1.0;

	config->Esteps=-1;

	//outside binary dos file
	config->ion_density=-1.0;
	config->ion_mobility=-1.0;
}


