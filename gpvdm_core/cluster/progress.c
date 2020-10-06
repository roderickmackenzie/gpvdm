/** @file progress.c
	@brief Progress bar functions.
*/
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#include <string.h>
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "util.h"

#define fg_reset	0
#define fg_red		31
#define fg_green	32
#define fg_yellow	33
#define fg_blue		34
#define fg_purple	35


static char progress_text[50];
static char multi_line_text[500];
static int text_color=fg_reset;
static int progress_spin=TRUE;
static int progress_colored=TRUE;
static int progress_noreset=TRUE;
static int progress_max=40;
static int progress_multi_line=FALSE;


void textcolor(int color)
{	char command[13];

	/* Command is the control command to the terminal */
	sprintf(command, "\e[%dm", color);
	printf("%s", command);
}

void set_porgress_color(int in)
{
text_color=in;
}

void set_progress_colored()
{
progress_colored=TRUE;
}

void set_porgress_nospin()
{
progress_spin=FALSE;
}

void set_porgress_noreset()
{
progress_noreset=TRUE;
}

void set_porgress_max(int in)
{
progress_max=in;
}

void text_progress(double percent)
{
	if (percent>=0)
	{
	char text[20];
	char message[100];

	int max=progress_max;
	int i=0;
	strcpy(message,"");
	for (i=0;i<(max-strlen(progress_text))/2;i++)
	{
		strcat(message," ");
	}
	strcat(message,progress_text);

	for (i=0;i<(max-strlen(progress_text))/2;i++)
	{
		strcat(message," ");
	}

	if (progress_multi_line==TRUE)
	{
		fputs("\e[1A\r", stdout);
		printf("%s\n",multi_line_text);
	}
	if (progress_noreset==FALSE) fputs("\r", stdout);


	fputs("[", stdout);
	int filled_in=max*percent;
	for (i=0;i<filled_in;i++)
	{
	textcolor(text_color);
	if (progress_colored==TRUE)
	{
	textcolor(fg_green);
	if (i>(max/4)) textcolor(fg_blue);
	if (i>(max/4)*2) textcolor(fg_yellow);
	if (i>(max/4)*3) textcolor(fg_red);

	}
		fputs("=", stdout);

	}

	if ((filled_in<(max-1))&&(progress_spin==TRUE))
	{
	static int pos=0;
	if (pos==0) fputs("-", stdout);
	if (pos==1) fputs("\\", stdout);
	if (pos==2) fputs("|", stdout);
	if (pos==3) fputs("/", stdout);
	pos++;
	if (pos==4) pos=0;
	filled_in++;
	}
	textcolor(fg_reset);

	for (i=filled_in;i<max;i++)
	{
	if (i<strlen(message))
	{
		fputc((char)message[i], stdout);
	}else
	{
		fputs(" ", stdout);
	}
	}

	fputs("]", stdout);
	sprintf(text," %d%% ",(int)(percent*100.0));
	fputs(text, stdout);
	fflush(stdout);
	}else
	{
	static int pos=0;
	if (pos==0) fputs("-", stdout);
	if (pos==1) fputs("\\", stdout);
	if (pos==2) fputs("|", stdout);
	if (pos==3) fputs("/", stdout);
	fputs("\r", stdout);
	fflush(stdout);
	pos++;
	if (pos==4) pos=0; 
	}
}

void progress_clear(int n)
{
fputs("\e[2A\r", stdout);
int i;
for (i=0;i<progress_max+7;i++)
{
	fputs(" ", stdout);
}
printf("\n");
for (i=0;i<progress_max+7;i++)
{
	fputs(" ", stdout);
}
fputs("\e[1A\r", stdout);
}

void text_progress_finish()
{
set_progress_multi_line_text("");
text_progress(1.0);
printf("\n");
}

void set_progress_multi_line_text(char *in)
{
if (strlen(in)>progress_max)
{
strcpy(multi_line_text,(in+(strlen(in)-progress_max)));
return;
}

strcpy(multi_line_text,in);
int i;
for (i=strlen(in);i<progress_max;i++)
{
	strcat(multi_line_text," ");
}
}

void set_progress_multi_line()
{
printf("\n");
progress_multi_line=TRUE;
}

void text_progress_start(char *in)
{
printf("%s\n",in);
progress_spin=TRUE;
progress_colored=FALSE;
progress_noreset=FALSE;
progress_multi_line=FALSE;
set_porgress_max(40);
strcpy(progress_text,"");
}
