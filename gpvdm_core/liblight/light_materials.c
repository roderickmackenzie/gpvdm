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

/** @file light_materials.c
	@brief This loads in any physical spectra for the light model, not alpha/n data is stored in the epitaxy.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "util.h"
#include "gpvdm_const.h"
#include "light.h"
#include "device.h"
#include "gpvdm_const.h"
#include "dump.h"
#include "config.h"
#include "inp.h"
#include "util.h"
#include "hard_limit.h"
#include "epitaxy.h"
#include <lang.h>
#include "log.h"
#include <cal_path.h>
#include <dat_file.h>
#include <triangles.h>
#include <light_fun.h>

static int unused __attribute__((unused));

void light_load_materials(struct simulation *sim,struct light *li, struct device *dev)
{
	printf_log(sim,"%s\n",_("load: materials"));
	long double min;
	long double max;
	struct vec my_vec;
	char file_path[PATH_MAX];

	struct dim_light *dim=&(li->dim);


	if (li->light_wavelength_auto_mesh==TRUE)
	{
		dim->llen=100;	//100 wavelengths as default

		light_src_cal_min_max(sim,&min,&max,&(li->light_src_y0));
		light_src_cal_min_max(sim,&min,&max,&(li->light_src_y1));
		li->lstart=min;
		li->lstop=max;
	}

	dim->dl=(li->lstop-li->lstart)/((long double)dim->llen);

	if (strcmp(li->light_profile,"box")!=0)
	{
		join_path(3,file_path,get_shape_path(sim),li->light_profile,"shape.inp");
		triangle_load_from_file(sim,(&li->light_profile_tri),file_path);

		triangles_find_min(&my_vec,&li->light_profile_tri);
		triangles_sub_vec(&li->light_profile_tri,&my_vec);
		triangles_find_max(&my_vec,&li->light_profile_tri);

		my_vec.x=my_vec.x/dev->xlen;
		my_vec.z=my_vec.z/dev->zlen;
		triangles_div_vec(&li->light_profile_tri,&my_vec);
		triangles_save(sim,"test.dat",&li->light_profile_tri);
		//my_vec.x=0.00042;
		//my_vec.z=0.00042;
		//double mul=triangles_interpolate(&li->light_profile_tri,&my_vec);

		//printf("%le %le %le\n",my_vec.x,my_vec.z,mul);
		//getchar();
	}
}


