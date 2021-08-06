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

/** @file ray.h
@brief ray tracing header files.
*/
#ifndef ray_h
#define ray_h
#include <enabled_libs.h>
#include <vec.h>
#include <i.h>
#include <sim_struct.h>
#include <triangle.h>
#include <dim.h>
#include <shape_struct.h>
#include <object.h>

#include <gpvdm_const.h>

#include <pthread.h>

struct ray
{
	struct vec xy;
	struct vec xy_end;
	struct vec dir;
	int state;
	int bounce;
	int obj_uid_start;		//The ray started in
	int parent;
	int uid;
	double mag;
};


struct ray_worker
{
	struct ray *rays;
	int nrays;
	int nray_max;
	int top_of_done_rays;
	int l;
		int working;
		pthread_t thread_han;
	int worker_n;
};

struct ray_src
{
	double x;
	double y;
	double z;

	int theta_steps;
	double theta_start;
	double theta_stop;

	int phi_steps;
	double phi_start;
	double phi_stop;

	int epi_layer;		//epi layer
	int light;			//light source
	int emission_source;	//single point or mesh


};

struct image
{
	int enabled;
	int worker_max;
	struct ray_worker *worker;

	struct ray_src *ray_srcs;
	int n_ray_srcs;

	double y_escape_level;
	long double *angle;
	long double **ang_escape;
	int ray_wavelength_points;
	double *lam;
	int escape_bins;

	double ray_lambda_start;
	double ray_lambda_stop;
	int ray_auto_wavelength_range;

	//viewpoint
	struct dimensions viewpoint_dim;
	int viewpoint_enabled;
	double viewpoint_size;
	double viewpoint_dz;
	long double ***viewpoint_image;

	//benchmarking
	int tot_rays;
	double start_time;

	//run control
	int ray_auto_run;
	int dump_snapshots;

};


#endif
