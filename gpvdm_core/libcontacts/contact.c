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
