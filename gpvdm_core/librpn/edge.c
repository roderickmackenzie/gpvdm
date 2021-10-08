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

/** @file edge.c
@brief RPN detecting edges.
*/


#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "util.h"
#include "cal_path.h"
#include "gpvdm_const.h"
#include <rpn.h>
#include <log.h>

int isnumber(char a)
{
int i;
int ret=TRUE;
int digit=isdigit(a);

	if ((digit!=0)||(a=='.'))
	{
		return 0;
	}

return -1;

}

int edge_detect(struct simulation *sim,struct rpn *in,char *buf,char next)
{
	int last=strlen(buf);
	if (last==0)
	{
		return -1;
	}

	last--;

	if ((isnumber(buf[last])==0) && (next=='e'))
	{
		return -1;
	}

	if (last>=1)
	{
		if ((isnumber(buf[last-1])==0) && (buf[last]=='e') && ((next=='+') || (next=='-') || (isnumber(next)==0)) )
		{
			return -1;
		}
	}

	if (last>=2)
	{
		if ((isnumber(buf[last-2])==0) && (buf[last-1]=='e') && ((buf[last]=='+') || (buf[last]=='-')) && (isnumber(next)==0))
		{
			return -1;
		}
	}

	if ((isnumber(buf[last])==0) && (isnumber(next)!=0))
	{
		return 0;
	}

	if ((isnumber(buf[last])!=0) && (isnumber(next)==0))
	{
		return 0;
	}

	if ((next=='(') || (next==')'))
	{
		return 0;
	}

	if (strcmp(buf,")")==0)
	{
		return 0;
	}

	if (strcmp(buf,"(")==0)
	{
		return 0;
	}

	if (strcmp(buf,"*")==0)
	{
		return 0;
	}

	int i;
	for (i=0;i<in->opp_count;i++)
	{
		if ((strcmp(buf,in->opps[i].name)==0)||(in->opps[i].name[0]==next))
		{

			return 0;
		}
	}
/*
	if ((next=='>')||strcmp(buf,">")==0)
	{
		return 0;
	}

	if ((next=='>')||strcmp(buf,">")==0)
	{
		return 0;
	}

	if ((next=='-')||strcmp(buf,"-")==0)
	{
		return 0;
	}

	if ((next=='+')||strcmp(buf,"+")==0)
	{
		return 0;
	}

	if ((next=='/')||strcmp(buf,"/")==0)
	{
		return 0;
	}
*/
	if (strcmp(buf,"^")==0)
	{
		return 0;
	}
	return -1;
}



