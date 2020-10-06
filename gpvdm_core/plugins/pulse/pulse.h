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

struct pulse
{
gdouble pulse_shift;
gdouble pulse_bias;
gdouble pulse_light_efficiency;
int pulse_sim_mode;
int pulse_subtract_dc;

};


void sim_pulse(struct simulation *sim,struct device *in);
extern struct pulse pulse_config;
void pulse_load_config(struct simulation *sim,struct pulse *in,struct device *dev,char *config_file_name);
void newton_aux_pulse(struct device *in,gdouble V,gdouble* i,gdouble* didv,gdouble* didphi,gdouble* didxil,gdouble* didxipl,gdouble* didphir,gdouble* didxir,gdouble* didxipr);
gdouble newton_pulse(struct device *in,gdouble Vtot,int usecap);

void newton_aux_pulse_voc(struct device *in,gdouble V,gdouble* i,gdouble* didv,gdouble* didphi,gdouble* didxil,gdouble* didxipl,gdouble* didphir,gdouble* didxir,gdouble* didxipr);
gdouble pulse_newton_sim_voc_fast(struct simulation *sim,struct device *in,int do_LC);
gdouble pulse_newton_sim_voc(struct simulation *sim, struct device *in);
void pulse_set_light_for_voc(struct simulation *sim,struct device *in,gdouble Voc);

#endif




