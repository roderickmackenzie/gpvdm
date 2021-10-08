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

/** @file eval.c
@brief evaluate math expresions for RPN
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
#include <math.h>

char* eval_sin(char *out,char* a,char* b)
{
	double aa=0.0;
	double bb=0.0;
	double sum=0.0;

	sscanf(a,"%le",&aa);
	//sscanf(b,"%le",&bb);
	sum=sin(aa);
	sprintf(out,"%le",sum);
}

char* eval_abs(char *out,char* a,char* b)
{
	double aa=0.0;
	double bb=0.0;
	double sum=0.0;

	sscanf(a,"%le",&aa);
	//sscanf(b,"%le",&bb);
	sum=fabs(aa);
	sprintf(out,"%le",sum);
}

char* eval_log10(char *out,char* a,char* b)
{
	double aa=0.0;
	double bb=0.0;
	double sum=0.0;

	sscanf(a,"%le",&aa);
	//sscanf(b,"%le",&bb);
	sum=log10(aa);
	sprintf(out,"%le",sum);
}

char* eval_pos(char *out,char* a,char* b)
{
	double aa=0.0;
	double bb=0.0;
	double sum=0.0;

	sscanf(a,"%le",&aa);
	//sscanf(b,"%le",&bb);
	if (aa<0)
	{
		sum=0.0;
	}else
	{
		sum=aa;
	}
	sprintf(out,"%le",sum);
}

char* eval_add(char *out,char* a,char* b)
{
	double aa=0.0;
	double bb=0.0;
	double sum=0.0;
	char ret[100];
	sscanf(a,"%le",&aa);
	sscanf(b,"%le",&bb);
	sum=aa+bb;
	sprintf(out,"%le",sum);
}

char* eval_sub(char *out,char* a,char* b)
{
	double aa=0.0;
	double bb=0.0;
	double sum=0.0;
	char ret[100];
	sscanf(a,"%le",&aa);
	sscanf(b,"%le",&bb);
	sum=aa-bb;
	sprintf(out,"%le",sum);
}


char* eval_mul(char *out,char* a,char* b)
{
	double aa=0.0;
	double bb=0.0;
	double sum=0.0;
	char ret[100];
	sscanf(a,"%le",&aa);
	sscanf(b,"%le",&bb);
	sum=aa*bb;
	sprintf(out,"%le",sum);
}

char* eval_bg(char *out,char* a,char* b)
{
	double aa=0.0;
	double bb=0.0;
	double sum=0.0;
	char ret[100];
	sscanf(a,"%le",&aa);
	sscanf(b,"%le",&bb);

	if (aa>bb)
	{
		sum=1.0;
	}

	sprintf(out,"%le",sum);
}

char* eval_sm(char *out,char* a,char* b)
{
	double aa=0.0;
	double bb=0.0;
	double sum=0.0;
	char ret[100];
	sscanf(a,"%le",&aa);
	sscanf(b,"%le",&bb);

	if (aa<bb)
	{
		sum=1.0;
	}

	sprintf(out,"%le",sum);
}

char* eval_pow(char *out,char* a,char* b)
{
	double aa=0.0;
	double bb=0.0;
	double sum=0.0;
	char ret[100];
	sscanf(a,"%le",&aa);
	sscanf(b,"%le",&bb);
	sum=pow(aa,bb);
	sprintf(out,"%le",sum);
}

char* eval_div(char *out,char* a,char* b)
{
	double aa=0.0;
	double bb=0.0;
	double sum=0.0;
	char ret[100];
	sscanf(a,"%le",&aa);
	sscanf(b,"%le",&bb);
	sum=aa/bb;
	sprintf(out,"%le",sum);
}



