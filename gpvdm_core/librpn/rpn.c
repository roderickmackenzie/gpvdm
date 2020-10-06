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

/** @file rpn.c
@brief RPN main
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


void rpn_init(struct simulation *sim,struct rpn *in)
{
	in->output_pos=0;
	in->stack_pos=0;
	in->vars_pos=0;
	in->functions_count=0;
	in->opp_count=0;
	add_opp(sim,in,"^", 4, RIGHT,&eval_pow);
	add_opp(sim,in,"*", 3, LEFT,&eval_mul);
	add_opp(sim,in,"/", 3, LEFT,&eval_div);
	add_opp(sim,in,"+", 2, LEFT,&eval_add);
	add_opp(sim,in,"-", 2, LEFT,&eval_sub);
	add_opp(sim,in,">", 3, LEFT,&eval_bg);
	add_opp(sim,in,"<", 3, LEFT,&eval_sm);

	add_function(sim,in,"sin",&eval_sin);
	add_function(sim,in,"abs",&eval_abs);
	add_function(sim,in,"pos",&eval_pos);
	add_function(sim,in,"log",&eval_log10);
}

void pro(struct simulation *sim,struct rpn *in,char *buf,int type)
{
int ii=0;
int temp_n=0;
char temp[100];
int push=1;
int o1_lr=0;
int o1_pr=0;
int o2_lr=0;
int o2_pr=0;

	//printf("edge %s %d\n",buf,isnumber(buf[0]));

	if (rpn_is_var(sim,in,NULL,buf)==0)
	{
		output_push(sim,in,buf);
		return;
	}
	else
	if (isnumber(buf[0])==0)
	{
		output_push(sim,in,buf);
		return;
	}

	if (strcmp(buf,")")==0)
	{
		temp_n=in->stack_pos;
		for (ii=0;ii<temp_n;ii++)
		{
			strcpy(temp,stack_pop(sim,in));
			if (strcmp(temp,"(")!=0)
			{
				output_push(sim,in,temp);
			}else
			{
				if (is_function(sim,in,stack_peak(sim,in))!=-1)
				{
					output_push(sim,in,stack_pop(sim,in));
				}
				break;
			}
		}

		return;
	}

	if (strcmp(buf,"(")==0)
	{
		stack_push(sim,in,buf);

		return;
	}

	if (is_opp(sim,in,buf)!=-1)
	{
		//printf("here %s\n",buf);
		o1_lr=opp_lr(sim,in,buf);
		o1_pr=opp_pr(sim,in,buf);
		//printf("r\n");

		while(is_opp(sim,in,stack_peak(sim,in))!=-1)
		{
			//printf("r %s %s\n",buf,stack_peak());

			o2_pr=opp_pr(sim,in,stack_peak(sim,in));
			if (o1_lr==LEFT)
			{
				if (o1_pr<=o2_pr)
				{
					output_push(sim,in,stack_pop(sim,in));
				}else
				{
					break;
				}
			}else
			{
				if (o1_pr<o2_pr)
				{
					output_push(sim,in,stack_pop(sim,in));
				}else
				{
					break;
				}
			}
		}

		stack_push(sim,in,buf);
		//print_stack(sim,in);
		//print_output(sim,in);
		//getchar();
	}

	if (is_function(sim,in,buf)!=-1)
	{
		stack_push(sim,in,buf);
	}

}

double evaluate(struct simulation *sim,struct rpn *in)
{
	double ret=0;
	int i=0;
	int n=in->output_pos;
	char *token;
	char *right;
	char *left;
	char result[100];
	char temp[100];
	//printf("eval:\n");
	for (i=0;i<in->output_pos;i++)
	{
		token=in->output[i];
		if (rpn_is_var(sim,in,temp,token)==0)
		{
			stack_push(sim,in,temp);
		}

		if (isnumber(token[0])==0)
		{
			stack_push(sim,in,token);
		}

		if (is_opp(sim,in,token)!=-1)
		{
			//printf(">token = %s<\n",token);
			right=stack_pop(sim,in);
			left=stack_pop(sim,in);
			//printf("%s %s\n",left,right);
			opp_run(sim,in,token,result,left,right);
			//printf("%s\n",result);
			stack_push(sim,in,result);
			//printf("%s %s %s\n",left,token,right);
			//print_stack(sim,in);
		}

		if (is_function(sim,in,token)!=-1)
		{
			//printf(">function = %s<\n",token);
			left=stack_pop(sim,in);
			right=NULL;
			function_run(sim,in,token,result,left,right);
			//printf(">>>>%s %s %s\n",left,right,result);
			stack_push(sim,in,result);
			//printf("%s %s %s\n",left,token,right);
			//print_stack(sim,in);
		}

	}

	sscanf(in->stack[0],"%le",&ret);
	return ret;
}

double rpn_evaluate(struct simulation *sim,struct rpn *in, char *string)
{
	double ret=0.0;
	int i=0;
	char temp[100];
	int n=isnumber(string[0]);
	int last=n;
	char buf[40];
	strcpy(buf,"");
	int buf_len=0;
	int temp_n=0;
	for (i=0;i<strlen(string);i++)
	{
		if (edge_detect(sim,in,buf,string[i])==0)
		{
			pro(sim,in,buf,last);
			strcpy(buf,"");
			buf_len=0;
			//print_stack(sim,in);
			//print_output(sim,in);
			//getchar();
		}

		if (string[i]!=' ')
		{
			buf[buf_len]=string[i];
			buf[buf_len+1]=0;
			buf_len++;
		}

		if (i==strlen(string)-1)
		{
			pro(sim,in,buf,last);
		}

		last=n;

	}

	n=in->stack_pos;
	for (i=0;i<n;i++)
	{
		output_push(sim,in,stack_pop(sim,in));
	}

	//print_output(sim,in);

	//print_stack(sim,in);
	ret=evaluate(sim,in);
	return ret;
}


