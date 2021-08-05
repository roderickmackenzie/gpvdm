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

/** @file dynamic_dump_thermal.c
@brief Dumps dynamic thermal stats
*/

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <i.h>
#include <exp.h>
#include <dos.h>
#include "sim.h"
#include "dump.h"
#include "dat_file.h"
#include "dynamic_store.h"
#include "memory.h"
#include "contacts.h"
#include <lang.h>
#include <cal_path.h>


static int unused __attribute__((unused));

void dump_dynamic_save_thermal(struct simulation *sim,struct device *dev,char *outputpath,struct dynamic_store *store)
{

	int i;
	long double tot;
	int sub=TRUE;
	char temp[200];
	struct dat_file buf;
	struct math_xy one;
	struct heat *thermal=&(dev->thermal);

	if (thermal->newton_enable_external_thermal==FALSE)
	{
		return;
	}

	buffer_init(&buf);

	char out_dir[1000];
	join_path(2, out_dir,outputpath,"dynamic");
	struct stat st = {0};


	buffer_add_dir(sim,out_dir);

	char outpath[200];


	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, dev,&(store->J_y0_n));
	sprintf(buf.title,"%s",_("H_joule"));
	strcpy(buf.data_label,_("H_joule"));
	strcpy(buf.data_units,"W m^{-3}");
	buffer_add_info(sim,&buf);
	buffer_add_xy_data(sim,&buf,(store->H_joule).x, (store->H_joule).data, (store->H_joule).len);
	buffer_dump_path(sim,out_dir,"H_joule.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, dev,&(store->J_y0_n));
	sprintf(buf.title,"%s",_("H_recombination"));
	strcpy(buf.data_label,_("H_recombination"));
	strcpy(buf.data_units,"W m^{-3}");
	buffer_add_info(sim,&buf);
	buffer_add_xy_data(sim,&buf,(store->H_recombination).x, (store->H_recombination).data, (store->H_recombination).len);
	buffer_dump_path(sim,out_dir,"H_recombination.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, dev,&(store->J_y0_n));
	sprintf(buf.title,"%s",_("H_parasitic"));
	strcpy(buf.data_label,_("H_parasitic"));
	strcpy(buf.data_units,"W m^{-3}");
	buffer_add_info(sim,&buf);
	buffer_add_xy_data(sim,&buf,(store->H_parasitic).x, (store->H_parasitic).data, (store->H_parasitic).len);
	buffer_dump_path(sim,out_dir,"H_parasitic.dat",&buf);
	buffer_free(&buf);

}

