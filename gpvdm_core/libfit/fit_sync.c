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

/** @file fit_sync.c
@brief sync the fit data to the main node if being run on an HPC - not a good idea to use
*/

#include <enabled_libs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <i.h>
#include <server.h>
#include "fit.h"
#include <dump_ctrl.h>
#include <timer.h>
#include <util.h>
#include <rand.h>
#include <inp.h>
#include <gpvdm_const.h>
#include <log.h>

static int unused __attribute__((unused));

//Sync everything back to the server
void do_copy(struct simulation *sim)
{
int ret=0;
FILE *file;
char path[PATH_MAX];
char command[PATH_MAX];

	if ((file=fopen("rsyncdir.inp","r"))==NULL)
	{
		printf_log(sim,"No rsyncdir.inp file so not copying\n");
		exit(0);
	}else
	{

		unused=fscanf(file,"%s",path);
		fclose(file);

		ret=snprintf(command,PATH_MAX,"cp ./*.dat ./*.inp %s\n",path);	
		if (ret<0)
		{
			ewe(sim,"sprintf error\n");
		}

		printf_log(sim,"Running command %s\n",command);
		unused=system(command);

		ret=snprintf(command,PATH_MAX,"cp ./materials %s -rf\n",path);
		if (ret<0)
		{
			ewe(sim,"sprintf error\n");
		}

		printf_log(sim,"Running command %s\n",command);
		unused=system(command);

	}

}

