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


/** @file light_memory.c
	@brief Deals with memory allocation for the light model.
*/
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "gpvdm_const.h"
#include "light.h"
#include "device.h"
#include "config.h"
#include "util.h"
#include "lang.h"
#include "log.h"
#include <light_fun.h>
#include <matrix.h>
#include <memory.h>
#include <device_fun.h>
#include <triangles.h>

static int unused __attribute__((unused));

void light_init(struct simulation *sim,struct light *li)
{
	struct dim_light *dim=&(li->dim);
	li->last_Psun= -1000.0;
	li->last_laser_eff= -1000.0;
	li->last_wavelength_laser= -1000.0;
	li->laser_wavelength= -1.0;
	li->laser_pos= -1;
	li->laser_wavelength= -1.0;
	li->lstart= -1.0;
	li->spotx= -1.0;
	li->spoty= -1.0;
	li->pulseJ= -1.0;
	li->pulse_width= -1.0;
	li->light_wavelength_auto_mesh=-1;
	li->disable_cal_photon_density=FALSE;
	li->finished_solveing=FALSE;
	li->print_wavlengths=TRUE;

	//output files
	li->dump_level=2;
	//strcpy(li->dump_dir,"none");

	//long double zxyl
	li->Ep=NULL;
	li->Epz=NULL;
	li->En=NULL;
	li->Enz=NULL;
	li->n=NULL;
	li->alpha0=NULL;
	li->alpha=NULL;
	li->photons=NULL;
	li->photons_asb=NULL;
	li->pointing_vector=NULL;
	li->E_tot_r=NULL;
	li->E_tot_i=NULL;
	li->H=NULL;

	//long double zxy
	li->Gn=NULL;
	li->Gp=NULL;
	li->Htot=NULL;
	li->photons_tot=NULL;

	//long double complex
	li->t=NULL;
	li->r=NULL;
	li->nbar=NULL;

	//zxy_p_object
	li->obj=NULL;

	//long double


	li->reflect=NULL;
	li->transmit=NULL;
	li->extract_eff=NULL;


	//Input spectra
	inter_init(sim,&(li->sun_read));
	li->sun=NULL;
	li->sun_norm=NULL;
	li->sun_photons=NULL;
	li->sun_E=NULL;
	li->filter=NULL;
	strcpy(li->suns_spectrum_file,"");
	strcpy(li->light_file_generation,"");

	li->lib_handle=NULL;

	//filter
	li->filter_enabled=FALSE;
	inter_init(sim,&(li->filter_read));
	strcpy(li->filter_path,"");
	
	strcpy(li->light_profile,"box");
	triangles_init((&(li->light_profile_tri)));

	dim_light_init(dim);
	li->mx=NULL;
	li->msm=NULL;
	strcpy(li->snapshot_path,"");
	li->dump_verbosity=0;
}

