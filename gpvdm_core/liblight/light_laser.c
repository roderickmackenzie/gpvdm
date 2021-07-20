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

int light_load_laser(struct simulation *sim, struct light *li,char *name, struct json_obj *json_data)
{
	int i;
	int segments=0;
	long double laser_photon_efficiency=0.0;
	char temp[200];
	char temp2[200];
	char search_name[200];
	char laser_name_in_file[200];
	struct json_obj *json_lasers;
	struct json_obj *json_laser;
	struct json_obj *json_config;
	struct dim_light *dim=&(li->dim);

	str_to_lower(search_name, name);

	json_lasers=json_obj_find(json_data, "lasers");
	json_get_int(sim, json_lasers, &segments,"segments");
	for (i=0;i<segments;i++)
	{
		sprintf(temp,"segment%d",i);
		json_laser=json_obj_find(json_lasers, temp);
		json_get_string(sim, json_laser, temp2,"english_name");
		str_to_lower(laser_name_in_file, temp2);
		//printf("%s %s\n",laser_name_in_file,search_name);
		if (strcmp(laser_name_in_file,search_name)==0)
		{
			json_config=json_obj_find(json_laser, "config");
			json_get_long_double(sim, json_config, &(li->laser_wavelength),"laserwavelength");

			li->laser_pos=(int)((li->laser_wavelength-li->lstart)/dim->dl);

			json_get_long_double(sim, json_config, &(li->spotx),"spotx");
			li->spotx=gfabs(li->spotx);


			json_get_long_double(sim, json_config, &(li->spoty),"spoty");
			li->spoty=gfabs(li->spoty);

			json_get_long_double(sim, json_config, &(li->pulseJ),"pulseJ");
			li->pulseJ=gfabs(li->pulseJ);

			json_get_long_double(sim, json_config, &(laser_photon_efficiency),"laser_photon_efficiency");

			li->pulseJ=li->pulseJ*gfabs(laser_photon_efficiency);

			json_get_long_double(sim, json_config, &(li->pulse_width),"laser_pulse_width");
			return 0;
		}
	}

	ewe(sim,"laser name not %s found\n",search_name);
return -1;
}

