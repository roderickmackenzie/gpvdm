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

/** @file log.c
	@brief Deal with output to the log file, can either be disk/screen or both.  UTF8 supported/HTML supported and can stream to GUI under Windows/Linux.
*/


#include <enabled_libs.h>
#include <stdio.h>
#include <stdarg.h>
#include "log.h"
#include <colors.h>
#include <time.h>
#include <stdlib.h>
#include <util.h>
#include <cal_path.h>
#include <string.h>
#include <gpvdm_const.h>
#include <lang.h>
#include <dump.h>
#include <wchar.h>
#include <color.h>

void randomprint(struct simulation *sim,char *in)
{
	int i;

	wchar_t wide[1000];
	//char temp[1000];
	int len=mbstowcs(wide, in, 1000);
	//wprintf(L"%S",wide);
	for (i=0;i<len;i++)
	{
	int rnd=(float)5.0*rand()/(float)RAND_MAX;
		if (rnd==0) textcolor(sim,fg_wight);
		if (rnd==1) textcolor(sim,fg_red);
		if (rnd==2) textcolor(sim,fg_green);
		if (rnd==3) textcolor(sim,fg_yellow);
		if (rnd==4) textcolor(sim,fg_blue);
		if (rnd==5) textcolor(sim,fg_purple);

		if ((wide[i]!='\n')||(sim->html==FALSE))
		{
			//mbstowcs(wide, in, 1000);
			//swprintf(temp,L"%C",wide[i]);
			printf_log(sim,"%C",wide[i]);
		}else
		{
			printf_log(sim,"<br>");
		}

		textcolor(sim,fg_reset);

		}

fflush(stdout);
}

void log_time_stamp(struct simulation *sim)
{
	time_t t=0;
	t=time(NULL);

	struct tm tm = *localtime(&t);

	printf_log(sim,"time: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void log_clear(struct simulation *sim)
{
	//FILE* out;
	char temp[500];
	/*
	join_path(2,temp,get_output_path(sim),"log.dat");
	remove_file(sim,temp);
	//out=fopen(temp,"w");
	//fprintf(out,"gpvdm log file:\n");
	//fclose(out);
	*/
	//join_path(2,temp,sim->path,"log_file_access.dat");
	//remove_file(sim,temp);

	//out=fopen(temp,"w");
	//fprintf(out,"gpvdm file access log file:\n");
	//fclose(out);

}

void log_tell_use_where_file_access_log_is(struct simulation *sim)
{
	if ((sim->log_level==log_level_disk)||(sim->log_level==log_level_screen_and_disk))
	{
		char temp[500];
		//join_path(2,temp,sim->path,"log_file_access.dat");
		//printf_log(sim,_("File access log written to %s\n"),temp);
	}

}
void log_write_file_access(struct simulation *sim,char * file,char mode)
{
	/*if ((sim->log_level==log_level_disk)||(sim->log_level==log_level_screen_and_disk))
	{
		FILE* out;
		char temp[500];
		join_path(2,temp,sim->path,"log_file_access.dat");
		out=fopen(temp,"a");
		if (mode=='w')
		{
			fprintf(out,"write:%s\n",file);
		}else
		{
			fprintf(out,"read:%s\n",file);
		}

		fclose(out);
	}*/
}

void set_logging_level(struct simulation *sim,int value)
{
	sim->log_level=value;
}

void text_to_html(struct simulation *sim,char *out, char *in,int max)
{
	if (sim->html==TRUE)
	{

		int i=0;
		int out_pos=0;
		int len=0;
		len=strlen(in);
		for (i=0;i<len;i++)
		{
			if (in[i]=='\n')
			{
				out[out_pos]=0;
				strcat(out,"<br>");
				out_pos=strlen(out);
			}else
			{
				out[out_pos]=in[i];
				out_pos++;
				if (out_pos>=max)
				{
					out_pos--;
					break;
				}
			}

		}

		out[out_pos]=0;
	}else
	{
		out[0]=0;
		strncpy(out,in,max);
	}

}

void printf_log(struct simulation *sim, const char *format, ...)
{
	FILE* out;
	char data[STR_MAX];
	char data_html[STR_MAX];
	char temp[PATH_MAX];
	va_list args;
	va_start(args, format);
	vsnprintf(data,STR_MAX,format, args);

	if ((sim->log_level==log_level_screen)||(sim->log_level==log_level_screen_and_disk))
	{
		text_to_html(sim,data_html, data,1000);

		//wchar_t wide[1000];
		//int i=mbstowcs(wide, data_html, 1000);
		//wprintf(L"%S",wide);
			printf("%s",data_html);
			fflush(stdout);

	}

	/*if ((sim->log_level==log_level_disk)||(sim->log_level==log_level_screen_and_disk))
	{
		join_path(2,temp,sim->path,"log.dat");
		out=fopen(temp,"a");
		if (out!=NULL)
		{
			fprintf(out,"%s",data);
			fclose(out);
		}//else
		//{
		//	wprintf(L"error: opening file %s\n",temp);
		//}
	}*/

	va_end(args);
}

void rainbow_print(struct simulation *sim,double start,double stop, const char *format, ...)
{
	char data[STR_MAX];
	char temp[PATH_MAX];
	va_list args;
	va_start(args, format);
	vsnprintf(data,STR_MAX,format, args);

	int r,g,b;
	int i;
	double dl=0.0;
	double lambda=start;
	int len=0;
	len=strlen(data);

	dl=(stop-start)/((double)50);

	for (i=0;i<len;i++)
	{
		wavelength_to_rgb(&r,&g,&b,lambda);
		if (lambda<stop)
		{
			textcolor_rgb(sim,r, g, b);
		}
		printf_log(sim,"%c",data[i]);
		lambda+=dl;
	}

	textcolor_rgb(sim,-1, -1, -1);
}
void waveprint(struct simulation *sim,char *in,double wavelength)
{
	int r;
	int g;
	int b;

	if ((sim->log_level==log_level_screen)||(sim->log_level==log_level_screen_and_disk))
	{
		if (sim->html==TRUE)
		{
			wavelength_to_rgb(&r,&g,&b,wavelength);
			printf_log(sim,"<font color=\"#%.2x%.2x%.2x\">",r,g,b);
		}else
		{
			wavelength_to_rgb(&r,&g,&b,wavelength);
			textcolor_rgb(sim,r, g, b);

		}
	}

	printf_log(sim,"%s",in);

	if ((sim->log_level==log_level_screen)||(sim->log_level==log_level_screen_and_disk))
	{
		if (sim->html==TRUE)
		{
			printf_log(sim,"</font>");
		}else
		{
			textcolor_rgb(sim,-1, -1, -1);
			//textcolor(sim,fg_reset);
		}

	}
}

void textcolor_rgb(struct simulation *sim, int r, int g, int b)
{
	if (sim->html==TRUE)
	{
		if ((r!=-1)||(g!=-1)||(b!=-1))
		{
			printf_log(sim,"<font style=\"color: rgb(%d,%d,%d)\">",r,g,b);
		}else
		{
			printf_log(sim,"</font>");
		}

	}else
	{
		if ((r!=-1)||(g!=-1)||(b!=-1))
		{
			printf_log(sim,"\x1b[38;2;%d;%d;%dm", r,g,b);
		}else
		{
			printf_log(sim,"\x1b[0m");
		}
	}
}

void textcolor(struct simulation *sim, int color)
{
if (sim->html==TRUE)
{
	if (color==fg_purple)
	{
		printf_log(sim,"<font color=\"purple\">");
	}else
	if (color==fg_blue)
	{
		printf_log(sim,"<font color=\"blue\">");
	}else
	if (color==fg_green)
	{
		printf_log(sim,"<font color=\"green\">");
	}else
	if (color==fg_yellow)
	{
		printf_log(sim,"<font color=\"yellow\">");
	}else
	if (color==fg_red)
	{
		printf_log(sim,"<font color=\"red\">");
	}else
	if (color==fg_wight)
	{
		printf_log(sim,"<font color=\"#ffffff\">");
	}else
	if (color==fg_reset)
	{
		printf_log(sim,"</font>");
	}

}else
{
	printf("\033[%dm", color);
	//printf "\x1b[38;2;100;17;149mTRUECOLOR\x1b[0m\n" last three are rgb values
}

}

int log_search_error(char *path)
{
	int ret=-1;
    FILE * fp;
    char * line = NULL;
    ssize_t read;
	size_t len = 0;
    fp = fopen(path, "r");

    if (fp == NULL)
	{
       return ret;
	}

    while ((read = getline(&line, &len, fp)) != -1)
	{
		if (strcmp_begin(line,"error:")==0)
		{
			ret=0;
			break;
		}

    }

    fclose(fp);

    if (line)
	{
        free(line);
	}
    return ret;
}
