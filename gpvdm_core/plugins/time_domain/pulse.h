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

/** @file pulse.h
@brief header file for timedomain plugin
*/


#ifndef pulse_h
#define pulse_h
#include <sim.h>
#include <json.h>

struct pulse
{
	long double pulse_shift;
	long double pulse_bias;
	long double pulse_light_efficiency;
	int pulse_sim_mode;
	int pulse_subtract_dc;
	int dump_verbosity;
	int dump_energy_space;
	int dump_x;
	int dump_y;
	int dump_z;
};


void sim_pulse(struct simulation *sim,struct device *in);
extern struct pulse pulse_config;
void pulse_load_config(struct simulation *sim,struct pulse *in,struct device *dev,struct json_obj *json_pulse);

#endif




