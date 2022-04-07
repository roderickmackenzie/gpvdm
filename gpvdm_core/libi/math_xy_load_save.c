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
char wholename[PATH_MAX];
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




