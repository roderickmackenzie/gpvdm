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

/** @file dump_contacts.c
@brief dump JV curves from the contacts.
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

#include "memory.h"
#include "contacts.h"
#include <lang.h>
#include <cal_path.h>
#include "contacts_vti_store.h"

static int unused __attribute__((unused));

void dump_contacts_init(struct simulation *sim,struct device *in,struct contacts_vti_store *store)
{
	if (in->ncontacts>2)
	{
		int i=0;

		for (i=0;i<in->ncontacts;i++)
		{
			inter_init(sim,&(store->time_v[i]));
			inter_init(sim,&(store->time_J[i]));
			inter_init(sim,&(store->J[i]));
			inter_init(sim,&(store->J_external[i]));
		}
	}
}

void dump_contacts_save(struct simulation *sim,struct device *in,struct contacts_vti_store *store)
{
	char string[200];
	if (in->ncontacts>2)
	{
		int i;
		int ii;
		int active=0;
		int sub=TRUE;
		char temp[200];
		struct dat_file buf;
		buffer_init(&buf);

		active=contact_get_active_contact_index(sim,in);
		for (i=0;i<in->ncontacts;i++)
		{
			if (in->go_time==FALSE)
			{
				buffer_malloc(&buf);
				buf.y_mul=1.0;
				buf.data_mul=1.0;
				sprintf(buf.title,"%s (%s)",_("Voltage - Current density"), in->contacts[i].name);
				strcpy(buf.type,"xy");
				sprintf(buf.y_label,"%s %s",in->contacts[active].name,_("Voltage"));
				strcpy(buf.y_units,"V");
				sprintf(buf.data_label,"%s %s",in->contacts[i].name,_("Current"));
				strcpy(buf.data_units,"A m^{-2}");
				buf.logscale_y=0;
				buf.logscale_data=0;
				buf.x=1;
				buf.y=store->J[i].len;
				buf.z=1;
				buffer_add_info(sim,&buf);
				buffer_add_xy_data(sim,&buf,store->J[i].x, store->J[i].data, store->J[i].len);
				sprintf(temp,"jv_contact%d.dat",i);
				buffer_dump_path(sim,in->output_path,temp,&buf);
				buffer_free(&buf);

				buffer_malloc(&buf);
				buf.y_mul=1.0;
				buf.data_mul=1.0;
				sprintf(buf.title,"%s (%s)",_("Voltage - Current"), in->contacts[i].name);
				strcpy(buf.type,"xy");
				sprintf(buf.y_label,"%s %s",in->contacts[active].name,_("Voltage"));
				strcpy(buf.y_units,"V");
				sprintf(buf.data_label,"%s %s",in->contacts[i].name,_("Current"));
				strcpy(buf.data_units,"A");
				buf.logscale_y=0;
				buf.logscale_data=0;
				buf.x=1;
				buf.y=store->J[i].len;
				buf.z=1;
				buffer_add_info(sim,&buf);

				for (ii=0;ii<store->J[i].len;ii++)
				{
					sprintf(string,"%Le %Le\n",store->J[i].x[ii],store->J[i].data[ii]*in->contacts[i].area);
					buffer_add_string(&buf,string);
				}

				sprintf(temp,"iv_contact%d.dat",i);
				buffer_dump_path(sim,in->output_path,temp,&buf);
				dat_file_dump_gnuplot_file(sim,in->output_path,temp,&buf);
				buffer_free(&buf);


				/*buffer_malloc(&buf);
				buf.y_mul=1.0;
				buf.data_mul=1.0;
				sprintf(buf.title,"%s (%s)",_("Voltage - Current"), in->contacts[i].name);
				strcpy(buf.type,"xy");
				strcpy(buf.y_label,_("Voltage"));
				strcpy(buf.y_units,"V");
				strcpy(buf.data_label,_("Current"));
				strcpy(buf.data_units,"A");
				buf.logscale_y=0;
				buf.logscale_data=0;
				buf.x=1;
				buf.y=store->J_external[i].len;
				buf.z=1;
				buffer_add_info(sim,&buf);

				for (ii=0;ii<store->J_external[i].len;ii++)
				{
					sprintf(string,"%Le %Le\n",store->J_external[i].x[ii],store->J_external[i].data[ii]*in->contacts[i].area);
					buffer_add_string(&buf,string);
				}

				sprintf(temp,"iv_contact_external%d.dat",i);
				buffer_dump_path(sim,in->output_path,temp,&buf);
				buffer_free(&buf);*/
			}else
			{
				buffer_malloc(&buf);
				buf.y_mul=1.0;
				buf.data_mul=1.0;
				sprintf(buf.title,"%s (%s)",_("Time - Current density"), in->contacts[i].name);
				strcpy(buf.type,"xy");
				sprintf(buf.y_label,"%s %s",in->contacts[active].name,_("Time"));
				strcpy(buf.y_units,"s");
				sprintf(buf.data_label,"%s %s",in->contacts[i].name,_("Current"));
				strcpy(buf.data_units,"A m^{-2}");
				buf.logscale_y=0;
				buf.logscale_data=0;
				buf.x=1;
				buf.y=store->time_J[i].len;
				buf.z=1;
				buffer_add_info(sim,&buf);
				buffer_add_xy_data(sim,&buf,store->time_J[i].x, store->time_J[i].data, store->time_J[i].len);
				sprintf(temp,"time_j_contact%d.dat",i);
				buffer_dump_path(sim,in->output_path,temp,&buf);
				buffer_free(&buf);

				buffer_malloc(&buf);
				buf.y_mul=1.0;
				buf.data_mul=1.0;
				sprintf(buf.title,"%s (%s)",_("Time - Voltage"), in->contacts[i].name);
				strcpy(buf.type,"xy");
				sprintf(buf.y_label,"%s %s",in->contacts[active].name,_("Time"));
				strcpy(buf.y_units,"s");
				sprintf(buf.data_label,"%s %s",in->contacts[i].name,_("Voltage"));
				strcpy(buf.data_units,"V");
				buf.logscale_y=0;
				buf.logscale_data=0;
				buf.x=1;
				buf.y=store->time_v[i].len;
				buf.z=1;
				buffer_add_info(sim,&buf);
				buffer_add_xy_data(sim,&buf,store->time_v[i].x, store->time_v[i].data, store->time_v[i].len);
				sprintf(temp,"time_v_contact%d.dat",i);
				buffer_dump_path(sim,in->output_path,temp,&buf);
				buffer_free(&buf);
			}
		}
	}
}

void dump_contacts_add_data(struct simulation *sim,struct device *in,struct contacts_vti_store *store)
{
	if (in->ncontacts>2)
	{
		int i=0;
		gdouble x_value=0.0;
		long double J=0.0;
		long double Vdelta=0.0;

		int ground=0;
		int active=0;
		ground=contacts_find_ground_contact(sim,in);
		active=contact_get_active_contact_index(sim,in);
		long double Jshunt=0.0;
		long double Vground=in->contacts[ground].J*in->contacts[ground].contact_resistance_sq;
		long double Vactive=in->contacts[ground].J*in->contacts[ground].contact_resistance_sq;
		long double Vdevice=contact_get_active_contact_voltage(sim,in);
		//printf("%Le %Le %Le\n",Vground,Vactive,Vdevice);
		x_value=Vdevice;//Vground+Vactive+
		//getchar();

		for (i=0;i<in->ncontacts;i++)
		{
			J=in->contacts[i].J;
			Vdelta=(in->contacts[i].voltage-in->contacts[ground].voltage);

			Jshunt=Vdelta/in->contacts[i].shunt_resistance_sq;

			//printf("%Le %Le\n",J,Jshunt);
			inter_append(&(store->time_v[i]),in->time,in->contacts[i].voltage);
			inter_append(&(store->time_J[i]),in->time,J);
			inter_append(&(store->J[i]),x_value,J);
			inter_append(&(store->J_external[i]),x_value,J+Jshunt);
			
		}

	}

}

void dump_contacts_free(struct simulation *sim,struct device *in,struct contacts_vti_store *store)
{
	if (in->ncontacts>2)
	{
		int i=0;

		for (i=0;i<in->ncontacts;i++)
		{
			//inter_free(&(store->v[i]));
			inter_free(&(store->time_v[i]));
			inter_free(&(store->time_J[i]));
			inter_free(&(store->J[i]));
			inter_free(&(store->J_external[i]));
		}
	}
}

