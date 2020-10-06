// 
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

/** @file rpn.h
@brief RPN functions which gpvdm can handle.
*/

#ifndef rpn_h
#define rpn_h

struct rpn_function_type
{
	char name[10];
	char* (*f)(char* out,char* a, char* b);
};

struct rpn_opperator_type
{
	char name[10];
	int prec;
	int right_left;
	char* (*f)(char* out,char* a, char* b);

};

struct rpn_vars_type
{
	char name[10];
	double value;
};

struct rpn
{
	char output[10][40];
	int output_pos;

	char stack[10][40];
	int stack_pos;

	struct rpn_vars_type vars[40];
	int vars_pos;


	struct rpn_function_type functions[40];
	int functions_count;

	struct rpn_opperator_type opps[40];
	int opp_count;

	int last_was_number;
	int last_was_e;

};

//vars
void rpn_add_var(struct simulation *sim,struct rpn *in,char *name,double value);
int rpn_is_var(struct simulation *sim,struct rpn *in,char *out,char *name);

//rpn
void rpn_init(struct simulation *sim,struct rpn *in);
void add_var(struct simulation *sim,struct rpn *in,char *name,double value);
void add_function(struct simulation *sim,struct rpn *in,char *name,void *f);
int is_function(struct simulation *sim,struct rpn *in,char *val);
char* function_run(struct simulation *sim,struct rpn *in,char *val,char *out,char* a,char* b);

//opp
void add_opp(struct simulation *sim,struct rpn *in,char *name, int prec, int right_left,void *f);
int is_opp(struct simulation *sim,struct rpn *in,char *val);
char* opp_run(struct simulation *sim,struct rpn *in,char *val,char *out,char* a,char* b);
int opp_pr(struct simulation *sim,struct rpn *in,char *val);
int opp_lr(struct simulation *sim,struct rpn *in,char *val);

//stack
void output_push(struct simulation *sim,struct rpn *in,char *val);
void stack_push(struct simulation *sim,struct rpn *in,char *val);
char* stack_pop(struct simulation *sim,struct rpn *in);
char* stack_peak(struct simulation *sim,struct rpn *in);
void print_stack(struct simulation *sim,struct rpn *in);
void print_output(struct simulation *sim,struct rpn *in);

int isnumber(char a);


int edge_detect(struct simulation *sim,struct rpn *in,char *buf,char next);
void pro(struct simulation *sim,struct rpn *in,char *buf,int type);
double rpn_evaluate(struct simulation *sim,struct rpn *in,char *string);

//Functions
char* eval_sin(char *out,char* a,char* b);
char* eval_abs(char *out,char* a,char* b);
char* eval_pos(char *out,char* a,char* b);
char* eval_add(char *out,char* a,char* b);
char* eval_sub(char *out,char* a,char* b);
char* eval_mul(char *out,char* a,char* b);
char* eval_pow(char *out,char* a,char* b);
char* eval_div(char *out,char* a,char* b);
char* eval_log10(char *out,char* a,char* b);
char* eval_bg(char *out,char* a,char* b);
char* eval_sm(char *out,char* a,char* b);
#endif
