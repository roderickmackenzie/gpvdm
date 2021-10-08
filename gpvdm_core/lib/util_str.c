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

/** @file util_str.c
	@brief Utility functions for string handeling.
*/



#include <enabled_libs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include "util.h"
#include "log.h"
#include <gpvdm_const.h>
#include <lang.h>
#include <math.h>
#include <stdarg.h>

	#include <fnmatch.h>

int is_numbered_file(char *in,char *root)
{
	if (strcmp_end(in,".inp")!=0)
	{
		return -1;
	}

	if (strcmp_begin(in,root)!=0)
	{
		return -1;
	}

return 0;
}

int is_number(char a)
{
switch(a)
{
	case '1':
		return TRUE;
	case '2':
		return TRUE;
	case '3':
		return TRUE;
	case '4':
		return TRUE;
	case '5':
		return TRUE;
	case '6':
		return TRUE;
	case '7':
		return TRUE;
	case '8':
		return TRUE;
	case '9':
		return TRUE;
	case '0':
		return TRUE;
	case 'e':
		return TRUE;
	case 'E':
		return TRUE;
	case '+':
		return TRUE;
	case '-':
		return TRUE;
	case '.':
		return TRUE;
	default:
		return FALSE;
}

}

int get_number_in_string(double *out, char* in, int n)
{
	int i=0;
	int len=strlen(in);
	int m=0;
	int c=0;
	int number=-1;
	for (i=0;i<len;i++)
	{
		if (i>0)
		{
			m=is_number(in[i-1]);
		}else
		{
			m=FALSE;
		}

		c=is_number(in[i]);

		if ((m==FALSE) && (c==TRUE))
		{
			number++;
			if (number==n)
			{
				sscanf(&(in[i]),"%le",out);
				return 0;
			}
		}


	}

return -1;
}

int replace_number_in_string(char *buf, char* in, double replace, int n)
{
	int i=0;
	int len=strlen(in);
	int m=0;
	int c=0;
	int number=-1;
	int pos=0;
	strcpy(buf,"");

	for (i=0;i<len;i++)
	{
		if (i>0)
		{
		m=is_number(in[i-1]);
		}else
		{
			m=FALSE;
		}

		c=is_number(in[i]);

		if ((m==FALSE) && (c==TRUE))
		{
			number++;
			if (number==n)
			{
				char temp[200];
				sprintf(temp,"%le ",replace);
				strcat(buf,temp);
				pos=strlen(buf);
			}
		}

		if (number!=n)
		{
			buf[pos]=in[i];
			pos++;
			buf[pos]=0;

		}

	}

}

int fnmatch2(char *pat,char *in)
{

	return fnmatch(pat,in, FNM_PATHNAME);

}

void string_to_hex(char* out,char* in)
{
int i;
char temp[8];
strcpy(out,"");

for (i=0;i<strlen(in);i++)
{
	sprintf(temp,"%02x",in[i]);
	strcat(out,temp);
}

}


int cmpstr_min(char * in1,char *in2)
{
int i;
int max=strlen(in1);
if (strlen(in2)<max) max=strlen(in2);
for (i=0;i<max;i++)
{
	if (in1[i]!=in2[i]) return 1;
}
return 0;
}

int strextract_name(char *out,char * in)
{
int i;
for (i=0;i<strlen(in);i++)
{
	if (in[i]=='@')
	{
		out[i]=0;
		return strlen(out);
	}
	out[i]=in[i];

}
strcpy(out,"");
return -1;
}

int strcmp_end(char * str,char *end)
{
if (strlen(str)<strlen(end)) return 1;
int pos=strlen(str)-strlen(end);
return strcmp((char *)(str+pos),end);
}

int strcmp_begin(char * str,char *begin)
{
return strcmp_begin_safe(str,strlen(str),begin,strlen(begin));
}

int strcmp_begin_safe(char * str,int str_len,char *begin,int begin_len)
{
	int i;

	if (str_len<begin_len) return 1;

	for (i=0;i<begin_len;i++)
	{
		if (str[i]!=begin[i]) return 1;
	}

	return 0;
}

char* strextract_domain(char * in)
{
int i=0;
for (i=0;i<strlen(in)-1;i++)
{
	if (in[i]=='@')
	{
		return (char *)(&in[i+1]);
	}
}
return NULL;
}

int is_domain(char * in)
{
int i=0;
for (i=0;i<strlen(in)-1;i++)
{
	if (in[i]=='@')
	{
		return 0;
	}
}


return -1;
}

int extract_str_number(char * in,char *cut)
{
int out;
int len=strlen(cut);
sscanf((in+len),"%d",&out);
return out;
}

int strextract_int(char * in)
{
char temp[200];
int i=0;
int ret=0.0;
int count=0;
for (i=0;i<strlen(in);i++)
{
	if ((in[i]>47)&&(in[i]<58))
	{
		temp[count]=in[i];
		count++;
	}

}
temp[count]=0;
sscanf(temp,"%d",&ret);
return ret;
}

int str_isnumber(char *input)
{
    int start = 0;
	int len=strlen(input);
	int stop= len-1;
	if (len==0)
	{
		return FALSE;
	}

	//sort spaces
	while(input[start] == ' ')
	{
		start++;
		if (start>=len)
		{
			return FALSE;
		}
	}

	while(input[stop] == ' ')
	{
        stop--;
		if (stop<=0)
		{
			printf("b\n");
			return FALSE;
		}
	}


    // len==1 and first character not digit
    if(len == 1 && !(input[start] >= '0' && input[stop] <= '9'))
	{
		return FALSE;
	}

    // 1st char must be +, -, . or number
    if( input[start] != '+' && input[start] != '-' && !(input[start] >= '0' && input[start] <= '9'))
	{
		return FALSE;
	}

    int dot_or_e = FALSE;
    int seen_e = FALSE;
	int i=start;

    for(i ; i <= stop ; i++)
    {
        // Only allow numbers, +, - and e
        if(input[i] != 'e' && input[i] != 'E' && input[i] != '.' &&   input[i] != '+' && input[i] != '-' &&  !(input[i] >= '0' && input[i] <= '9'))
		{
			return FALSE;
		}

        if(input[i] == '.')
        {
            // a . as a last character is not allowed
            if(i == len-1)
			{
				return FALSE;
			}

            // have we seen a dot or e before
            if(dot_or_e == TRUE)
			{
                return FALSE;
			}

            // If we have a . we need a number after it
            if(!(input[i+1] >= '0' && input[i+1] <= '9'))
			{
				return FALSE;
			}

		}else
		if ((input[i] == 'e') || (input[i] == 'E'))
        {
            if (seen_e==TRUE)
			{
				return FALSE;
			}

            dot_or_e = TRUE;
			seen_e=TRUE;

            // e as the last character is also not allowed
            if(i == len-1)
			{
				return FALSE;
			}

            // an e first is not allowed we need a number before it
            if(!(input[i-1] >= '0' && input[i-1] <= '9'))
			{
				return FALSE;
			}

            // e must be followed by a + - or a number
            if (input[i+1] != '+' && input[i+1] != '-' && (input[i+1] >= '0' && input[i] <= '9'))
			{
				return FALSE;
			}
        }
    }


	return TRUE;
}

void split_dot(char *out, char *in)
{
	int i=0;
	strcpy(out,in);
	for (i=0;i<strlen(out);i++)
	{
		if (out[i]=='.')
		{
			out[i]=0;
			break;
		}
	}
}


int get_line(char *out,char *data,int len,int *pos)
{
	out[0]=0;
	//printf("%s\n",data);
	int i=0;
	if (*pos>=len)
	{
		return -1;
	}

		//printf("pos = %d\n",*pos);
		//getchar();
	while(*pos<len)
	{
		if ((data[*pos]=='\n')||(data[*pos]=='\r')||(data[*pos]==0))
		{
			out[i]=0;

			if (data[*pos]=='\r')
			{
				(*pos)++;
			}

			if (*pos<len)
			{
				if (data[*pos]=='\n')
				{
					(*pos)++;
				}
			}
			break;
		}

		out[i]=data[*pos];
		out[i+1]=0;
		i++;
		(*pos)++;

	}

return 0;
}

void str_split(char *in_string, ...)
{

	va_list arguments;
	int i;
	va_start ( arguments, in_string );
	char *ret=va_arg ( arguments, char * );
	strcpy(ret,"");
	int pos=0;
	for (i=0;i<strlen(in_string);i++)
	{
		ret[pos]=in_string[i];
		ret[pos+1]=0;


		if (in_string[i]==',')
		{
			ret[pos]=0;
			ret=va_arg ( arguments, char * );
			pos=0;
			ret[pos]=0;
		}else
		{
			pos++;
		}
		//printf("%s\n",ret);
	}

	va_end ( arguments );                  // Cleans up the list

	return;
}

void str_strip(char *in_string)
{
	int i;
	for (i=strlen(in_string)-1;i>=0;i--)
	{
		if (in_string[i]==13)
		{
			in_string[i]=0;
		}else
		if (in_string[i]==10)
		{
			in_string[i]=0;
		}else
		{
			break;
		}
	}

}
