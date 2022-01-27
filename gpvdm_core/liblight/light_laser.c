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
	struct dimensions *dim=&(li->dim);

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
			if (li->laser_wavelength<li->lstart)
			{
				ewe(sim,"fs laser wavelength smaller than minimum point on optical spectra min=%Le (m) laser=%Le (m) max=%Le (m)\n",li->lstart,li->laser_wavelength,li->lstop);
			}

			if (li->laser_wavelength>li->lstop)
			{
				ewe(sim,"fs laser wavelength smaller than maximum point on optical spectra min=%Le (m) laser=%Le (m) max=%Le (m)\n",li->lstart,li->laser_wavelength,li->lstop);
			}

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

