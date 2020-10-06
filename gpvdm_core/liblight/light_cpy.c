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
#include <sim.h>

	#include <dlfcn.h>

static int unused __attribute__((unused));



void light_cpy_memory(struct simulation *sim,struct light *out,struct light *in)
{
	int w;
	struct dim_light *dim=&(in->dim);

	strcpy(out->mode,in->mode);
	strcpy(out->config_file,in->config_file);
	strcpy(out->dump_dir,in->dump_dir);
	out->dump_level=in->dump_level;

	dim_light_cpy(&(out->dim),&(in->dim));

	//long double zxyl
	cpy_light_zxyl_long_double(dim,&(out->Ep),&(in->Ep));
	cpy_light_zxyl_long_double(dim,&(out->Epz),&(in->Epz));
	cpy_light_zxyl_long_double(dim,&(out->En),&(in->En));
	cpy_light_zxyl_long_double(dim,&(out->Enz),&(in->Enz));
	cpy_light_zxyl_long_double(dim,&(out->n),&(in->n));
	cpy_light_zxyl_long_double(dim,&(out->alpha0),&(in->alpha0));
	cpy_light_zxyl_long_double(dim,&(out->alpha),&(in->alpha));
	cpy_light_zxyl_long_double(dim,&(out->photons),&(in->photons));
	cpy_light_zxyl_long_double(dim,&(out->photons_asb),&(in->photons_asb));
	cpy_light_zxyl_long_double(dim,&(out->pointing_vector),&(in->pointing_vector));
	cpy_light_zxyl_long_double(dim,&(out->E_tot_r),&(in->E_tot_r));
	cpy_light_zxyl_long_double(dim,&(out->E_tot_i),&(in->E_tot_i));
	cpy_light_zxyl_long_double(dim,&(out->H),&(in->H));

	//long double complex
	cpy_light_zxyl_long_double_complex(dim,&(out->t),&(in->t));
	cpy_light_zxyl_long_double_complex(dim,&(out->r),&(in->r));
	cpy_light_zxyl_long_double_complex(dim,&(out->nbar),&(in->nbar));

	//zxy_p_object
	cpy_light_zxy_p_object(dim, &(out->obj), &(in->obj));

	//long double zxy
	cpy_light_zxy_long_double(dim,&(out->Gn),&(in->Gn));
	cpy_light_zxy_long_double(dim,&(out->Gp),&(in->Gp));
	cpy_light_zxy_long_double(dim,&(out->Htot),&(in->Htot));
	cpy_light_zxy_long_double(dim,&(out->photons_tot),&(in->photons_tot));

	//1D arrays
	cpy_light_l_long_double(dim,&(out->reflect),&(in->reflect));
	cpy_light_l_long_double(dim,&(out->transmit),&(in->transmit));

	//Input spectra
	inter_copy(&(out->sun_read),&(in->sun_read),TRUE);
	cpy_light_l_long_double(dim,&(out->sun),&(in->sun));
	cpy_light_l_long_double(dim,&(out->sun_norm),&(in->sun_norm));
	cpy_light_l_long_double(dim,&(out->sun_photons),&(in->sun_photons));
	cpy_light_l_long_double(dim,&(out->sun_E),&(in->sun_E));
	cpy_light_l_long_double(dim,&(out->filter),&(in->filter));

	//matrix
	malloc_1d((void**)&(out->mx), sim->server.worker_max, sizeof(struct matrix));
	malloc_1d((void**)&(out->msm), sim->server.worker_max, sizeof(struct matrix_solver_memory));

	for (w=0;w<sim->server.worker_max;w++)
	{
		matrix_cpy(sim,&(out->mx[w]),&(in->mx[w]));
		matrix_solver_memory_init(&(out->msm[w]));
	}

	//laser
	out->laser_wavelength=in->laser_wavelength;
	out->laser_pos=in->laser_pos;
	out->ND=in->ND;
	out->spotx=in->spotx;
	out->spoty=in->spoty;
	out->pulseJ=in->pulseJ;
	out->pulse_width=in->pulse_width;

	out->Psun=in->Psun;
	out->laser_eff=in->laser_eff;
	out->simplephotondensity=in->simplephotondensity;
	out->simple_alpha=in->simple_alpha;
	out->Dphotoneff=in->Dphotoneff;

	//Dll section
	light_load_dlls(sim,out);

	//config
	out->lstart=in->lstart;
	out->lstop=in->lstop;
	strcpy(out->mode,in->mode);
	out->electron_eff=in->electron_eff;
	out->hole_eff=in->hole_eff;

	out->force_update=in->force_update;
	out->light_wavelength_auto_mesh=in->light_wavelength_auto_mesh;
	cpy_light_l_long_double(dim,&(out->extract_eff),&(in->extract_eff));

	//Config values
	out->align_mesh=in->align_mesh;
	out->flip_field=in->flip_field;
	out->disable_transfer_to_electrical_mesh=in->disable_transfer_to_electrical_mesh;
	out->disable_cal_photon_density=in->disable_cal_photon_density;
	out->light_file_generation_shift=in->light_file_generation_shift;

	//filter
	out->filter_enabled=in->filter_enabled;
	strcpy(out->filter_path,in->filter_path);
	inter_copy(&(out->filter_read),&(in->filter_read),TRUE);

	out->print_wavlengths=in->print_wavlengths;

	out->finished_solveing=in->finished_solveing;

	out->last_Psun=in->last_Psun;
	out->last_laser_eff=in->last_laser_eff;
	out->last_wavelength_laser=in->last_wavelength_laser;


	out->epi=in->epi;

	strcpy(out->light_profile,in->light_profile);

	strcpy(out->snapshot_path,in->snapshot_path);
	out->dump_verbosity=in->dump_verbosity;

	//printf_log(sim,_("Freeing memory from the optical model\n"));
}
