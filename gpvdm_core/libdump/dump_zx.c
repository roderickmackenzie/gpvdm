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

/** @file dump_zxy_slice.c
@brief dump 1 1d slice across the device.
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
#include <lock.h>
#include <contacts.h>

void dump_zx(struct simulation *sim,struct device *in,char *out_dir)
{
int x;
int y;
int z;
long double mul_x;
long double mul_y;
long double mul_z;

long double **temp_2d=NULL;
int band;
char name[100];
char temp[200];
long double Vexternal=get_equiv_V(sim,in);
struct dat_file buf;
buffer_init(&buf);


	struct newton_state *ns=&(in->ns);
	struct dimensions *dim=&in->ns.dim;
	malloc_zx_gdouble(dim, &temp_2d);
	//buffer_add_dir(sim,out_dir);


	buffer_malloc(&buf);
	sprintf(name,"%s","Jnp1_zx.dat");
	dim_info_to_buf(&buf,dim);
	sprintf(buf.title,"%s - %s",_("Current map of the bottom contact"),_("position"));
	strcpy(buf.data_label,_("Hole current density (diffusion)"));
	strcpy(buf.data_units,"A m^{-2}");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(sim,&buf);

	buffer_add_zx_data(sim,&buf,dim, in->Jp_y1);

	buffer_dump_path(sim,"",name,&buf);

	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","R1_zx.dat");
	dim_info_to_buf(&buf,dim);
	sprintf(buf.title,"%s - %s",_("Resistance of bottom of device"),_("position"));
	strcpy(buf.data_label,_("Hole current density (diffusion)"));
	strcpy(buf.data_units,"Ohms");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(sim,&buf);

	long double voltage=contact_get_active_contact_voltage(sim,in);
	mem_set_zx_gdouble_from_zx_gdouble(dim, temp_2d, in->Jp_y1);
	mem_mul_zx_area(dim, temp_2d);
	mem_zx_invert_long_double(dim, temp_2d);
	mem_mul_zx_long_double(dim, temp_2d,voltage);

	buffer_add_zx_data(sim,&buf,dim, temp_2d);

	buffer_dump_path(sim,"",name,&buf);

	buffer_free(&buf);

	free_zx_gdouble(dim, &temp_2d);

	/*zxy_set_gdouble(dim, temp_3d, 0.0);
	three_d_add_gdouble(dim, temp_3d, in->Jp_diffusion);
	three_d_add_gdouble(dim, temp_3d, in->Jp_drift);
	buffer_add_3d_data(sim,&buf,dim, temp_3d);
	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);


	malloc_zx_gdouble(dim, &temp_top);
	malloc_zx_gdouble(dim, &temp_btm);

	mem_set_zx_gdouble_from_zx_gdouble(dim, temp_top, in->V_y0);
	mem_add_zx_gdouble_from_zx_gdouble(dim, temp_top, in->Vapplied_y0);

	mem_set_zx_gdouble_from_zx_gdouble(dim, temp_btm, in->V_y1);
	mem_add_zx_gdouble_from_zx_gdouble(dim, temp_btm, in->Vapplied_y1);

	buffer_add_3d_device_data_including_boundaries(sim,&buf,in,  ns->phi,temp_top,temp_btm);

	free_zx_gdouble(dim, &temp_btm);
	free_zx_gdouble(dim, &temp_top);

	buffer_dump_path(sim,out_dir,name,&buf);*/

}
