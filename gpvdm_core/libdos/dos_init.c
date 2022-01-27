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

void dos_init(struct dos *mydos)
{
	mydos->enabled=FALSE;
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

	//Auger
	mydos->auger_enabled=FALSE;
	mydos->Cn=-1.0;
	mydos->Cp=-1.0;

	//SS SRH
	mydos->ss_srh_enabled=FALSE;
	mydos->n1=-1.0;
	mydos->p1=-1.0;
	mydos->tau_n=-1.0;
	mydos->tau_p=-1.0;

	mydos->B=-1;

	mydos->mobility_symmetric=-1;
	struct dosconfig *config=&(mydos->config);


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
	config->Na0=-1.0;
	config->Na1=-1.0;
	config->Nd0=-1.0;
	config->Nd1=-1.0;

	config->Esteps=-1;

	//outside binary dos file
	config->ion_density=-1.0;
	config->ion_mobility=-1.0;
	strcpy(config->dos_name,"none");
}


