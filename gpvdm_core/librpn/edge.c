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



