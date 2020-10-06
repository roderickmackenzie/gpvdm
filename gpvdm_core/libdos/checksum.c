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

/** @file checksum.c
	@brief An MD5 like check sum for deciding if files have been changed.  Implemented because I do not want openssl dependencies.
*/

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "checksum.h"
#include <gpvdm_const.h>
#include "inp.h"
#include "util.h"
#include "cal_path.h"
#include "md5.h"
static int unused __attribute__((unused));

/*void calculate_dos_check_sum()
{
struct inp_file inp;
inp_init(sim,&inp);
inp_load(sim,&inp,full_name);
}*/

void checksum_write(struct simulation *sim,char *file_name)
{

FILE *file;
char *buffer;
unsigned long len;
long l;
char chkfile[100];
char full_name[PATH_MAX];
char temp[100];
int ret;
struct md5 sum;
md5_init(&sum);

get_file_name_from_path(temp,file_name);

ret=snprintf(chkfile,100,"%s.chk",temp);
if (ret<0)
{
	ewe(sim,"string error\n");
}
inp_read_buffer(sim,&buffer, &l,file_name);
len=(unsigned int)l;

md5_update(&sum,buffer,len);
md5_to_str(temp,&sum);

free(buffer);

gpvdm_mkdir(get_cache_path(sim));
join_path(2, full_name,get_cache_path(sim),chkfile);

file=fopen(full_name,"w");
if (file==NULL)
{
	ewe(sim,"Could not write %s not found\n",full_name);
}
fprintf(file,"%s\n",temp);
fclose(file);
}

int checksum_check(struct simulation *sim,char *file_name)
{
struct md5 sum;
md5_init(&sum);

FILE *file;
char *buffer;
unsigned long len;
char chkfile[100];
char newcheck[100];
char fromfile[100];
char full_name[PATH_MAX];
char temp[PATH_MAX];
int ret;
strcpy(newcheck,"hello");

get_file_name_from_path(temp,file_name);
ret=snprintf(chkfile,100,"%s.chk",temp);
if (ret<0)
{
	ewe(sim,"string error\n");
}
long l;
inp_read_buffer(sim,&buffer, &l,file_name);
len=(unsigned int)l;

md5_update(&sum,buffer,len);
md5_to_str(newcheck,&sum);
//checksum(newcheck,buffer, len);
free(buffer);

join_path(2, full_name,get_cache_path(sim),chkfile);

file=fopen(full_name,"r");
//printf("%s\n",full_name);
//getchar();
if (!file)
{
	return FALSE;
}

unused=fscanf(file,"%s\n",fromfile);
fclose(file);

if (strcmp(newcheck,fromfile)==0)
{
	return TRUE;
}else
{
	return FALSE;
}

return 0;
}



