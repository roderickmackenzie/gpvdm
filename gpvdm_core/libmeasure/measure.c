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

/** @file measure.c
	@brief Job management for fitting, run multiple fitting instances over multiple CPUs.
*/

#include "util.h"
#include "gpvdm_const.h"
#include "light.h"
#include "device.h"
#include "gpvdm_const.h"
#include <dump.h>
#include "config.h"
#include "inp.h"
#include "util.h"
#include "cal_path.h"
#include "lang.h"
#include "log.h"
#include "measure.h"
#include <probe.h>
#include <dat_file.h>
#include <sys/stat.h>
#include <i.h>
#include <rpn.h>
#include <list.h>

static int unused __attribute__((unused));

struct probe_config config;

static struct math_xy spectrum_first;
static int first=FALSE;

void measure_file(struct simulation *sim,char *output_path,char *input_path,char *file_name)
{
	int i;
	int vector;
	int enable;
	char vec_str[400];
	char file[400];
	char data_path[400];
	long double position;
	char output_token[400];
	char temp[4000];
	char sim_name[400];

	char math[400];
	FILE *out=NULL;
	struct inp_file inp;
	struct inp_file inp_data;
	char input_position[400];
	char output_file[400];
	int sprintf_ret=0;

	struct rpn rpn_cal;
	long double value;
	long double y;
	int write=FALSE;
	int ret=0;
	strcpy(vec_str,"");

	join_path(2, file,input_path,file_name);

	inp_init(sim,&inp);

	if (inp_load(sim, &inp , file)!=0)
	{
		ewe(sim,"Measure: I can't find file %s\n",file);
	}

	inp_check(sim,&inp,1.0);
	inp_reset_read(sim,&inp);
	struct math_xy data;
	inter_init(sim,&data);

	inp_get_string(sim,sim_name,&inp);	//name
	inp_get_string(sim,sim_name,&inp);	//

	inp_get_string(sim,temp,&inp);	//enable
	inp_get_string(sim,temp,&inp);

	enable=english_to_bin(sim,temp);

	if (enable==FALSE)
	{
		inp_free(sim,&inp);
		return;
	}

	inp_get_string(sim,temp,&inp);	//compile to vector
	inp_get_string(sim,temp,&inp);
	vector=english_to_bin(sim,temp);

	sprintf_ret=snprintf(output_file,400,"measure_%s.dat",sim_name);
	if (sprintf_ret<0)
	{
		ewe(sim,"error\n");
	}

	while(1)
	{
		write=FALSE;

		inp_get_string(sim,temp,&inp);	//file token
		if (strcmp(temp,"#ver")==0)
		{
			break;
		}

		inp_get_string(sim,file_name,&inp);


		inp_get_string(sim,temp,&inp);	//position token
		inp_get_string(sim,input_position,&inp);

		inp_get_string(sim,temp,&inp);	//output

		inp_get_string(sim,output_token,&inp);	//output token
		//printf("%s %Lf %s\n",file_name,position,output_token);

		join_path(2,data_path,output_path,file_name);

		inp_get_string(sim,temp,&inp);
		inp_get_string(sim,math,&inp);	//output token

		if (strcmp_begin(input_position,"#")==0)
		{
				inp_init(sim,&inp_data);
				if (inp_load(sim, &inp_data , data_path)==0)
				{
					ret=inp_search_gdouble(sim,&inp_data,&value,input_position);

					if (ret==0)
					{
						rpn_init(sim,&rpn_cal);
						rpn_add_var(sim,&rpn_cal,"x",(double)value);

						y=(long double)rpn_evaluate(sim,&rpn_cal,math);
						//printf("%Le %s %Le %s\n",y,math,value,input_position);
						//getchar();
						inp_free(sim,&inp_data);
						write=TRUE;
					}else
					{
						printf_log(sim,_("Token not found: %s\n"),input_position);
					}

				}else
				{
					printf_log(sim,_("File not found: %s\n"),data_path);
				}
		}else
		{
			if (inter_load(sim,&data,data_path)==0)
			{
				inter_sort(&data);
				//inter_dump(sim,&data);
				sscanf(input_position,"%Le",&(position));
				y=inter_get_hard(&data,position);

				rpn_init(sim,&rpn_cal);
				rpn_add_var(sim,&rpn_cal,"x",(double)y);

				y=(long double)rpn_evaluate(sim,&rpn_cal,math);

				inter_free(&data);
				write=TRUE;
			}
		}

		if (write==TRUE)
		{
			if (out==NULL)
			{
					out=fopena(output_path,output_file,"w");
			}

			if (vector==FALSE)
			{
				fprintf(out,"#%s\n",output_token);
				fprintf(out,"%Le\n",y);
			}else
			{
				sprintf(temp,"%Le ",y);
				strcat(vec_str,temp);
			}
		}

	}

	inp_free(sim,&inp);

	if (out!=NULL)
	{
		if (vector==FALSE)
		{
			fprintf(out,"#ver\n");
			fprintf(out,"#1.0\n");
			fprintf(out,"#end\n");
		}else
		{
			fprintf(out,"%s",vec_str);
		}

		fclose(out);
	}

}


void measure(struct simulation *sim, char *output_path,char *input_path)
{
int i=0;
struct list a;
inp_listdir(sim,input_path,&a);


	for (i=0;i<a.len;i++)
	{
		if ((strcmp(a.names[i],".")!=0)&&(strcmp(a.names[i],"..")!=0))
		{
			if ((cmpstr_min(a.names[i],"measure")==0)&&(strcmp_end(a.names[i],".inp")==0))
			{
				measure_file(sim,output_path,input_path,a.names[i]);
			}
		}
	}

list_free(&a);
}
