#ifndef tx_packet_h
#define tx_packet_h
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include "util.h"
#include <sys/stat.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "tx_packet.h"
#include "zlib.h"

#define TX_STRUCT_DATA_ITEM_SIZE 200
struct tx_struct
{
char id[TX_STRUCT_DATA_ITEM_SIZE];
char src[TX_STRUCT_DATA_ITEM_SIZE];
char file_name[TX_STRUCT_DATA_ITEM_SIZE];
int size;
char target[TX_STRUCT_DATA_ITEM_SIZE];
char message[TX_STRUCT_DATA_ITEM_SIZE];
double load0;
double load1;
double load2;
char ip[40];
int stat;
int zip;
char *data;
int uzipsize;
char exe_name[TX_STRUCT_DATA_ITEM_SIZE];
char dir_name[TX_STRUCT_DATA_ITEM_SIZE];
char command[TX_STRUCT_DATA_ITEM_SIZE];
char host_name[TX_STRUCT_DATA_ITEM_SIZE];
char job[TX_STRUCT_DATA_ITEM_SIZE];
char token[TX_STRUCT_DATA_ITEM_SIZE];
int cpus;
int percent;
int pid;
};

void tx_struct_init(struct tx_struct *in);
void tx_set_id(struct tx_struct *in,char *id);
void tx_set_size(struct tx_struct *in,int size);
int tx_packet(int sock,struct tx_struct *in,char *buf);
void tx_set_file_name(struct tx_struct *in,char *file_name);
void tx_set_target(struct tx_struct *in,char *target);
void tx_set_stat(struct tx_struct *in,int stat_in);
int rx_packet(int sock,struct tx_struct *in);
void tx_set_src(struct tx_struct *in,char *src);
void tx_set_zip(struct tx_struct *in,int zip);
void tx_set_uzipsize(struct tx_struct *in,int uzipsize);
void tx_thing_done(int sock,struct tx_struct* in_packet);
void tx_pid(int thread,char *dir_name);
#endif
