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




/** @file i.c
	@brief Simple functions to read in scientific data from text files and perform simple maths on the data.
*/
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sim_struct.h>

#include "i.h"
#include "util.h"
#include "cal_path.h"
#include "gpvdm_const.h"
#include <log.h>
#include "inp.h"
#include <memory.h>


static int unused __attribute__((unused));
static char* unused_pchar __attribute__((unused));


/**Load data from a file
@param in the math_xy holding the data
@param name The file name.
*/
int inter_load(struct simulation *sim,struct math_xy* in,char *name)
{
char temp[1000];
struct inp_file data;
long double x;
long double y;
long len;

strcpy(in->name,name);


inp_init(sim,&data);

if (inp_load(sim,&data,name)!=0)
{
	return -1;
}


inter_init(sim,in);
char line[4000];
int ret_val=inp_get_string(sim,line,&data);
while(ret_val==0)
{
	//
	//unused_pchar=gpvdm_fgets(temp, 1000, file);
	if ((line[0]!='#')&&(line[0]!='\n')&&(line[0]!='\r')&&(line[0]!=0)&&(line[0]!=0x0D))
	{
		sscanf(line,"%Le %Le",&(x),&(y));

		inter_append(in,x,y);
	}
	ret_val=inp_get_string(sim,line,&data);
}

inp_free(sim,&data);
return 0;
}


/**Print math_xy to screen
@param in struct to print
*/
void inter_dump(struct simulation *sim,struct math_xy* in)
{
int i=0;
for  (i=0;i<in->len;i++)
{
	printf_log(sim,"%Le %Le\n",in->x[i],in->data[i]);
}

}

/**Save an math_xy to disk and define path
@param in struct to save
@param path path of output file
@param path name of output file
*/
void inter_save_backup(struct math_xy* in,char *name,int backup)
{
char wholename[200];
char backup_file[200];
sprintf(wholename,"%s",name);
if (backup==FALSE)
{
	inter_save(in,wholename);
}else
{

	sprintf(backup_file,"%s.back",name);

	if( access( wholename, F_OK ) != -1 )
	{
	remove(backup_file);
	rename(wholename,backup_file);
	}
	inter_save(in,wholename);
}


}


/**Save an math_xy to disk and define path
@param in struct to save
@param path path of output file
@param path name of output file
*/
void inter_save_a(struct math_xy* in,char *path,char *name)
{
char wholename[200];
join_path(2, wholename,path,name);
inter_save(in,wholename);
}

void inter_save_seg(struct math_xy* in,char *path,char *name,int seg)
{
FILE *file=NULL;
int i=0;

int count_max=in->len/seg;
int count=0;
char temp[1000];
char file_name[1000];
int file_count=0;
for  (i=0;i<in->len;i++)
{
	if (count==0)
	{
		sprintf(file_name,"%s%d.dat",name,file_count);

		join_path(2, temp,path,file_name);

		file=fopen(temp,"w");
		file_count++;
	}
		fprintf(file,"%Le",in->x[i]);
		fprintf(file," %Le",in->data[i]);
	count++;
	fprintf(file,"\n");

	if (count==count_max)
	{
		fclose(file);
		count=0;
	}

}
if (count!=0) fclose(file);

}


/**Save an math_xy to disk
@param in struct to save
@param name outputfile
*/
void inter_save(struct math_xy* in,char *name)
{
FILE *file;
file=fopen(name,"w");
int i=0;
for  (i=0;i<in->len;i++)
{
	fprintf(file,"%Le %Le",in->x[i],in->data[i]);
	if (in->complex_enabled==TRUE)
	{
		fprintf(file," %Le",in->imag[i]);
	}
	fprintf(file,"\n");
}

fclose(file);
}




