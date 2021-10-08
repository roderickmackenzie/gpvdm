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

/** @file server.h
@brief header file for the internal server used to run jobs across multiple CPUs
*/

#ifndef serverh
#define serverh
#include <sim_struct.h>
#include <server_struct.h>
#include <json.h>

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
void server2_jobs_run(struct simulation *sim,struct server_struct *server,int batch_id);
struct worker* server2_get_next_worker(struct simulation *sim,struct server_struct *server);
void server2_job_finished(struct simulation *sim,struct job *j);
void server2_run_until_done(struct simulation *sim,struct server_struct *server,int batch_id);
void server2_dump_workers(struct simulation *sim,struct server_struct *server);
void server2_config_load(struct simulation *sim,struct server_struct *server,struct json_obj *json_server);
void job_init(struct simulation *sim,struct job *j);
void job_free(struct simulation *sim,struct job *j);
void server2_free_finished_jobs(struct simulation *sim,struct server_struct *server, int batch_id);
int server2_get_next_batch_id(struct simulation *sim,struct server_struct *server);
#endif
