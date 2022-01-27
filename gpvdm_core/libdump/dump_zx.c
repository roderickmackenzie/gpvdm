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

long double **temp_2d=NULL;
char name[100];
long double Vexternal=get_equiv_V(sim,in);
struct dat_file buf;
dat_file_init(&buf);

	struct dimensions *dim=&in->ns.dim;
	malloc_zx_gdouble(dim, &temp_2d);

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


}
