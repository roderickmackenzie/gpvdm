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

/** @file server.h
@brief header file for the internal server used to run jobs across multiple CPUs
*/

#ifndef serverh
#define serverh

#include <sim_struct.h>
#include <server_struct.h>
struct my_msgbuf{
    long mtype;
    char mtext[200];
};


void server_stop_and_exit();
void server_shut_down(struct simulation *sim,struct server_struct *myserver);
void server_add_job(struct simulation *sim,char *command,char *output);
void print_jobs(struct simulation *sim);
void server_init(struct simulation *sim);
void server_exe_jobs(struct simulation *sim, struct server_struct *myserver);
void server_job_finished(struct server_struct *myserver,char *job);
int server_run_jobs(struct simulation *sim,struct server_struct *myserver);
double server_get_odes_per_s();
double server_get_jobs_per_s();
void change_cpus(struct simulation *sim,struct server_struct *myserver);
void server_set_lock_file(struct server_struct *myserver, char *file_name);
void server_check_wall_clock(struct simulation *sim,struct server_struct *myserver);
void server_update_last_job_time();
void server_set_dbus_finish_signal(struct server_struct *myserver, char *signal);

//server2
void server2_init(struct simulation *sim,struct server_struct *server);
void server2_malloc(struct simulation *sim,struct server_struct *server);
void server2_free(struct simulation *sim,struct server_struct *server);
void server2_add_job(struct simulation *sim,struct server_struct *server,struct job *j_in);
void server2_dump_jobs(struct simulation *sim,struct server_struct *server);
void server2_jobs_run(struct simulation *sim,struct server_struct *server);
struct worker* server2_get_next_worker(struct simulation *sim,struct server_struct *server);
void server2_job_finished(struct simulation *sim,struct job *j);
void server2_run_until_done(struct simulation *sim,struct server_struct *server);
void server2_dump_workers(struct simulation *sim,struct server_struct *server);
void server2_config_load(struct simulation *sim,struct server_struct *server);
void job_init(struct simulation *sim,struct job *j);
void job_free(struct simulation *sim,struct job *j);
void server2_free_finished_jobs(struct simulation *sim,struct server_struct *server);
#endif
