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

/** @file light_laser.c
	@brief Deals with lasers for the light model.
*/

#include <unistd.h>
#include "gpvdm_const.h"
#include "light.h"
#include "device.h"
#include "gpvdm_const.h"
#include "dump.h"
#include "config.h"
#include "inp.h"
#include "util.h"
#include "hard_limit.h"
#include "lang.h"
#include "log.h"
#include <cal_path.h>

static int unused __attribute__((unused));

int light_load_laser(struct simulation *sim, struct light *li,char *name)
{
	char pwd[PATH_MAX];
	char file_name[255];
	struct inp_file inp;
	int ret=0;
	long double laser_photon_efficiency=0.0;
	struct dim_light *dim=&(li->dim);

	if (getcwd(pwd,1000)==NULL)
	{
		ewe(sim,"IO error\n");
	}

	ret=search_for_token(sim,file_name,pwd,"#laser_name",name);

	if (ret==0)
	{
		inp_init(sim,&inp);
		inp_load_from_path(sim,&inp,get_input_path(sim),file_name);
		inp_check(sim,&inp,1.0);

		inp_search_gdouble(sim,&inp,&li->laser_wavelength,"#laserwavelength");
		li->laser_pos=(int)((li->laser_wavelength-li->lstart)/dim->dl);

		inp_search_gdouble(sim,&inp,&li->spotx,"#spotx");
		li->spotx=gfabs(li->spotx);

		inp_search_gdouble(sim,&inp,&li->spoty,"#spoty");
		li->spoty=gfabs(li->spoty);

		inp_search_gdouble(sim,&inp,&li->pulseJ,"#pulseJ");
		li->pulseJ=gfabs(li->pulseJ);

		inp_search_gdouble(sim,&inp,&laser_photon_efficiency,"#laser_photon_efficiency");

		//printf("%Le\n",li->pulseJ);

		li->pulseJ=li->pulseJ*gfabs(laser_photon_efficiency);

		//printf("%Le\n",li->pulseJ);
		//getchar();
		inp_search_gdouble(sim,&inp,&li->pulse_width,"#laser_pulse_width");

		inp_free(sim,&inp);
		printf_log(sim,"Loaded laser\n");
	}else
	{
		ewe(sim,"laser name not found\n");
	}
return 0;
}

