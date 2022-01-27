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

/** @file dump_dynamic.c
@brief setup the dynamic dump stuff, this enables things like average charge density to be stored for each simulation stip and then wrtten to disk
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

void dynamic_info_to_buf(struct simulation *sim,struct dat_file *buf, struct device *dev,struct math_xy* data)
{
	char unit[100];
	long double mul;
	strcpy(buf->type,"xy");

	if (dev->go_time==TRUE)
	{
		strcpy(buf->y_label,_("Time"));
		get_time_dim(unit,&mul,data->x[data->len-1]);
		strcpy(buf->y_units,unit);
		buf->y_mul=mul;
	}else
	{
		strcpy(buf->y_label,_("Voltage"));
		strcpy(buf->y_units,"V");
		buf->y_mul=1.0;
	}


	buf->logscale_x=0;
	buf->logscale_y=0;
	buf->x=1;
	buf->y=data->len;
	buf->z=1;
}


void dump_dynamic_save(struct simulation *sim,struct device *in,char *outputpath,struct dynamic_store *store)
{
char out_dir[PATH_MAX];

if (in->circuit_simulation==TRUE)
{
	return;
}

struct dat_file buf;

dat_file_init(&buf);

if (get_dump_status(sim,dump_dynamic)==TRUE)
{

	if (get_dump_status(sim,dump_norm_time_to_one)==TRUE)
	{
		buf.norm_x_axis=TRUE;
	}

	if (get_dump_status(sim,dump_norm_y_axis)==TRUE)
	{
		buf.norm_y_axis=TRUE;
	}


	join_path(2, out_dir,outputpath,"dynamic");

	gpvdm_mkdir(out_dir);


	//////////////////////////////////////////recombination

	if (buffer_set_file_name(sim,in,&buf,"R_nfree_to_pfree.csv")==0)
	{
		buffer_malloc(&buf);
		buf.data_mul=1.0;
		dynamic_info_to_buf(sim,&buf, in,&(store->R_nfree_to_pfree));
		sprintf(buf.title,"%s - %s",_("Free electron -> Free hole recombination rate"),_("time"));
		strcpy(buf.data_label,_("Free carrier recombination rate"));
		strcpy(buf.data_units,"m^{-3}s^{-1}");
		dat_file_add_xy_data(sim,&buf,(store->R_nfree_to_pfree).x, (store->R_nfree_to_pfree).data, (store->R_nfree_to_pfree).len);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"R_nfree_to_pfree_norm.csv")==0)
	{
		buffer_malloc(&buf);
		buf.data_mul=1.0;
		dynamic_info_to_buf(sim,&buf, in,&(store->R_nfree_to_pfree));
		sprintf(buf.title,"%s - %s",_("Free electron -> Free hole recombination rate"),_("time"));
		strcpy(buf.data_label,_("Free carrier recombination rate"));
		strcpy(buf.data_units,"m^{-3}s^{-1}");
		inter_norm(&(store->R_nfree_to_pfree),1.0);
		dat_file_add_xy_data(sim,&buf,(store->R_nfree_to_pfree).x, (store->R_nfree_to_pfree).data, (store->R_nfree_to_pfree).len);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"R_srh_nfree.csv")==0)
	{
		buffer_malloc(&buf);
		buf.data_mul=1.0;
		dynamic_info_to_buf(sim,&buf, in,&(store->R_srh_nfree));
		sprintf(buf.title,"%s - %s",_("Free electron loss due to SRH processes (including trapping)"),_("time"));
		strcpy(buf.data_label,_("Free electron loss"));
		strcpy(buf.data_units,"m^{-3}s^{-1}");
		dat_file_add_xy_data(sim,&buf,(store->R_srh_nfree).x, (store->R_srh_nfree).data, (store->R_srh_nfree).len);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"R_srh_pfree.csv")==0)
	{
		buffer_malloc(&buf);
		buf.data_mul=1.0;
		dynamic_info_to_buf(sim,&buf, in,&(store->R_srh_pfree));
		sprintf(buf.title,"%s - %s",_("Free hole loss due to SRH processes (including trapping)"),_("time"));
		strcpy(buf.data_label,_("Free hole loss"));
		strcpy(buf.data_units,"m^{-3}s^{-1}");
		dat_file_add_xy_data(sim,&buf,(store->R_srh_pfree).x, (store->R_srh_pfree).data, (store->R_srh_pfree).len);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"T_srh_nfree_to_ntrap.csv")==0)
	{
		buffer_malloc(&buf);
		buf.data_mul=1.0;
		dynamic_info_to_buf(sim,&buf, in,&(store->T_srh_nfree_to_ntrap));
		strcpy(buf.title,_("SRH relaxation free electrons -> trapped electrons"));
		strcpy(buf.data_label,_("Relaxation"));
		strcpy(buf.data_units,"m^{-3}s^{-1}");
		dat_file_add_xy_data(sim,&buf,(store->T_srh_nfree_to_ntrap).x, (store->T_srh_nfree_to_ntrap).data, (store->T_srh_nfree_to_ntrap).len);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"T_srh_pfree_to_ptrap.csv")==0)
	{
		buffer_malloc(&buf);
		buf.data_mul=1.0;
		dynamic_info_to_buf(sim,&buf, in,&(store->T_srh_pfree_to_ptrap));
		strcpy(buf.title,_("SRH relaxation free holes -> trapped holes"));
		strcpy(buf.data_label,_("Relaxation"));
		strcpy(buf.data_units,"m^{-3}s^{-1}");
		dat_file_add_xy_data(sim,&buf,(store->T_srh_pfree_to_ptrap).x, (store->T_srh_pfree_to_ptrap).data, (store->T_srh_pfree_to_ptrap).len);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->G_n));
	strcpy(buf.title,_("Free electron generation rate"));
	strcpy(buf.data_label,_("Generation rate"));
	strcpy(buf.data_units,"m^{-3}s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->G_n).x, (store->G_n).data, (store->G_n).len);
	buffer_dump_path(sim,out_dir,"G_n.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->G_p));
	strcpy(buf.title,_("Free hole generation rate"));
	strcpy(buf.data_label,_("Generation rate"));
	strcpy(buf.data_units,"m^{-3}s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->G_p).x, (store->G_p).data, (store->G_p).len);
	buffer_dump_path(sim,out_dir,"G_p.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->R_srh_nfree_to_ptrap));
	strcpy(buf.title,_("SRH recombination free electrons -> trapped holes"));
	strcpy(buf.data_label,_("Recombination"));
	strcpy(buf.data_units,"m^{-3}s^{-1}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	dat_file_add_xy_data(sim,&buf,(store->R_srh_nfree_to_ptrap).x, (store->R_srh_nfree_to_ptrap).data,(store->R_srh_nfree_to_ptrap).len);
	buffer_dump_path(sim,out_dir,"R_srh_nfree_to_ptrap.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->R_srh_pfree_to_ntrap));
	strcpy(buf.title,_("SRH recombination free holes -> trapped electrons"));
	strcpy(buf.data_label,_("Recombination"));
	strcpy(buf.data_units,"m^{-3}s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->R_srh_pfree_to_ntrap).x, (store->R_srh_pfree_to_ntrap).data,(store->R_srh_pfree_to_ntrap).len);
	buffer_dump_path(sim,out_dir,"R_srh_pfree_to_ntrap.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->R_surface_y0));
	strcpy(buf.title,_("Surface recombination on left contact"));
	strcpy(buf.data_label,_("Recombination"));
	strcpy(buf.data_units,"m^{-3}s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->R_surface_y0).x, (store->R_surface_y0).data,(store->R_surface_y0).len);
	buffer_dump_path(sim,out_dir,"R_surface_y0.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->R_surface_y1));
	strcpy(buf.title,_("Surface recombination on right contact"));
	strcpy(buf.data_label,_("Recombination"));
	strcpy(buf.data_units,"m^{-3}s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->R_surface_y1).x, (store->R_surface_y1).data,(store->R_surface_y1).len);
	buffer_dump_path(sim,out_dir,"R_surface_y1.dat",&buf);
	buffer_free(&buf);

	//////////////////////////////////////////charge


	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->Q_ntrap));
	strcpy(buf.title,_("Trapped electron density"));
	strcpy(buf.data_label,_("Electron density"));
	strcpy(buf.data_units,"$m^{-3}$");
	dat_file_add_xy_data(sim,&buf,(store->Q_ntrap).x, (store->Q_ntrap).data, (store->Q_ntrap).len);
	buffer_dump_path(sim,out_dir,"Q_ntrap.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->Q_ptrap));
	strcpy(buf.title,_("Trapped hole density"));
	strcpy(buf.data_label,_("Hole density"));
	strcpy(buf.data_units,"m^{-3}");
	dat_file_add_xy_data(sim,&buf,(store->Q_ptrap).x, (store->Q_ptrap).data, (store->Q_ptrap).len);
	buffer_dump_path(sim,out_dir,"Q_ptrap.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->Q_nfree));
	strcpy(buf.title,_("Free electron density"));
	strcpy(buf.data_label,_("Electron density"));
	strcpy(buf.data_units,"m^{-3}");
	dat_file_add_xy_data(sim,&buf,(store->Q_nfree).x, (store->Q_nfree).data, (store->Q_nfree).len);
	buffer_dump_path(sim,out_dir,"Q_nfree.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->Q_pfree));
	strcpy(buf.title,_("Free hole density"));
	strcpy(buf.data_label,_("Hole density"));
	strcpy(buf.data_units,"m^{-3}");
	dat_file_add_xy_data(sim,&buf,(store->Q_pfree).x, (store->Q_pfree).data, (store->Q_pfree).len);
	buffer_dump_path(sim,out_dir,"Q_pfree.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->Q_nfree_and_ntrap));
	strcpy(buf.title,_("Total electron density"));
	strcpy(buf.data_label,_("Electron density"));
	strcpy(buf.y_units,"\\mu s");
	strcpy(buf.data_units,"m^{-3}");
	dat_file_add_xy_data(sim,&buf,(store->Q_nfree_and_ntrap).x, (store->Q_nfree_and_ntrap).data, (store->Q_nfree_and_ntrap).len);
	buffer_dump_path(sim,out_dir,"Q_nfree_and_ntrap.dat",&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->Q_pfree_and_ptrap));
	strcpy(buf.title,_("Total hole density"));
	strcpy(buf.data_label,_("Hole density"));
	strcpy(buf.data_units,"m^{-3}");
	dat_file_add_xy_data(sim,&buf,(store->Q_pfree_and_ptrap).x, (store->Q_pfree_and_ptrap).data, (store->Q_pfree_and_ptrap).len);
	buffer_dump_path(sim,out_dir,"Q_pfree_and_ptrap.dat",&buf);
	buffer_free(&buf);

	//inter_sub_gdouble(&dynamic_np,dynamic_np.data[0]);

	//buffer_malloc(&buf);
	//buffer_add_xy_data(sim,&buf,(store->dynamic_np).x, (store->dynamic_np).data, (store->dynamic_np).len);
	//buffer_dump_path(sim,out_dir,"np.dat",&buf);
	//buffer_free(&buf);

	//inter_norm(&(store->dynamic_np),1.0);

	//buffer_malloc(&buf);
	//buffer_add_xy_data(sim,&buf,(store->dynamic_np).x, (store->dynamic_np).data, (store->dynamic_np).len);
	//buffer_dump_path(sim,out_dir,"np_norm.dat",&buf);
	//buffer_free(&buf);

	//buffer_malloc(&buf);
	//buf.data_mul=1.0;
	//dynamic_info_to_buf(sim,&buf, in,&(store->charge_change));
	//strcpy(buf.title,_("Change in charge distribution"));
	//strcpy(buf.data_label,_("percent"));
	//strcpy(buf.data_units,"\\%");
	//buffer_add_info(sim,&buf);
	//buffer_add_xy_data(sim,&buf,(store->charge_change).x, (store->charge_change).data, (store->charge_change).len);
	//buffer_dump_path(sim,out_dir,"charge_change.dat",&buf);
	//buffer_free(&buf);

	//inter_sub_long_double(&(store->dynamic_charge_tot),(store->dynamic_charge_tot).data[0]);
	//buffer_malloc(&buf);
	//buffer_add_xy_data(sim,&buf,(store->dynamic_charge_tot).x, (store->dynamic_charge_tot).data, (store->dynamic_charge_tot).len);
	//buffer_dump_path(sim,out_dir,"charge_tot.dat",&buf);
	//buffer_free(&buf);

	//////////////////////////////////////////mobility

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->mu_n));
	strcpy(buf.title,_("Electron mobility"));
	strcpy(buf.data_label,_("Mobility"));
	strcpy(buf.data_units,"m^{2}V^{-1}s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->mu_n).x, (store->mu_n).data,(store->mu_n).len);
	buffer_dump_path(sim,out_dir,"mu_n.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->mu_p));
	strcpy(buf.title,_("Hole mobility"));
	strcpy(buf.data_label,_("Mobility"));
	strcpy(buf.data_units,"m^{2}V^{-1}s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->mu_p).x, (store->mu_p).data,(store->mu_p).len);
	buffer_dump_path(sim,out_dir,"mu_p.dat",&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->mu_n_p_avg));
	strcpy(buf.title,_("Average electron and hole mobility"));
	strcpy(buf.data_label,_("Mobility"));
	strcpy(buf.data_units,"m^{2}V^{-1}s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->mu_n_p_avg).x, (store->mu_n_p_avg).data,(store->mu_n_p_avg).len);
	buffer_dump_path(sim,out_dir,"mu_n_p_avg.dat",&buf);
	buffer_free(&buf);

	//Thermal
	dump_dynamic_save_thermal(sim,in,outputpath,store);

	//////////////////////////////////////////srh rates

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->srh_n_r1));
	sprintf(buf.title,"%s - %s",_("time"),"srh_n_r1");
	strcpy(buf.type,"xy");
	strcpy(buf.data_label,"srh_n_r1");
	strcpy(buf.data_units,"m^{-3} s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->srh_n_r1).x, (store->srh_n_r1).data, (store->srh_n_r1).len);
	buffer_dump_path(sim,out_dir,"srh_n_r1.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->srh_n_r2));
	sprintf(buf.title,"%s - %s",_("time"),"srh_n_r2");
	strcpy(buf.type,"xy");
	strcpy(buf.y_label,_("Time"));
	strcpy(buf.data_label,"srh_n_r2");
	strcpy(buf.y_units,"s");
	strcpy(buf.data_units,"m^{-3}s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->srh_n_r2).x, (store->srh_n_r2).data, (store->srh_n_r2).len);
	buffer_dump_path(sim,out_dir,"srh_n_r2.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	sprintf(buf.title,"%s - %s",_("time"),"srh_n_r3");
	dynamic_info_to_buf(sim,&buf, in,&(store->srh_n_r3));
	strcpy(buf.data_label,"srh_n_r3");
	strcpy(buf.data_units,"m^{-3}s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->srh_n_r3).x, (store->srh_n_r3).data, (store->srh_n_r3).len);
	buffer_dump_path(sim,out_dir,"srh_n_r3.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->srh_n_r4));
	sprintf(buf.title,"%s %s",_("time"),"srh_n_r4");
	strcpy(buf.data_label,"srh_n_r4");
	strcpy(buf.data_units,"m^{-3}s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->srh_n_r4).x, (store->srh_n_r4).data, (store->srh_n_r4).len);
	buffer_dump_path(sim,out_dir,"srh_n_r4.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->srh_p_r1));
	sprintf(buf.title,"%s %s",_("time"),"srh_p_r1");
	strcpy(buf.data_label,"srh_p_r1");
	strcpy(buf.data_units,"m^{-3}s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->srh_p_r1).x, (store->srh_p_r1).data, (store->srh_p_r1).len);
	buffer_dump_path(sim,out_dir,"srh_p_r1.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->srh_p_r2));
	sprintf(buf.title,"%s - %s",_("time"),"srh_p_r2");
	strcpy(buf.type,"xy");
	strcpy(buf.data_label,"srh_p_r2");
	strcpy(buf.data_units,"m^{-3}s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->srh_p_r2).x, (store->srh_p_r2).data, (store->srh_p_r2).len);
	buffer_dump_path(sim,out_dir,"srh_p_r2.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->srh_p_r3));
	sprintf(buf.title,"%s - %s",_("time"),"srh_p_r3");
	strcpy(buf.data_label,"srh_p_r3");
	strcpy(buf.data_units,"m^{-3}s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->srh_p_r3).x, (store->srh_p_r3).data, (store->srh_p_r3).len);
	buffer_dump_path(sim,out_dir,"srh_p_r3.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->srh_p_r4));
	sprintf(buf.title,"%s - %s",_("time"),"srh_p_r4");
	strcpy(buf.data_label,"srh_p_r4");
	strcpy(buf.data_units,"m^{-3}s^{-1}");
	dat_file_add_xy_data(sim,&buf,(store->srh_p_r4).x, (store->srh_p_r4).data, (store->srh_p_r4).len);
	buffer_dump_path(sim,out_dir,"srh_p_r4.dat",&buf);
	buffer_free(&buf);



	//////////////////////////////////////////J
	dump_dynamic_save_j(sim,in,outputpath,store);

	//////////////////////////////////////////pl

	if (in->emission_enabled==TRUE)
	{
		inter_chop(&(store->dynamic_pl_tot),1.0e-9, 1.0);
		buffer_malloc(&buf);
		buf.data_mul=1.0;
		dynamic_info_to_buf(sim,&buf, in,&(store->dynamic_pl_tot));
		strcpy(buf.title,_("Total PL intensity"));
		strcpy(buf.data_label,_("Total PL Intensity"));
		strcpy(buf.data_units,"Photons m^{-2}");
		dat_file_add_xy_data(sim,&buf,(store->dynamic_pl_tot).x, (store->dynamic_pl_tot).data, (store->dynamic_pl_tot).len);
		buffer_dump_path(sim,out_dir,"pl_tot.dat",&buf);
		buffer_free(&buf);

		//gdouble max=math_xy_get_max(&(store->dynamic_pl));
		buffer_malloc(&buf);
		buf.data_mul=1.0;
		dynamic_info_to_buf(sim,&buf, in,&(store->dynamic_pl));
		strcpy(buf.title,_("PL intensity at given energy"));
		strcpy(buf.type,"xy");
		strcpy(buf.data_label,_("PL Intensity"));
		strcpy(buf.data_units,"au");
		dat_file_add_xy_data(sim,&buf,(store->dynamic_pl).x, (store->dynamic_pl).data, (store->dynamic_pl).len);
		buffer_dump_path(sim,out_dir,"pl.dat",&buf);
		buffer_free(&buf);
	}


	//////////////////////////////////////////field

	inter_div_long_double(&(store->E_field),(store->E_field).data[0]);
	buffer_malloc(&buf);
	dat_file_add_xy_data(sim,&buf,(store->E_field).x, (store->E_field).data, (store->E_field).len);
	buffer_dump_path(sim,out_dir,"E_field.dat",&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->dynamic_Vapplied));
	strcpy(buf.title,_("Voltage applied to diode"));
	strcpy(buf.data_label,_("Voltage"));
	strcpy(buf.data_units,"V");
	dat_file_add_xy_data(sim,&buf,(store->dynamic_Vapplied).x, (store->dynamic_Vapplied).data, (store->dynamic_Vapplied).len);
	buffer_dump_path(sim,out_dir,"Vapplied.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->band_bend));
	sprintf(buf.title,"%s - %s",_("time"),"band_bend");
	sprintf(buf.title,"%s %s",_("time"),_("band bend (percent)"));
	strcpy(buf.data_label,_("band bend"));
	strcpy(buf.data_units,_("percent"));
	dat_file_add_xy_data(sim,&buf,(store->band_bend).x, (store->band_bend).data, (store->band_bend).len);
	buffer_dump_path(sim,out_dir,"band_bend.dat",&buf);
	buffer_free(&buf);

	//////////////////////////////////////////other

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	dynamic_info_to_buf(sim,&buf, in,&(store->dynamic_qe));
	strcpy(buf.title,_("Dynamic quantum efficiency"));
	strcpy(buf.data_label,_("Percent"));
	strcpy(buf.data_units,"\%");
	dat_file_add_xy_data(sim,&buf,(store->dynamic_qe).x, (store->dynamic_qe).data, (store->dynamic_qe).len);
	buffer_dump_path(sim,out_dir,"qe.dat",&buf);
	buffer_free(&buf);


}

}

