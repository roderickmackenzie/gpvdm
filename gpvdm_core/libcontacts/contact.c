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


void contact_init(struct simulation *sim,struct contact *c)
{
	strcpy(c->name,"none");
	c->position=-1;

	c->active=-1;
	c->ground=-1;

	strcpy(c->applied_voltage_type,"none");
	c->voltage=-1.0;
	c->voltage_want=-1.0;
	c->voltage_last=-1.0;
	c->store=-1.0;
	c->np=-1.0;
	c->charge_type=-1;
	c->area=-1.0;

	strcpy(c->shape_file_name,"none");
	shape_init(sim,&(c->shape));

	c->contact_resistance_sq=-1.0;
	c->shunt_resistance_sq=-1.0;

	c->type=-1;
	c->ve0=-1.0;
	c->vh0=-1.0;
	c->J=-1.0;
	c->i=-1.0;
}

void contact_cpy(struct simulation *sim,struct contact *out,struct contact *in)
{
	strcpy(out->name,in->name);
	out->position=in->position;

	out->active=in->active;
	out->ground=in->ground;

	strcpy(out->applied_voltage_type,in->applied_voltage_type);
	out->voltage=in->voltage;
	out->voltage_want=in->voltage_want;
	out->voltage_last=in->voltage_last;
	out->store=in->store;
	out->np=in->np;
	out->charge_type=in->charge_type;
	out->area=in->area;

	strcpy(out->shape_file_name,in->shape_file_name);
	shape_init(sim,&(out->shape));

	out->contact_resistance_sq=in->contact_resistance_sq;
	out->shunt_resistance_sq=in->shunt_resistance_sq;

	out->type=in->type;
	out->ve0=in->ve0;
	out->vh0=in->vh0;
	out->J=in->J;
	out->i=in->i;
}
