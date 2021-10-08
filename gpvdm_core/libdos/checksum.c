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


