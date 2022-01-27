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

/** @file buffer.c
@brief used to save output files to disk with a nice header, so the user knows what was writtne to them
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "dat_file.h"
#include "gpvdm_const.h"
#include "code_ctrl.h"
#include "cal_path.h"
#include "dump.h"
#include <log.h>
//<strip>
#include <lock.h>
//</strip>

#include <triangle.h>
#include <triangles.h>


void dat_file_load_info(struct simulation *sim,struct dat_file *in,char *file_name)
{
char line[10000];
char temp2[200];
FILE *file;
file=fopen(file_name,"r");

if (file == NULL)
{
	ewe(sim,"Can not load the file %s\n",file_name);
}

do
{
	memset(line,0,10000);
	fgets(line, 10000, file);

	if (line[0]=='#')
	{
		if (strcmp_begin(line,"#x ")==0)
		{
			sscanf(line,"%s %d",temp2,&in->x);
		}else
		if (strcmp_begin(line,"#y ")==0)
		{
			sscanf(line,"%s %d",temp2,&in->y);
		}else
		if (strcmp_begin(line,"#z ")==0)
		{
			sscanf(line,"%s %d",temp2,&in->z);
		}else
		if (strcmp_begin(line,"#type ")==0)
		{
			sscanf(line,"%s %s",temp2,in->type);
		}


	}else
	{
		break;
	}


}while(!feof(file));
fclose(file);

}

