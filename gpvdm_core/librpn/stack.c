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



