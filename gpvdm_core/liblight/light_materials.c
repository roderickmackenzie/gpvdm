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

	struct dimensions *dim=&(li->dim);


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


