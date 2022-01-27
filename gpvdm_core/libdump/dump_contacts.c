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
	int i;
	int ii;
	int active=0;
	char temp[200];
	char string[200];
	if (in->ncontacts>2)
	{
		struct dat_file buf;
		dat_file_init(&buf);

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
				dat_file_add_xy_data(sim,&buf,store->J[i].x, store->J[i].data, store->J[i].len);
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
				dat_file_add_xy_data(sim,&buf,store->time_J[i].x, store->time_J[i].data, store->time_J[i].len);
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
				dat_file_add_xy_data(sim,&buf,store->time_v[i].x, store->time_v[i].data, store->time_v[i].len);
				sprintf(temp,"time_v_contact%d.dat",i);
				buffer_dump_path(sim,in->output_path,temp,&buf);
				buffer_free(&buf);
			}
		}
	}
}

void dump_contacts_add_data(struct simulation *sim,struct device *in,struct contacts_vti_store *store)
{
	int i=0;
	gdouble x_value=0.0;
	long double J=0.0;
	long double Vdelta=0.0;
	int ground=0;
	//int active=0;
	long double Jshunt=0.0;
	long double Vdevice;

	if (in->ncontacts>2)
	{

		ground=contacts_find_ground_contact(sim,in);
		//active=contact_get_active_contact_index(sim,in);

		//long double Vground=in->contacts[ground].J*in->contacts[ground].contact_resistance_sq;
		//long double Vactive=in->contacts[ground].J*in->contacts[ground].contact_resistance_sq;
		Vdevice=contact_get_active_contact_voltage(sim,in);
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

