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

/** @file dynamic_dump_j.c
@brief Dumps dynamic current density
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

void dump_dynamic_save_j(struct simulation *sim,struct device *in,char *outputpath,struct dynamic_store *store)
{
	int i;
	long double tot;

	int sub=TRUE;
	char temp[200];
	struct dat_file buf;
	struct math_xy one;

	dat_file_init(&buf);



	if (get_dump_status(sim,dump_norm_time_to_one)==TRUE)
	{
		buf.norm_x_axis=TRUE;
	}

	if (get_dump_status(sim,dump_norm_y_axis)==TRUE)
	{
		buf.norm_y_axis=TRUE;
	}

	char out_dir[1000];
	join_path(2, out_dir,outputpath,"dynamic");

	gpvdm_mkdir(out_dir);

	char outpath[200];

	//////////////////////////////////////////Current densities on contacts
	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->J_y0_n));
	sprintf(buf.title,"%s",_("Electron current density left contact (J_y0_n)"));
	strcpy(buf.data_label,_("Electron current density"));
	strcpy(buf.data_units,"A m^{-2}");
	dat_file_add_xy_data(sim,&buf,(store->J_y0_n).x, (store->J_y0_n).data, (store->J_y0_n).len);
	buffer_dump_path(sim,out_dir,"J_y0_n.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->J_y0_p));
	sprintf(buf.title,"%s",_("Hole current density left contact (J_y0_p)"));
	strcpy(buf.data_label,_("Hole current density"));
	strcpy(buf.data_units,"A m^{-2}");
	dat_file_add_xy_data(sim,&buf,(store->J_y0_p).x, (store->J_y0_p).data, (store->J_y0_p).len);
	buffer_dump_path(sim,out_dir,"J_y0_p.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->J_y1_n));
	sprintf(buf.title,"%s",_("Electron current density right contact (J_y1_n)"));
	strcpy(buf.data_label,_("Electron current density"));
	strcpy(buf.data_units,"A m^{-2}");
	dat_file_add_xy_data(sim,&buf,(store->J_y1_n).x, (store->J_y1_n).data, (store->J_y1_n).len);
	buffer_dump_path(sim,out_dir,"J_y1_n.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->J_y1_p));
	sprintf(buf.title,"%s",_("Hole current density right contact (J_y1_p)"));
	strcpy(buf.data_label,_("Hole current density"));
	strcpy(buf.data_units,"A m^{-2}");
	dat_file_add_xy_data(sim,&buf,(store->J_y1_p).x, (store->J_y1_p).data, (store->J_y1_p).len);
	buffer_dump_path(sim,out_dir,"J_y1_p.dat",&buf);
	buffer_free(&buf);

	//////////////////////////////////////////Drift diffusion current densities
	////////////////////

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->dynamic_jn));
	sprintf(buf.title,"%s",_("Jn at contacts"));
	strcpy(buf.type,"xy");
	strcpy(buf.data_label,_("Electron current density"));
	strcpy(buf.data_units,"A m^{-2}");
	dat_file_add_xy_data(sim,&buf,(store->dynamic_jn).x, (store->dynamic_jn).data, (store->dynamic_jn).len);
	buffer_dump_path(sim,out_dir,"jn_contacts.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->dynamic_jp));
	strcpy(buf.title,_("Jp at contacts"));
	strcpy(buf.data_label,_("Hole current density"));
	strcpy(buf.y_units,"\\mu s");
	strcpy(buf.data_units,"A m^{-2}");
	dat_file_add_xy_data(sim,&buf,(store->dynamic_jp).x, (store->dynamic_jp).data, (store->dynamic_jp).len);
	buffer_dump_path(sim,out_dir,"jp_contacts.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	dat_file_add_xy_data(sim,&buf,(store->jn_avg).x, (store->jn_avg).data, (store->jn_avg).len);
	buffer_dump_path(sim,out_dir,"jn_avg.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	dat_file_add_xy_data(sim,&buf,(store->jp_avg).x, (store->jp_avg).data, (store->jp_avg).len);
	buffer_dump_path(sim,out_dir,"jp_avg.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->iout));
	strcpy(buf.title,_("External Current"));
	strcpy(buf.data_label,_("Current"));
	strcpy(buf.data_units,"Amps");
	dat_file_add_xy_data(sim,&buf,(store->iout).x, (store->iout).data, (store->iout).len);
	join_path(3, outpath,outputpath,"dynamic","i.dat");
	buffer_free(&buf);

	buffer_malloc(&buf);
	dat_file_add_xy_data(sim,&buf,(store->jnout_mid).x, (store->jnout_mid).data, (store->jnout_mid).len);
	buffer_dump_path(sim,out_dir,"jn_mid.dat",&buf);
	buffer_free(&buf);


	inter_init(sim,&one);
	inter_copy(&one,&(store->jnout_mid),TRUE);
	inter_deriv(&one,&(store->jnout_mid));

	buffer_malloc(&buf);
	dat_file_add_xy_data(sim,&buf,one.x, one.data, one.len);
	buffer_dump_path(sim,out_dir,"djn.dat",&buf);
	buffer_free(&buf);

	inter_free(&one);

	buffer_malloc(&buf);
	dat_file_add_xy_data(sim,&buf,(store->jpout_mid).x, (store->jpout_mid).data, (store->jpout_mid).len);
	buffer_dump_path(sim,out_dir,"jp_mid.dat",&buf);
	buffer_free(&buf);

	inter_copy(&one,&(store->jpout_mid),TRUE);
	inter_deriv(&one,&(store->jpout_mid));
	buffer_malloc(&buf);
	dat_file_add_xy_data(sim,&buf,one.x, one.data, one.len);
	buffer_dump_path(sim,out_dir,"djp.dat",&buf);
	buffer_free(&buf);
	inter_free(&one);




	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->jout));
	strcpy(buf.title,_("Current density at contacts"));
	strcpy(buf.data_label,_("Current density"));
	strcpy(buf.data_units,"A m^{-2}");
	if (sub==TRUE)
	{
		inter_sub_long_double(&(store->jout),(store->jout).data[0]);
		math_xy_mul_long_double(&(store->jout),-1.0);
	}
	dat_file_add_xy_data(sim,&buf,(store->jout).x, (store->jout).data, (store->jout).len);
	buffer_dump_path(sim,out_dir,"j.dat",&buf);
	buffer_free(&buf);

}

