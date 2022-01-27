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
	struct dimensions *dim=&(li->dim);
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
	//li->pointing_vector=NULL;
	//li->E_tot_r=NULL;
	//li->E_tot_i=NULL;
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
	light_src_init(sim,&(li->light_src_y0));
	light_src_init(sim,&(li->light_src_y1));

	//Input spectra
	li->sun_y0=NULL;
	li->sun_y1=NULL;
	li->sun_photons_y0=NULL;
	li->sun_photons_y1=NULL;

	//Input spectra no filter
	li->sun_y0_no_filter=NULL;
	li->sun_y1_no_filter=NULL;
	li->sun_photons_y0_no_filter=NULL;
	li->sun_photons_y1_no_filter=NULL;

	//Input field
	li->sun_E_y0=NULL;
	li->sun_E_y1=NULL;
	strcpy(li->suns_spectrum_file,"");
	strcpy(li->light_file_generation,"");

	li->lib_handle=NULL;

	strcpy(li->light_profile,"box");
	triangles_init((&(li->light_profile_tri)));

	dim_light_init(dim);
	li->worker_max=-1;
	li->mx=NULL;
	li->msm=NULL;
	strcpy(li->snapshot_path,"none");
	li->dump_verbosity=0;
	strcpy(li->dump_dir,"none");

	//for EQE
	li->use_flat_sepctrum=FALSE;			

}

