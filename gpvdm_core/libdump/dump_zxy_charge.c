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

/** @file dump_zxy_charge.c
@brief dump zxy slice across the device.
*/

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sim.h>
#include <dump.h>
#include <dat_file.h>
#include <util.h>
#include <lang.h>
#include <i.h>
#include <exp.h>
#include <dos.h>
#include <memory.h>



void dump_zxy_charge(struct simulation *sim,struct device *in,char *out_dir)
{
int x;
int y;
int z;
long double mul_x;
long double mul_y;
long double mul_z;

long double ***temp_3d=NULL;
long double **temp_top=NULL;
long double **temp_btm=NULL;
int band;
char name[100];
char temp[200];
long double Vexternal=get_equiv_V(sim,in);
struct dat_file buf;
buffer_init(&buf);


	struct newton_state *ns=&(in->ns);
	struct dimensions *dim=&in->ns.dim;
	malloc_zxy_gdouble(dim, &temp_3d);
	buffer_add_dir(sim,out_dir);


	buffer_malloc(&buf);
	sprintf(name,"%s","Nad.dat");
	dim_info_to_buf(&buf,dim);
	sprintf(buf.title,"%s - %s",_("Doping"),_("Position"));
	strcpy(buf.data_label,_("Doping density"));
	strcpy(buf.data_units,"m^{-3}");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(sim,&buf);
	buffer_add_3d_data(sim,&buf,dim,  in->Nad);
	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","Q_nfree.dat");
	dim_info_to_buf(&buf,dim);
	sprintf(buf.title,"%s - %s",_("Free electron carrier density"),_("Position"));
	strcpy(buf.data_label,_("Carrier density"));
	strcpy(buf.data_units,"m^{-3}");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Material parameters"));
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buf.logscale_data=TRUE;
	buf.x=dim->xlen;
	buf.y=dim->ylen+2;
	buf.z=dim->zlen;
	buffer_add_info(sim,&buf);
	//buffer_add_3d_data(sim,&buf,dim,  in->n);
	buffer_add_3d_device_data_including_boundaries(sim,&buf,in,  in->n,in->electrons_y0,in->electrons_y1);
	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Q_pfree.dat");
	dim_info_to_buf(&buf,dim);
	sprintf(buf.title,"%s - %s",_("Free hole carrier density"),_("Position"));
	strcpy(buf.data_label,_("Carrier density"));
	strcpy(buf.data_units,"m^{-3}");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Material parameters"));
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buf.logscale_data=TRUE;
	buf.x=dim->xlen;
	buf.y=dim->ylen+2;
	buf.z=dim->zlen;
	buffer_add_info(sim,&buf);
	//buffer_add_3d_data(sim,&buf,dim,  in->p);
	buffer_add_3d_device_data_including_boundaries(sim,&buf,in,  in->p,in->holes_y0,in->holes_y1);
	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Q_ntrap.dat");
	dim_info_to_buf(&buf,dim);
	sprintf(buf.title,"%s - %s",_("Trapped electron carrier density"),_("Position"));
	strcpy(buf.data_label,_("Carrier density"));
	strcpy(buf.data_units,"m^{-3}");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(sim,&buf);
	buffer_add_3d_data(sim,&buf,dim,  in->nt_all);
	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Q_ptrap.dat");
	dim_info_to_buf(&buf,dim);
	sprintf(buf.title,"%s - %s",_("Trapped hole carrier density"),_("Position"));
	strcpy(buf.data_label,_("Carrier density"));
	strcpy(buf.data_units,"m^{-3}");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(sim,&buf);
	buffer_add_3d_data(sim,&buf,dim,  in->pt_all);
	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Q_pfree_and_ptrap.dat");
	dim_info_to_buf(&buf,dim);
	sprintf(buf.title,"%s - %s",_("Total hole density"),_("Position"));
	strcpy(buf.data_label,_("Carrier density"));
	strcpy(buf.data_units,"m^{-3}");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.logscale_data=TRUE;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(sim,&buf);

	zxy_set_gdouble(dim, temp_3d, 0.0);
	three_d_add_gdouble(dim, temp_3d, in->p);
	three_d_add_gdouble(dim, temp_3d, in->pt_all);
	buffer_add_3d_data(sim,&buf,dim, temp_3d);

	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","Q_nfree_and_ntrap.dat");
	dim_info_to_buf(&buf,dim);
	sprintf(buf.title,"%s - %s",_("Total electron density"),_("Position"));
	strcpy(buf.data_label,_("Carrier density"));
	strcpy(buf.data_units,"m^{-3}");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.logscale_data=TRUE;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(sim,&buf);

	zxy_set_gdouble(dim, temp_3d, 0.0);
	three_d_add_gdouble(dim, temp_3d, in->n);
	three_d_add_gdouble(dim, temp_3d, in->nt_all);
	buffer_add_3d_data(sim,&buf,dim, temp_3d);

	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","dn.dat");
	dim_info_to_buf(&buf,dim);
	sprintf(buf.title,"%s %s",_("Change in electron population"),_("Position"));
	strcpy(buf.data_label,_("Carrier density"));
	strcpy(buf.data_units,"m^{-3}");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Chaerge density"));
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(sim,&buf);

	zxy_set_gdouble(dim, temp_3d, 0.0);
	three_d_add_gdouble(dim, temp_3d, in->n);
	three_d_add_gdouble(dim, temp_3d, in->nt_all);
	three_d_sub_gdouble(dim, temp_3d, in->nf_save);
	three_d_sub_gdouble(dim, temp_3d, in->nt_save);
	buffer_add_3d_data(sim,&buf,dim, temp_3d);

	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Q_total.dat");
	dim_info_to_buf(&buf,dim);
	sprintf(buf.title,"%s - %s",_("Total charge"),_("Position"));
	strcpy(buf.data_label,_("Carrier density"));
	strcpy(buf.data_units,"m^{-3}");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(sim,&buf);

	zxy_set_gdouble(dim, temp_3d, 0.0);
	three_d_add_gdouble(dim, temp_3d, in->p);
	three_d_sub_gdouble(dim, temp_3d, in->n);
	three_d_add_gdouble(dim, temp_3d, in->pt_all);
	three_d_sub_gdouble(dim, temp_3d, in->nt_all);
	buffer_add_3d_data(sim,&buf,dim, temp_3d);

	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","dcharge.dat");
	dim_info_to_buf(&buf,dim);
	sprintf(buf.title,"%s - %s",_("Change in total charge"),_("Position"));
	strcpy(buf.data_label,_("Carrier density"));
	strcpy(buf.data_units,"m^{-3}");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(sim,&buf);

	zxy_set_gdouble(dim, temp_3d, 0.0);
	three_d_add_gdouble(dim, temp_3d, in->p);
	three_d_sub_gdouble(dim, temp_3d, in->n);
	three_d_add_gdouble(dim, temp_3d, in->pt_all);
	three_d_sub_gdouble(dim, temp_3d, in->nt_all);

	three_d_sub_gdouble(dim, temp_3d, in->pt_save);
	three_d_add_gdouble(dim, temp_3d, in->nf_save);
	three_d_sub_gdouble(dim, temp_3d, in->pf_save);
	three_d_add_gdouble(dim, temp_3d, in->nt_save);

	buffer_add_3d_data(sim,&buf,dim, temp_3d);

	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","dp.dat");
	dim_info_to_buf(&buf,dim);
	sprintf(buf.title,"%s - %s",_("Change in hole population"),_("Position"));
	strcpy(buf.data_label,_("Carrier density"));
	strcpy(buf.data_units,"m^{-3}");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(sim,&buf);

	zxy_set_gdouble(dim, temp_3d, 0.0);
	three_d_add_gdouble(dim, temp_3d, in->p);
	three_d_add_gdouble(dim, temp_3d, in->pt_all);
	three_d_sub_gdouble(dim, temp_3d, in->pf_save);
	three_d_sub_gdouble(dim, temp_3d, in->pt_save);

	buffer_add_3d_data(sim,&buf,dim, temp_3d);

	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","dnt.dat");
	dim_info_to_buf(&buf,dim);
	sprintf(buf.title,"%s - %s",_("Excess electron density"),_("Position"));
	strcpy(buf.x_label,_("x-position"));
	strcpy(buf.y_label,_("y-position"));
	strcpy(buf.z_label,_("z-position"));
	strcpy(buf.data_label,_("Electron density"));
	strcpy(buf.data_units,"m^{-3}");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(sim,&buf);

	zxy_set_gdouble(dim, temp_3d, 0.0);
	three_d_add_gdouble(dim, temp_3d, in->nt_all);
	three_d_sub_gdouble(dim, temp_3d, in->nt_save);
	buffer_add_3d_data(sim,&buf,dim, temp_3d);

	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","dpt.dat");
	dim_info_to_buf(&buf,dim);
	sprintf(buf.title,"%s - %s",_("Excess hole density"),_("Position"));
	strcpy(buf.data_label,_("Hole density"));
	strcpy(buf.data_units,"m^{-3}");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(sim,&buf);

	zxy_set_gdouble(dim, temp_3d, 0.0);
	three_d_add_gdouble(dim, temp_3d, in->pt_all);
	three_d_sub_gdouble(dim, temp_3d, in->pt_save);
	buffer_add_3d_data(sim,&buf,dim, temp_3d);

	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);

	if (in->Nion!=NULL)
	{
		buffer_malloc(&buf);
		sprintf(name,"%s","Nion.dat");
		dim_info_to_buf(&buf,dim);
		sprintf(buf.title,"%s - %s",_("Perovskite mobile ion density"),_("Position"));
		strcpy(buf.data_label,_("Ion density"));
		strcpy(buf.data_units,"m^{-3}");
		strcpy(buf.section_one,_("1D position space output"));
		strcpy(buf.section_two,_("Charge density"));
		buf.time=in->time;
		buf.Vexternal=Vexternal;
		buffer_add_info(sim,&buf);
		buffer_add_3d_data(sim,&buf,dim,  in->Nion);
		buffer_dump_path(sim,out_dir,name,&buf);
		buffer_free(&buf);
	}

	free_zxy_gdouble(dim, &temp_3d);
}
