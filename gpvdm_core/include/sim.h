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

/** @file sim.h
@brief functions which work on the sim.h structure.
*/


#ifndef sim_h
#define sim_h

#include "code_ctrl.h"
#include "gpvdm_const.h"
#include <stdio.h>
#include <sim_struct.h>

#include "version.h"
#include "device.h"
#include <util.h>
#include "light_interface.h"
#include "newton_interface.h"

//newtonsolver
int solve_cur_thermal(struct device *in,int thermal, int z, int x);
int solve_pos(struct simulation *sim,struct device *in, int z, int x);
void get_initial(struct simulation *sim,struct device *in,int guess);
void update_y_array(struct simulation *sim,struct device *in,int z,int x);
void find_n0(struct simulation *sim,struct device *in);

//from time.c
void time_mesh_save(struct simulation *sim,struct device *in);
void time_load_mesh(struct simulation *sim,struct device *in,int number);
void time_init(struct simulation *sim,struct device *in);
void time_store(struct simulation *sim,struct device *in);
void device_timestep(struct simulation *sim,struct device *in);
int time_test_last_point(struct device *in);
gdouble time_get_voltage(struct device *in);
gdouble time_get_sun(struct device *in);
gdouble time_get_laser(struct device *in);
gdouble time_get_fs_laser(struct device *in);
void time_memory_free(struct time_mesh *tm);
void time_mesh_init(struct time_mesh *tm);
void time_mesh_cpy(struct time_mesh *out,struct time_mesh *in);

//
int get_clamp_state();

void get_max_layers(int in);
void lock_main(int argc, char *argv[]);

void update_material_arrays(struct simulation *sim, struct device *in);



void load_config(struct simulation *sim,struct device *in);
void update(struct device *cell);
int run_simulation(struct simulation *sim);
void solve_all(struct simulation *sim,struct device *in);

//Light
void light_transfer_gen_rate_to_device(struct device *cell,struct light *in);
void solve_light(struct device *cell,struct light *in,gdouble Psun_in,gdouble Plaser_in);
void light_load_dlls(struct simulation *sim,struct light *in);
void light_transfer_gen_rate_to_device(struct device *cell,struct light *in);
void light_solve_and_update(struct simulation *sim,struct device *cell,struct light *in,gdouble laser_eff_in);

//debug
void stop_start(struct simulation *sim,struct device *in);
void run_electrical_dll(struct simulation *sim,struct device *in,char *dll_name);
void gen_dos_fd_gaus_fd(struct simulation *sim,struct epitaxy *in_epitaxy);
void gen_dos_fd_gaus_fd_stand_alone(struct simulation *sim);
void sim_init(struct simulation *sim);
void fit_log_init(struct simulation *sim);

//errors
void errors_init(struct simulation *sim);
void errors_free(struct simulation *sim);
void errors_dump(struct simulation *sim);
void errors_add(struct simulation *sim, const char *format, ...);
int is_errors(struct simulation *sim);

#endif

