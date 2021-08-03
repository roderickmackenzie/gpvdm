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

/** @file contacts.c
@brief backend to handle complex contacts
*/

#include <string.h>
#include "epitaxy.h"
#include "inp.h"
#include "util.h"
#include "gpvdm_const.h"
#include <cal_path.h>
#include "contacts.h"
#include <log.h>
#include <dump.h>
#include <shape.h>
#include <shape_struct.h>
#include <lock.h>

void contacts_load(struct simulation *sim,struct device *in)
{
	int i;
	int active=FALSE;
	long double ingress=0.0;
	struct epitaxy *epi;
	long double layer_top;
	long double layer_btm;
	long double y_pos=0.0;
	struct shape *s;
	struct json_obj *obj_contacts;
	struct json_obj *obj_contact;
	char contact_name[200];
	char name[100];
	strcpy(name,"segment");
	in->ncontacts=0;

	obj_contacts=json_obj_find_by_path(sim,&(in->config.obj), "epitaxy.contacts");

	if (json_is_token(sim,obj_contacts,"segments")==0)
	{
		json_get_int(sim,obj_contacts, &(in->ncontacts),"segments");
	}else
	{
		json_get_int(sim,obj_contacts, &(in->ncontacts),"ncontacts");
		strcpy(name,"contact");
	}


	in->contacts=(struct contact *)malloc(in->ncontacts*sizeof(struct contact));


	epi=&(in->my_epitaxy);

	if (epi->layers>1)
	{
		layer_top=0.0;
		layer_btm=epi->layer[epi->layers-1].y_start;
	}


	for (i=0;i<in->ncontacts;i++)
	{
		sprintf(contact_name,"%s%d",name,i);
		obj_contact=json_obj_find(obj_contacts, contact_name);

		contact_init(sim,&(in->contacts[i]));

		json_get_english(sim,obj_contact, &(in->contacts[i].position),"position");

		json_get_string(sim, obj_contact, in->contacts[i].applied_voltage_type,"applied_voltage_type");

		if (strcmp(in->contacts[i].applied_voltage_type,"change")==0)
		{
			in->contacts[i].active=TRUE;
		}else
		{
			in->contacts[i].active=FALSE;
		}

		json_get_long_double(sim,obj_contact, &(in->contacts[i].voltage_want),"applied_voltage");

		in->contacts[i].voltage=0.0;
		in->contacts[i].voltage_last=in->contacts[i].voltage;

		if (strcmp(in->contacts[i].applied_voltage_type,"ground")==0)
		{
			in->contacts[i].ground=TRUE;
			in->contacts[i].voltage=0.0;
			in->contacts[i].voltage_want=0.0;
			in->contacts[i].voltage_last=0.0;
		}else
		{
			in->contacts[i].ground=FALSE;
		}


		//printf("%Le\n",in->contacts[i].voltage_want);
		//getchar();

		json_get_long_double(sim,obj_contact, &(in->contacts[i].np),"np");
		in->contacts[i].np=fabs(in->contacts[i].np);

		json_get_english(sim,obj_contact, &(in->contacts[i].charge_type),"charge_type");

		if (in->contacts[i].position==BOTTOM)
		{
			y_pos=layer_btm;
		}else
		{
			y_pos=0.0;
		}

		//printf("%d %Le\n",in->contacts[i].position,y_pos);
		//getchar();
		shape_load_from_json(sim,&(in->my_epitaxy),&(in->contacts[i].shape), obj_contact ,y_pos);
		//shape_load_file(sim,&(in->my_epitaxy),&(in->contacts[i].shape),in->contacts[i].shape_file_name,y_pos);
		s=&(in->contacts[i].shape);
		s->nx=1;
		s->nz=1;
		s->dz=in->zlen;
		s->dy_padding=0.0;
		s->dx_padding=0.0;
		s->dz_padding=0.0;
		strcpy(in->contacts[i].name,s->name);

		json_get_long_double(sim,obj_contact, &(in->contacts[i].contact_resistance_sq),"contact_resistance_sq");
		in->contacts[i].contact_resistance_sq=fabs(in->contacts[i].contact_resistance_sq);

		json_get_long_double(sim,obj_contact, &(in->contacts[i].shunt_resistance_sq),"shunt_resistance_sq");
		in->contacts[i].shunt_resistance_sq=fabs(in->contacts[i].shunt_resistance_sq);

		json_get_english(sim,obj_contact, &(in->contacts[i].type),"physical_model");

		json_get_long_double(sim,obj_contact, &(in->contacts[i].ve0),"ve0");
		in->contacts[i].ve0=fabs(in->contacts[i].ve0);

		json_get_long_double(sim,obj_contact, &(in->contacts[i].vh0),"vh0");
		in->contacts[i].vh0=fabs(in->contacts[i].vh0);

		if (in->contacts[i].position==LEFT)
		{
			s->dx=ingress;
		}
	}

	in->boundry_y0=contact_ohmic;
	for (i=0;i<in->ncontacts;i++)
	{
		//printf("%d\n",in->contacts[i].type);
		if (in->contacts[i].type==contact_schottky)
		{
			in->boundry_y0=contact_schottky;
			//printf("set\n");
			//getchar();
		}	
	}
}


void contacts_setup(struct simulation *sim,struct device *in)
{
	contacts_update(sim,in);
	contact_set_flip_current(sim,in);

	contacts_cal_area(sim,in);
}

