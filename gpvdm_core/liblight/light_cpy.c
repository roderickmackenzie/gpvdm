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

#include <enabled_libs.h>
#include <enabled_libs.h>
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

	#define _GNU_SOURCE
	#include <dlfcn.h>

static int unused __attribute__((unused));



void light_cpy_memory(struct simulation *sim,struct light *out,struct light *in)
{
	int w;
	struct dimensions *dim=&(in->dim);

	strcpy(out->mode,in->mode);
	strcpy(out->dump_dir,in->dump_dir);

	dim_light_cpy(&(out->dim),&(in->dim));
	printf("cpy light\n");
	//long double zxyl
	cpy_light_zxyl_float(dim,&(out->Ep),&(in->Ep));
	cpy_light_zxyl_float(dim,&(out->Epz),&(in->Epz));
	cpy_light_zxyl_float(dim,&(out->En),&(in->En));
	cpy_light_zxyl_float(dim,&(out->Enz),&(in->Enz));
	cpy_light_zxyl_float(dim,&(out->n),&(in->n));
	cpy_light_zxyl_float(dim,&(out->alpha0),&(in->alpha0));
	cpy_light_zxyl_float(dim,&(out->alpha),&(in->alpha));
	cpy_light_zxyl_double(dim,&(out->photons),&(in->photons));
	cpy_light_zxyl_double(dim,&(out->photons_asb),&(in->photons_asb));
	//cpy_light_zxyl_float(dim,&(out->pointing_vector),&(in->pointing_vector));
	//cpy_light_zxyl_float(dim,&(out->E_tot_r),&(in->E_tot_r));
	//cpy_light_zxyl_float(dim,&(out->E_tot_i),&(in->E_tot_i));
	cpy_light_zxyl_float(dim,&(out->H),&(in->H));

	//long double complex
	cpy_light_zxyl_float_complex(dim,&(out->t),&(in->t));
	cpy_light_zxyl_float_complex(dim,&(out->r),&(in->r));
	cpy_light_zxyl_float_complex(dim,&(out->nbar),&(in->nbar));

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
	cpy_light_l_long_double(dim,&(out->sun_y0),&(in->sun_y0));
	cpy_light_l_long_double(dim,&(out->sun_y1),&(in->sun_y1));
	cpy_light_l_long_double(dim,&(out->sun_photons_y0),&(in->sun_photons_y0));
	cpy_light_l_long_double(dim,&(out->sun_photons_y1),&(in->sun_photons_y1));

	//Input spectra no filter
	cpy_light_l_long_double(dim,&(out->sun_y0_no_filter),&(in->sun_y0_no_filter));
	cpy_light_l_long_double(dim,&(out->sun_y1_no_filter),&(in->sun_y1_no_filter));
	cpy_light_l_long_double(dim,&(out->sun_photons_y0_no_filter),&(in->sun_photons_y0_no_filter));
	cpy_light_l_long_double(dim,&(out->sun_photons_y1_no_filter),&(in->sun_photons_y1_no_filter));

	//Input field
	cpy_light_l_long_double(dim,&(out->sun_E_y0),&(in->sun_E_y0));
	cpy_light_l_long_double(dim,&(out->sun_E_y1),&(in->sun_E_y1));

	//matrix
	printf("%d\n",sim->server.worker_max);
	malloc_1d((void**)&(out->mx), in->worker_max, sizeof(struct matrix));
	malloc_1d((void**)&(out->msm), in->worker_max, sizeof(struct matrix_solver_memory));

	for (w=0;w<in->worker_max;w++)
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
	if (in->lib_handle!=NULL)
	{
		light_load_dlls(sim,out);
	}

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
	out->disable_transfer_to_electrical_mesh=in->disable_transfer_to_electrical_mesh;
	out->disable_cal_photon_density=in->disable_cal_photon_density;
	out->light_file_generation_shift=in->light_file_generation_shift;

	out->print_wavlengths=in->print_wavlengths;

	out->finished_solveing=in->finished_solveing;

	out->last_Psun=in->last_Psun;
	out->last_laser_eff=in->last_laser_eff;
	out->last_wavelength_laser=in->last_wavelength_laser;


	out->epi=in->epi;

	strcpy(out->light_profile,in->light_profile);

	strcpy(out->snapshot_path,in->snapshot_path);
	out->dump_verbosity=in->dump_verbosity;

	out->use_flat_sepctrum=in->use_flat_sepctrum;			
	//printf_log(sim,_("Freeing memory from the optical model\n"));
}
