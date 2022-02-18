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

/** @file fit_constraints.c
@brief constrain the fit to phyicaly real vals.
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <config.h>
#include "fit.h"
#include <i.h>
#include <inp.h>
#include <gpvdm_const.h>
#include <util.h>
#include <cal_path.h>
#include <log.h>
#include <rpn.h>
#include <json.h>


static int unused __attribute__((unused));
static char* unused_pchar __attribute__((unused));

double get_constraints_error(struct simulation *sim,struct fitvars *config)
{
double ret=0.0;
return ret;
}

int get_fit_crashes(struct simulation *sim,struct fitvars *fitconfig)
{
int i;
int ret=0;
char temp[PATH_MAX];

for (i=0;i<fitconfig->data_sets;i++)
{
	if (fitconfig->data_set[i].enabled!=0)
	{
		join_path(2,temp,fitconfig->data_set[i].fit_path,"log.dat");

		if (log_search_error(temp)==0)
		{
			ret=ret+100;
		}
	}
}


return ret;
}
