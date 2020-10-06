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

/** @file dump_zxy_interfaces.c
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



void dump_zxy_interfaces(struct simulation *sim,struct device *dev,char *out_dir)
{
int x;
int y;
int z;

char name[100];
char temp[200];
long double Vexternal=get_equiv_V(sim,dev);
struct dat_file buf;
buffer_init(&buf);


	struct newton_state *ns=&(dev->ns);
	struct dimensions *dim=&dev->ns.dim;
	buffer_add_dir(sim,out_dir);

	if (dev->interfaces_n>0)
	{
		buffer_malloc(&buf);
		sprintf(name,"%s","interfaces_B.dat");
		dim_info_to_buf(&buf,dim);
		sprintf(buf.title,"%s - %s",_("Interface recombination constant"),_("Position"));
		strcpy(buf.data_label,_("Interface recombination constant"));
		strcpy(buf.data_units,"m^{3}s^{-1}");
		strcpy(buf.section_one,_("1D position space output"));
		strcpy(buf.section_two,_("Interface recombination"));
		buf.time=dev->time;
		buf.Vexternal=Vexternal;
		buffer_add_info(sim,&buf);
		buffer_add_3d_data(sim,&buf,dim,  dev->interface_B);
		buffer_dump_path(sim,out_dir,name,&buf);
		buffer_free(&buf);

		buffer_malloc(&buf);
		sprintf(name,"%s","interfaces_R.dat");
		dim_info_to_buf(&buf,dim);
		sprintf(buf.title,"%s - %s",_("Interface recombination rate"),_("Position"));
		strcpy(buf.data_label,_("Interface recombination rate"));
		strcpy(buf.data_units,"m^{-3}^{-1}");
		strcpy(buf.section_one,_("1D position space output"));
		strcpy(buf.section_two,_("Interface recombination"));
		buf.time=dev->time;
		buf.Vexternal=Vexternal;
		buffer_add_info(sim,&buf);
		buffer_add_3d_data(sim,&buf,dim,  dev->interface_R);
		buffer_dump_path(sim,out_dir,name,&buf);
		buffer_free(&buf);
	}

}
