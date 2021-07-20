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

/** @file server.c
	@brief Job management for fitting, run multiple fitting instances over multiple CPUs.
*/

#include <enabled_libs.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dump.h>
#include <unistd.h>
#include <dos.h>
#include "util.h"

	#include <sys/inotify.h>
	#include <sys/time.h>
	#include <signal.h>
	#include <linux/limits.h>
	#define EVENT_SIZE  ( sizeof (struct inotify_event) )
	#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
#include "sim.h"
#include "server.h"
#include "timer.h"
#include "gui_hooks.h"
#include "lang.h"
#include "log.h"
#include <gui_hooks.h>
#include <cal_path.h>
#include <gpvdm_const.h>
#include <server.h>
#include <memory.h>
#include <json.h>

static int unused __attribute__((unused));


void server2_config_load(struct simulation *sim,struct server_struct *server,struct json_obj *json_server)
{
	json_get_int(sim, json_server, &server->max_threads,"gpvdm_core_max_threads");
	json_get_int(sim, json_server, &server->max_run_time,"server_max_run_time");

}

