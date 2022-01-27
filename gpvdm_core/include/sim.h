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

/** @file sim.h
@brief functions which work on the sim.h structure.
*/


#ifndef sim_h
#define sim_h

#include <enabled_libs.h>
#include "code_ctrl.h"
#include "gpvdm_const.h"
#include <stdio.h>
#include <sim_struct.h>

#include "version.h"
#include "device.h"
#include <util.h>
#include "light_interface.h"
#include "newton_interface.h"
#include <json.h>

struct pos_config
{
	int max_ittr;
	long double posclamp;
	long double pos_min_error;
	int pos_dump_verbosity;

};

//newtonsolver
int solve_cur_thermal(struct device *in,int thermal, int z, int x);
int solve_pos(struct simulation *sim,struct device *dev);
int solve_pos_y(struct simulation *sim,struct device *in, int z, int x,struct pos_config *config);
void get_initial(struct simulation *sim,struct device *in,int guess);
void update_y_array(struct simulation *sim,struct device *in,int z,int x);
void find_n0(struct simulation *sim,struct device *in);

//from time.c
void time_mesh_save(struct simulation *sim,struct device *in);
void time_load_mesh(struct simulation *sim,struct device *in,struct json_obj *json_pulse);
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
int device_run_simulation(struct simulation *sim, struct device *dev);
void clean_sim_dir(struct simulation *sim, struct device *dev);
void solve_all(struct simulation *sim,struct device *in);

//Light
void solve_light(struct device *cell,struct light *in,gdouble Psun_in,gdouble Plaser_in);
void light_load_dlls(struct simulation *sim,struct light *in);
void light_solve_and_update(struct simulation *sim,struct device *cell,struct light *in,gdouble laser_eff_in);

//debug
void stop_start(struct simulation *sim,struct device *in);
void run_electrical_dll(struct simulation *sim,struct device *in,char *dll_name);
void sim_init(struct simulation *sim);
void sim_free(struct simulation *sim);
void fit_log_init(struct simulation *sim);

//errors
void errors_init(struct simulation *sim);
void errors_free(struct simulation *sim);
void errors_dump(struct simulation *sim);
void errors_add(struct simulation *sim, const char *format, ...);
int is_errors(struct simulation *sim);

#endif

