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

/** @file light_filter.c
	@brief Loads the optical filter
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

static int unused __attribute__((unused));

void light_load_filter(struct simulation *sim,struct light *li)
{

	int x;
	long double val;
	char filter_path[PATH_MAX];

	long double dB;

	struct inp_file inp;
	inp_init(sim,&inp);

	inp_load_from_path(sim,&inp,get_input_path(sim),"filter.inp");
	li->filter_enabled=inp_search_english(sim,&inp,"#filter_enabled");

	if (li->filter_enabled==TRUE)
	{
		inp_search_string(sim,&inp,filter_path,"#filter_material");
		inp_search_gdouble(sim,&inp,&(dB),"#filter_db");
		join_path(3,li->filter_path,get_materials_path(sim),filter_path,"alpha.gmat");

		if (isfile(li->filter_path)==0)
		{
			printf_log(sim,"%s %s\n",_("Light: Load filter"),li->filter_path);
			inter_init(sim,&(li->filter_read));
			inter_load(sim,&(li->filter_read),li->filter_path);
			inter_sort(&(li->filter_read));
			inter_mod(&(li->filter_read));
			inter_norm(&(li->filter_read),1.0);

			val=log10(20);
			for (x=0;x<li->filter_read.len;x++)
			{

				//printf("%Le %Le\n",li->filter_read.x[x],li->filter_read.data[x]);
				li->filter_read.data[x]=powl(10,-li->filter_read.data[x]*val);	//dB*li->filter_read.data[x]
				
				//printf("%Le %Le\n",li->filter_read.x[x],li->filter_read.data[x]);
			}
			//inter_dump(sim,&(filter));
			inter_save_a(&(li->filter_read),get_input_path(sim),"filtered_spectrum.dat");
			//exit(0);
		}
	}
	inp_free(sim,&inp);

}


