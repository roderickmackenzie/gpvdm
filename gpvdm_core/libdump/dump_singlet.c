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



void dump_zxy_singlet(struct simulation *sim,struct device *dev,char *out_dir)
{

	long double ***temp_3d=NULL;
	//char name[100];
	long double Vexternal=get_equiv_V(sim,dev);
	struct dat_file buf;
	dat_file_init(&buf);
	struct newton_state *ns=&(dev->ns);
	struct dimensions *dim=&dev->ns.dim;
	malloc_zxy_long_double(dim, &temp_3d);
	gpvdm_mkdir(out_dir);

	if (ns->Ns!=NULL)
	{
		if (buffer_set_file_name(sim,dev,&buf,"Ns.csv")==0)
		{
			buffer_malloc(&buf);
			dim_info_to_buf(&buf,dim);
			sprintf(buf.title,"%s - %s",_("Singlet density"),_("Position"));
			strcpy(buf.data_label,_("Singlet density"));
			strcpy(buf.data_units,"m^{-3}");
			buf.time=dev->time;
			buf.Vexternal=Vexternal;
			dat_file_add_zxy_data(sim,&buf,dim,  ns->Ns);
			buffer_dump_path(sim,out_dir,NULL,&buf);
			buffer_free(&buf);
		}

	}

	if (ns->Nt!=NULL)
	{
		if (buffer_set_file_name(sim,dev,&buf,"Nt.csv")==0)
		{
			buffer_malloc(&buf);
			dim_info_to_buf(&buf,dim);
			sprintf(buf.title,"%s - %s",_("Triplet density"),_("Position"));
			strcpy(buf.data_label,_("Triplet density"));
			strcpy(buf.data_units,"m^{-3}");
			buf.time=dev->time;
			buf.Vexternal=Vexternal;
			dat_file_add_zxy_data(sim,&buf,dim,  ns->Nt);
			buffer_dump_path(sim,out_dir,NULL,&buf);
			buffer_free(&buf);
		}

	}

	if (ns->Nsd!=NULL)
	{
		if (buffer_set_file_name(sim,dev,&buf,"Nsd.csv")==0)
		{
			buffer_malloc(&buf);
			dim_info_to_buf(&buf,dim);
			sprintf(buf.title,"%s - %s",_("Singlet in donor density"),_("Position"));
			strcpy(buf.data_label,_("Singlet density"));
			strcpy(buf.data_units,"m^{-3}");
			buf.time=dev->time;
			buf.Vexternal=Vexternal;
			dat_file_add_zxy_data(sim,&buf,dim,  ns->Nsd);
			buffer_dump_path(sim,out_dir,NULL,&buf);
			buffer_free(&buf);
		}

	}

	if (ns->Ntd!=NULL)
	{
		if (buffer_set_file_name(sim,dev,&buf,"Ntd.csv")==0)
		{
			buffer_malloc(&buf);
			dim_info_to_buf(&buf,dim);
			sprintf(buf.title,"%s - %s",_("Triplet in donor density"),_("Position"));
			strcpy(buf.data_label,_("Ion density"));
			strcpy(buf.data_units,"m^{-3}");
			buf.time=dev->time;
			buf.Vexternal=Vexternal;
			dat_file_add_zxy_data(sim,&buf,dim,  ns->Ntd);
			buffer_dump_path(sim,out_dir,NULL,&buf);
			buffer_free(&buf);
		}

	}

	if (ns->Nho!=NULL)
	{
		if (buffer_set_file_name(sim,dev,&buf,"Nho.csv")==0)
		{
			buffer_malloc(&buf);
			dim_info_to_buf(&buf,dim);
			sprintf(buf.title,"%s - %s",_("Photon density"),_("Position"));
			strcpy(buf.data_label,_("Photon density"));
			strcpy(buf.data_units,"m^{-3}");
			buf.time=dev->time;
			buf.Vexternal=Vexternal;
			dat_file_add_zxy_data(sim,&buf,dim,  ns->Nho);
			buffer_dump_path(sim,out_dir,NULL,&buf);
			buffer_free(&buf);
		}

	}

}
