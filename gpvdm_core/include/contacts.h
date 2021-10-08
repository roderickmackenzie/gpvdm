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

/** @file contacts.h
	@brief Header to handle complex contacts.
*/


#ifndef contacts_h
#define contacts_h

#include "contact_struct.h"
#include <sim_struct.h>
#include <device.h>


//Contact
void contact_init(struct simulation *sim,struct contact *c);
void contact_cpy(struct simulation *sim,struct contact *out,struct contact *in);

long double contact_get_active_contact_voltage_last(struct simulation *sim,struct device *in);
long double contact_get_voltage(struct simulation *sim,struct device *in,int contact);
void contact_set_voltage(struct simulation *sim,struct device *in,int contact,gdouble voltage);
long double contact_get_voltage_last(struct simulation *sim,struct device *in,int contact);
void contact_set_all_voltages(struct simulation *sim,struct device *in,gdouble voltage);
void contact_set_active_contact_voltage(struct simulation *sim,struct device *in,gdouble voltage);
long double contact_get_active_contact_voltage(struct simulation *sim,struct device *in);
int contact_get_active_contact_index(struct simulation *sim,struct device *in);
void contact_set_flip_current(struct simulation *sim,struct device *in);
void contact_set_wanted_active_contact_voltage(struct simulation *sim,struct device *in,gdouble voltage);
int contact_within(struct contact *c, long double x, long double z);
int contact_within_zx(struct contact *c, long double z, long double x);
long double contact_get_wanted_active_contact_voltage(struct simulation *sim,struct device *in);

//Contacts
void contacts_cpy(struct simulation *sim,struct device *out,struct device *in);
void contacts_passivate(struct simulation *sim,struct device *in);
void contacts_load(struct simulation *sim,struct device *in);
void contacts_setup(struct simulation *sim,struct device *in);
void contacts_update(struct simulation *sim,struct device *in);
void contacts_time_step(struct simulation *sim,struct device *in);
void contacts_force_to_zero(struct simulation *sim,struct device *in);
int contacts_itterate_to_desired_voltage(struct simulation *sim,struct device *in,char *text);
void contacts_cal_area(struct simulation *sim,struct device *in);
void contacts_detailed_dump(struct device *in);
void contacts_cal_J_and_i(struct simulation *sim,struct device *dev);
long double contacts_get_Jleft(struct device *in);
long double contacts_get_Jright(struct device *in);
int contacts_get_active_contact_left_right(struct device *in);
void contacts_dump(struct simulation *sim,struct device *in);
long double contacts_get_lcharge(struct simulation *sim,struct device *in);
long double contacts_get_rcharge(struct simulation *sim,struct device *in);
int contacts_get_lcharge_type(struct simulation *sim,struct device *in);
int contacts_get_rcharge_type(struct simulation *sim,struct device *in);
void contacts_cal_std_resistance(struct simulation *sim,struct device *dev);
void contacts_state_to_string(struct simulation *sim,char *out, struct device *dev);
int contacts_find_ground_contact(struct simulation *sim,struct device *in);
void contacts_interpolate(struct simulation *sim,struct device *in, long double **var_zx);
void contacts_ingress(struct simulation *sim,struct device *in);
void contacts_free(struct simulation *sim,struct device *in);
#endif
