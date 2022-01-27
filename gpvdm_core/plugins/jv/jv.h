// 
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// The model can simulate OLEDs, Perovskite cells, and OFETs.
// 
// Copyright (C) 2012-2017 Roderick C. I. MacKenzie info at gpvdm dot com
// 
// https://www.gpvdm.com
// 
// 
// This program is free software; you can redistribute it and/or modify it
// under the terms and conditions of the GNU Lesser General Public License,
// version 2.1, as published by the Free Software Foundation.
// 
// This program is distributed in the hope it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
// 
// 
// 

/** @file jv.h
@brief JV curve header file.
*/


#ifndef jv_h
#define jv_h
#include <sim.h>
#include <json.h>

struct jv
{
	gdouble Vstart;
	gdouble Vstop;
	gdouble Vstep;
	gdouble jv_step_mul;
	gdouble jv_light_efficiency;
	gdouble jv_max_j;
	long double jv_Rshunt;
	long double jv_Rcontact;
	int jv_single_point;
	int dump_verbosity;
	int dump_energy_space;
	int dump_x;
	int dump_y;
	int dump_z;
	int jv_use_external_voltage_as_stop;
};

void sim_jv(struct simulation *sim,struct device *in);
void jv_load_config(struct simulation *sim,struct jv* in, struct json_obj *json_jv);
#endif
