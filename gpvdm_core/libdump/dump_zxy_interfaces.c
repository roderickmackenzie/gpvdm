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
long double Vexternal=get_equiv_V(sim,dev);
struct dat_file buf;
dat_file_init(&buf);

	struct dimensions *dim=&dev->ns.dim;
	gpvdm_mkdir(out_dir);

	if (dev->interfaces_n>0)
	{
		if (buffer_set_file_name(sim,dev,&buf,"interfaces_B.csv")==0)
		{
			buffer_malloc(&buf);
			dim_info_to_buf(&buf,dim);
			sprintf(buf.title,"%s - %s",_("Interface free-to-free recombination constant"),_("Position"));
			strcpy(buf.data_label,_("Interface free-to-free recombination constant"));
			strcpy(buf.data_units,"m^{3}s^{-1}");
			strcpy(buf.section_one,_("1D position space output"));
			strcpy(buf.section_two,_("Interface recombination"));
			buf.time=dev->time;
			buf.Vexternal=Vexternal;
			dat_file_add_zxy_data(sim,&buf,dim,  dev->interface_B);
			buffer_dump_path(sim,out_dir,NULL,&buf);
			buffer_free(&buf);
		}

	}

	if (dev->interfaces_tunnels_e>0)
	{
		if (buffer_set_file_name(sim,dev,&buf,"interfaces_Ge.csv")==0)
		{
			buffer_malloc(&buf);
			dim_info_to_buf(&buf,dim);
			sprintf(buf.title,"%s - %s",_("Electron tunneling though the interface"),_("Position"));
			strcpy(buf.data_label,_("Tunneling-e"));
			strcpy(buf.data_units,"m^{3}s^{-1}");
			strcpy(buf.section_one,_("1D position space output"));
			strcpy(buf.section_two,_("Interface recombination"));
			buf.time=dev->time;
			buf.Vexternal=Vexternal;
			dat_file_add_zxy_data(sim,&buf,dim,  dev->interface_Ge);
			buffer_dump_path(sim,out_dir,NULL,&buf);
			buffer_free(&buf);
		}

	}

	if (dev->interfaces_tunnels_h>0)
	{
		if (buffer_set_file_name(sim,dev,&buf,"interfaces_Gh.csv")==0)
		{
			buffer_malloc(&buf);
			dim_info_to_buf(&buf,dim);
			sprintf(buf.title,"%s - %s",_("Hole tunneling though the interface"),_("Position"));
			strcpy(buf.data_label,_("Tunneling-h"));
			strcpy(buf.data_units,"m^{3}s^{-1}");
			strcpy(buf.section_one,_("1D position space output"));
			strcpy(buf.section_two,_("Interface recombination"));
			buf.time=dev->time;
			buf.Vexternal=Vexternal;
			dat_file_add_zxy_data(sim,&buf,dim,  dev->interface_Gh);
			buffer_dump_path(sim,out_dir,NULL,&buf);
			buffer_free(&buf);
		}

	}

	if (dev->interfaces_n_srh>0)
	{
		if (buffer_set_file_name(sim,dev,&buf,"interfaces_srh.csv")==0)
		{
			buffer_malloc(&buf);
			dim_info_to_buf(&buf,dim);
			sprintf(buf.title,"%s - %s",_("Interface free-to-trap recombination constant"),_("Position"));
			strcpy(buf.data_label,_("Interface free-to-trap  recombination constant"));
			strcpy(buf.data_units,"m^{3}s^{-1}");
			strcpy(buf.section_one,_("1D position space output"));
			strcpy(buf.section_two,_("Interface recombination"));
			buf.time=dev->time;
			buf.Vexternal=Vexternal;
			dat_file_add_zxy_data(sim,&buf,dim,  dev->interface_Bt);
			buffer_dump_path(sim,out_dir,NULL,&buf);
			buffer_free(&buf);
		}

		if (buffer_set_file_name(sim,dev,&buf,"interfaces_R.csv")==0)
		{
			buffer_malloc(&buf);
			dim_info_to_buf(&buf,dim);
			sprintf(buf.title,"%s - %s",_("Interface recombination rate"),_("Position"));
			strcpy(buf.data_label,_("Interface recombination rate"));
			strcpy(buf.data_units,"m^{-3}^{-1}");
			strcpy(buf.section_one,_("1D position space output"));
			strcpy(buf.section_two,_("Interface recombination"));
			buf.time=dev->time;
			buf.Vexternal=Vexternal;
			dat_file_add_zxy_data(sim,&buf,dim,  dev->interface_R);
			buffer_dump_path(sim,out_dir,NULL,&buf);
			buffer_free(&buf);
		}
	}

}
