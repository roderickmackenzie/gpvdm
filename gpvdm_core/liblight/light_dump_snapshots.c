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



/** @file light_dump.c
	@brief Dumps optical fields from the light model.
*/

#include <string.h>
#include <sys/stat.h>
#include "util.h"
#include "gpvdm_const.h"
#include "dump_ctrl.h"
#include "light.h"
#include "dat_file.h"
#include <cal_path.h>
#include <lang.h>
#include <light_fun.h>
#include <dump.h>



void light_dump_shapshots(struct simulation *sim,struct light *li)
{
	int l;
	FILE *out;
	char out_dir[PATH_MAX];
	struct dim_light *dim=&li->dim;

	struct dat_file buf;
	buffer_init(&buf);

	if (li->dump_verbosity==0)
	{
		for (l=0;l<dim->llen;l++)
		{
			dump_make_snapshot_dir_with_name(sim,out_dir ,(double)0.0, 0.0, dim->l[l] ,l,"optical_snapshots");

			buffer_malloc(&buf);
			dim_light_info_to_buf(&buf,dim);
			strcpy(buf.title,_("Electron generation rate"));
			strcpy(buf.type,"zxy-d");
			strcpy(buf.data_label,_("Generation rate"));
			strcpy(buf.data_units,"m^{-3}");
			buf.x=dim->xlen;
			buf.y=dim->ylen;
			buf.z=dim->zlen;
			buffer_add_info(sim,&buf);
			dat_file_add_zxy_from_zxyl_long_double_light_data(sim,&buf,li->photons_asb, dim,l);
			dat_file_add_zxy_long_double_light_data(sim,&buf,li->Gn, dim);
			buffer_dump_path(sim,out_dir,"light_Gn.dat",&buf);
			buffer_free(&buf);

			///////////
		}
	}

}

