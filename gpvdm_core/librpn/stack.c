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

/** @file stack.c
@brief RPN stack - all based off wikipedia
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

void output_push(struct simulation *sim,struct rpn *in,char *val)
{
	strcpy(in->output[in->output_pos++],val);
}

void stack_push(struct simulation *sim,struct rpn *in,char *val)
{
	//printf("stack push %d %s\n",in->stack_pos,val);
	strcpy(in->stack[in->stack_pos++],val);
}

char* stack_pop(struct simulation *sim,struct rpn *in)
{
	//printf("stack pop %d %s\n",in->stack_pos,in->stack[in->stack_pos-1]);
	if (in->stack_pos==0)
	{
		strcpy(in->stack[0],"");
		return in->stack[0];
	}
	return in->stack[--in->stack_pos];
}

char* stack_peak(struct simulation *sim,struct rpn *in)
{
	if (in->stack_pos!=0)
	{
		return in->stack[in->stack_pos-1];
	}else
	{
		return "";
	}
}

void print_stack(struct simulation *sim,struct rpn *in)
{
	int i;
	printf("stack:\n");
	for (i=0;i<in->stack_pos;i++)
	{
		printf(">%s<\n",in->stack[i]);
	}
}
void print_output(struct simulation *sim,struct rpn *in)
{
	int i=0;
	printf("output:\n");
	for (i=0;i<in->output_pos;i++)
	{
		printf(">%s<\n",in->output[i]);
	}
}



