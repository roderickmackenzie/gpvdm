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

/** @file opp.c
@brief RPN opps.
*/


#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "util.h"
#include "cal_path.h"
#include "gpvdm_const.h"
#include <rpn.h>
#include <log.h>
#include <sim_struct.h>


void add_opp(struct simulation *sim,struct rpn *in,char *name, int prec, int right_left,void *f)
{
	strcpy(in->opps[in->opp_count].name,name);
	in->opps[in->opp_count].prec=prec;
	in->opps[in->opp_count].right_left=right_left;
	in->opps[in->opp_count].f=f;
	in->opp_count++;
}

int is_opp(struct simulation *sim,struct rpn *in,char *val)
{
	int i;
	for (i=0;i<in->opp_count;i++)
	{
		if (strcmp(val,in->opps[i].name)==0)
		{

			return i;
		}
	}
return -1;
}

char* opp_run(struct simulation *sim,struct rpn *in,char *val,char *out,char* a,char* b)
{
	int i;
	for (i=0;i<in->opp_count;i++)
	{
		if (strcmp(val,in->opps[i].name)==0)
		{
			return (in->opps[i].f)(out,a,b);
		}
	}
return "error";
}

int opp_pr(struct simulation *sim,struct rpn *in,char *val)
{
	return in->opps[is_opp(sim,in,val)].prec;
}

int opp_lr(struct simulation *sim,struct rpn *in,char *val)
{
	return in->opps[is_opp(sim,in,val)].right_left;
}



