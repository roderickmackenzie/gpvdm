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

	struct dat_file buf;
	struct heat *thermal=&(dev->thermal);

	if (thermal->newton_enable_external_thermal==FALSE)
	{
		return;
	}

	dat_file_init(&buf);

	char out_dir[PATH_MAX];
	join_path(2, out_dir,outputpath,"dynamic");


	gpvdm_mkdir(out_dir);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, dev,&(store->J_y0_n));
	sprintf(buf.title,"%s",_("H_joule"));
	strcpy(buf.data_label,_("H_joule"));
	strcpy(buf.data_units,"W m^{-3}");
	dat_file_add_xy_data(sim,&buf,(store->H_joule).x, (store->H_joule).data, (store->H_joule).len);
	buffer_dump_path(sim,out_dir,"H_joule.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, dev,&(store->J_y0_n));
	sprintf(buf.title,"%s",_("H_recombination"));
	strcpy(buf.data_label,_("H_recombination"));
	strcpy(buf.data_units,"W m^{-3}");
	dat_file_add_xy_data(sim,&buf,(store->H_recombination).x, (store->H_recombination).data, (store->H_recombination).len);
	buffer_dump_path(sim,out_dir,"H_recombination.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, dev,&(store->J_y0_n));
	sprintf(buf.title,"%s",_("H_parasitic"));
	strcpy(buf.data_label,_("H_parasitic"));
	strcpy(buf.data_units,"W m^{-3}");
	dat_file_add_xy_data(sim,&buf,(store->H_parasitic).x, (store->H_parasitic).data, (store->H_parasitic).len);
	buffer_dump_path(sim,out_dir,"H_parasitic.dat",&buf);
	buffer_free(&buf);

}

