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

/** @file hash.c
	@brief Hashing function for fast lookup in tables, but the arrays are now linear so you don't need to hash.
*/

//<stripall>

#include <stdio.h>
#include "sim.h"
#include "inp.h"
#include "cal_path.h"
#include "list.h"
#include "md5.h"

void newton_state_hash(struct simulation *sim,char *out,struct newton_state *ns)
{
	struct md5 hash;
	md5_init(&hash);
	zxy_hash_gdouble(&hash,struct dimensions *dim, long double ***var);

	ns->phi=NULL;

	ns->x=NULL;
	ns->xp=NULL;

	ns->xt=NULL;
	ns->xpt=NULL;

	ns->last_ittr= -1;
	ns->last_error= -1.0;

	dim_init(&(ns->dim));



	int i=0;
	char *buffer;
	unsigned int len;
	long l;
	char newcheck[100];
	struct list files;
	struct inp_file inp;
	inp_listdir(sim, get_input_path(sim),&files);


	for (i=0;i<files.len;i++)
	{
		do_md5=FALSE;

		if (strcmp(files.names[i],"light.inp")==0)
		{
			do_md5=TRUE;
		}

		if (is_numbered_file(files.names[i],"dos")==0)
		{
			do_md5=TRUE;
		}

		if (strcmp(files.names[i],"mesh_x.inp")==0)
		{
			do_md5=TRUE;
		}

		if (strcmp(files.names[i],"mesh_y.inp")==0)
		{
			do_md5=TRUE;
		}

		if (strcmp(files.names[i],"mesh_z.inp")==0)
		{
			do_md5=TRUE;
		}

		if (strcmp(files.names[i],"epitaxy.inp")==0)
		{
			do_md5=TRUE;
		}

		if (do_md5==TRUE)
		{
			inp_read_buffer(sim,&buffer, &l,files.names[i]);
			len=(unsigned int)l;
			
			md5_update(&sum,buffer,len);

			free(buffer);
		}else
		if (strcmp(files.names[i],"contacts.inp")==0)
		{
			inp_init(sim,&inp);
			inp_load(sim,&inp,files.names[i]);
			inp_replace(sim,&inp,"#contact_voltage0", "");
			inp_replace(sim,&inp,"#contact_voltage1", "");
			inp_replace(sim,&inp,"#contact_voltage2", "");
			inp_replace(sim,&inp,"#contact_voltage3", "");
			inp_replace(sim,&inp,"#contact_voltage4", "");
			inp_replace(sim,&inp,"#contact_voltage5", "");
			inp_replace(sim,&inp,"#contact_voltage6", "");
			inp_replace(sim,&inp,"#contact_voltage7", "");
			md5_update(&sum,inp.data,inp.fsize);
			inp_free_no_save(sim,&inp);
		}

		/*if (strcmp(files.names[i],"light.inp")==0)
		{
			inp_init(sim,&inp);
			inp_load(sim,&inp,files.names[i]);
			md5_update(&sum,inp.data,inp.fsize);
			inp_free_no_save(sim,&inp);
		}*/

	}


	list_free(&files);

	md5_to_str(out,&sum);
}

