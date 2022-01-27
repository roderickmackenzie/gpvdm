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

/** @file dump_map.c
@brief dump a map charge carrier density map of the device, not really used any more due to python back end needs rewritign anyway
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include "sim.h"
#include "dump.h"
#include "dat_file.h"
#include <cal_path.h>
#include <exp.h>
#include <lang.h>
#include <i.h>
#include <math_2d.h>
#include <memory.h>
#include <contacts.h>

static int unused __attribute__((unused));


void dump_device_map(struct simulation *sim,char* out_dir,struct device *in)
{
	struct dimensions *dim=&in->ns.dim;
	struct snapshots snap;
	struct dat_file buf;
	char temp[200];
	char shapshot_path[PATH_MAX];

	dat_file_init(&buf);

	snapshots_init(&snap);

	if (in->circuit_simulation==TRUE)
	{
		return;
	}

	strcpy(snap.type,"snapshots");
	strcpy(snap.plot_type,"trap_map");
	strcpy(snap.name,"trap_map");
	strcpy(snap.path,out_dir);

	dump_make_snapshot_dir(sim, shapshot_path, in->snapshot_number,&snap);


	buffer_malloc(&buf);

	sprintf(temp,"{\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"\t\"voltage\":%Le,\n",(long double )contact_get_active_contact_voltage(sim,in));
	buffer_add_string(&buf,temp);

	sprintf(temp,"\t\"time\":%Le,\n",(long double )in->time);
	buffer_add_string(&buf,temp);
	buffer_add_string(&buf,"\t\"item_type\":\"snapshots_sub_dir\"\n");
	sprintf(temp,"}");
	buffer_add_string(&buf,temp);

	buffer_dump_path(sim,shapshot_path,"data.json",&buf);
	buffer_free(&buf);

	if (buffer_set_file_name(sim,in,&buf,"trap_map.csv")==0)
	{
		buffer_malloc(&buf);
		dim_info_to_buf(&buf,dim);
		strcpy(buf.title,"Energy - trap ocupation");
		strcpy(buf.type,"trap_map");
		strcpy(buf.y_label,"Energy");
		strcpy(buf.data_label,"Ocupation");
		strcpy(buf.y_units,"eV");
		strcpy(buf.data_units,"m^{-3} eV^{-1}");
		buf.time=in->time;
		buf.Vexternal=contact_get_active_contact_voltage(sim,in);
		dat_file_add_zxyt_long_double(sim,&buf,in);
		buffer_dump_path(sim,shapshot_path,NULL,&buf);
		buffer_free(&buf);
	}

}



