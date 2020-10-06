//
//  General-purpose Photovoltaic Device Model gpvdm.com- a drift diffusion
//  base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
//
//  Copyright (C) 2012 Roderick C. I. MacKenzie <r.c.i.mackenzie@googlemail.com>
//
//	www.roderickmackenzie.eu
//	Room B86 Coates, University Park, Nottingham, NG7 2RD, UK
//
//
// This program is free software; you can redistribute it and/or modify it
// under the terms and conditions of the GNU General Public License,
// version 2, as published by the Free Software Foundation.
//
// This program is distributed in the hope it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.

/** @file util.h
@brief header for cluster functions, too much is in here.
*/
#ifndef util_h
#define util_h

#define TRUE 1
#define FALSE 0

#define BACKLOG 10
#define LENGTH 512 // Buffer length
#include <state.h>

struct state *get_sim();

struct job
{
	char name[100];
	int done;
	int status;
	int cpus_needed;
	char target[100];
	char ip[100];
	int copy_state;
	time_t t_start;
	time_t t_stop;
	time_t t_force_stop;
	int pid;
};


struct node_struct
{
	char ip[100];
	char type[100];
	char host_name[100];
	int cpus;
	int load;
	int sock;
	double load0;
	int max_cpus;
	time_t alive;
};

#include "tx_packet.h"

void packet_init_mutex();
void state_init(struct state *sim);
int nodes_html_load(char *buf);
void stop_all_jobs();
void calpath_set_exe_name(char *in);
char* calpath_get_exe_name();
int send_all(int sock, void *buffer, int length, int encode);
int set_master_load();

struct node_struct* node_find_master();
struct node_struct* node_find(char *ip);

void nodes_print();
int node_add(char *name,char *ip, int cpus,int sock,char *host_name);

void copy_packet(struct tx_struct *out,struct tx_struct *in);
int send_file(struct state *sim,int sockfd,char *base_name,char *file_name,char *target);
void mkdirs(char *dir);
int cmpstr_min(char * in1,char *in2);
int english_to_bin( char * in);
int ewe( const char *format, ...);
char *get_file_name_from_path(char *in);
int get_dir_name_from_path(char *out, char *in);
void join_path(int max, ...);
int head(struct state *sim);
int node(struct state *sim);

int file_rx_and_save(char *file_name,int sock_han,int size);
int send_dir(struct state *sim,int sockfd,const char *name, int level,char *base_name,char *target);

int register_node(int sock);
int send_delete_node(int sock);

void node_delete(char *ip);
struct job* jobs_get_next();
int send_command(int sockfd,char *command,char *dir_name,int cpus);


char* get_my_ip();
int cal_my_ip(int sock);
int get_ip_from_sock(char *out,int sock);
double jobs_cal_percent_finished();
int jobs_remaining();
void jobs_clear_all();
int close_all_open();
int broadcast_to_nodes(struct tx_struct *packet);
int send_packet_to_node(char *node,struct tx_struct *packet);
int nodes_txnodelist();

void remove_dir(char* dir_name);
int isdir(char *dir);
void copy_dir_to_all_nodes(struct state *sim,char *dir);

void jobs_print();
void jobs_reset();
void jobs_add(char *name,char *target,int cpus);
int tx_job_list();

void *rx_loop(void *s);
void run_jobs(struct state *sim);
struct job* get_jobs_array();
int get_njobs();
void set_njobs(int n);


void calpath_set_store_path(char *in);
char* calpath_get_store_path();

struct job* jobs_find_job(char *name);



void textcolor(int color);
void set_porgress_color(int in);
void set_progress_colored();
void set_porgress_nospin();
void set_porgress_noreset();
void set_porgress_max(int in);
void text_progress(double percent);
void progress_clear(int n);
void text_progress_finish();
void set_progress_multi_line_text(char *in);
void set_progress_multi_line();
void text_progress_start(char *in);


int cmp_send_job_list(int sock_han,struct tx_struct *data);
int cmp_node_send_data(struct state *sim,int sock,struct tx_struct *data);
int cmp_get_data(struct state *sim,int sock,struct tx_struct *data);
int cmp_node_runjob(struct state *sim,struct tx_struct *data);
int cmp_addnode(int sock_han,struct tx_struct *data);
int cmp_rxfile(int sock_han,struct tx_struct *data,struct state *sim);
int cmp_addjob(int sock_han,struct tx_struct *data);
int cmp_deletenode(int sock_han,struct tx_struct *data);
int cmp_simfinished(struct state *sim,int sock,struct tx_struct *data);
int cmp_node_killall(int sock,struct tx_struct *data);
int cmp_node_killjob(int sock,struct tx_struct *data);
int cmp_head_killall(int sock,struct tx_struct *data);
int cmp_node_sleep(int sock,struct tx_struct *data);
int cmp_head_sleep(int sock,struct tx_struct *data);
int cmp_node_poweroff(int sock,struct tx_struct *data);
int cmp_head_poweroff(int sock,struct tx_struct *data);
int cmp_sendnodelist(int sock,struct tx_struct *data);
int cmp_head_exe(struct state *sim,int sock,struct tx_struct *data);
int cmp_register_master(int sock,struct tx_struct *data);
int cmp_master_clean(int sock,struct tx_struct *data);
int cmp_slave_clean(int sock,struct tx_struct *data);
int cmp_runjobs(struct state *sim,int sock_han,struct tx_struct *data);
int cmp_head_stop_all_jobs(int sock,struct tx_struct *data);
int cmp_rxloadstats(int sock,struct tx_struct *data);
int cmp_nodeload(int sock,struct tx_struct *data);
int cmp_node_quit(int sock,struct tx_struct *data);
int cmp_head_quit(int sock,struct tx_struct *data);
int cmp_rxsetmaxloads(int sock,struct tx_struct *data);
int cmp_sync_packet_one(int sock_han,struct tx_struct *data);
int cmp_sync_packet_two(struct state *sim,int sock,struct tx_struct *data);
int cmp_delete_all_jobs(int sock,struct tx_struct *data);
int cmp_job_pid(struct state *sim,int sock,struct tx_struct *data);

void encrypt(char *data,int round_len);
void decrypt(char *data,int round_len);
int send_node_load(int sock);

void encrypt_load();
int node_alive_time(struct node_struct* node);
int send_message(char *message);
int recv_all(int sock,char *buf, int buf_len);

int cal_abs_path_from_target(char *full_path,char *target,char *file_name);
int gen_dir_list(char ** out,int *len,int *pos,const char *path,char *base_path);
int tx_sync_packet_one(int sock,char *src, char* target);

void gen_job_list(char *buf);
int jobs_load();
void jobs_save();
int nodes_get_nnodes();
void log_alarm_wakeup (int i);
struct node_struct *nodes_list();
int update_pids(int *list, int *list_len,int want_id);
void kill_all(int want_id);
//Debug
void set_debug(int value);
void debug_printf( const char *format, ...);

#endif
