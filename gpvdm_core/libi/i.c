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




/** @file i.c
	@brief Simple functions to read in scientific data from text files and perform simple maths on the data.
*/
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#include <enabled_libs.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sim_struct.h>

#include "i.h"
#include "util.h"
#include "cal_path.h"
#include "gpvdm_const.h"
#include <log.h>
#include "inp.h"
#include <memory.h>


static int unused __attribute__((unused));
static char* unused_pchar __attribute__((unused));

void math_xy_sin(struct math_xy *in,long double mag,long double fx,long double delta)
{
int i;
	for (i=0;i<in->len;i++)
	{
		in->data[i]=fabs(mag)*sin(2*M_PI*fx*(in->x[i]+delta));
	}
}

void math_xy_cos(struct math_xy *in,long double mag,long double fx,long double phi)
{
int i;
	for (i=0;i<in->len;i++)
	{
		in->data[i]=fabs(mag)*cos(2*M_PI*fx*(in->x[i])+phi);
	}
}

void inter_add_to_hist(struct math_xy* in,long double pos,long double value)
{
int ii=0;
long double min=in->x[0];
long double max=in->x[in->len-1];
long double dx=(max-min)/((long double)in->len);

ii=(int)((pos-min)/dx);

if (ii<in->len)
{
	if (ii>=0)
	{
		in->data[ii]+=value;
	}
}

}



int inter_sort_compare(const void *a, const void *b)
{
long double aa = *(long double*)a;
long double bb = *(long double*)b;

if (aa < bb) return -1;
if (aa > bb) return  1;

return 0;
}

/**Do a quick search
@param in input structure
*/
int inter_sort(struct math_xy* in)
{
if (in->len==0) return -1;

int i=0;
long double *data=(long double *)malloc(in->len*2*sizeof(long double));

for (i=0;i<in->len;i++)
{
	data[i*2]=in->x[i];
	data[(i*2)+1]=in->data[i];
}

qsort(data, in->len, sizeof(long double)*2, inter_sort_compare);

for (i=0;i<in->len;i++)
{
	in->x[i]=data[i*2];
	in->data[i]=data[(i*2)+1];
}

free(data);
return 0;
}


/**Translate the input math_xy to a log struct
@param in inout math_xy
@param out output math_xy
*/
void inter_to_log_mesh(struct math_xy* out,struct math_xy* in)
{
long double a=log10(in->x[0]);
long double b=log10(in->x[in->len-1]);
long double step=(b-a)/((long double)out->len);
int i;
long double pos=a;
for (i=0;i<out->len;i++)
{
out->x[i]=pow(10.0,pos);
out->data[i]=inter_get(in,pow(10.0,pos));
pos+=step;

}

}

/**Use linear interpolation to project an math_xy array to a new linear mesh
@param in input math_xy
@param out output math_xy
*/
void inter_to_new_mesh(struct math_xy* in,struct math_xy* out)
{
int i;
int ii;
long double pos=in->x[0];
long double delta=(in->x[in->len-1]-in->x[0])/(long double)out->len;
pos+=delta/2.0;
for (i=0;i<out->len;i++)
{
	ii=search(in->x,in->len,pos);

	long double x0=in->x[ii];
	long double x1=in->x[ii+1];

	long double y0=in->data[ii];
	long double y1=in->data[ii+1];

	out->x[i]=pos;
	out->data[i]=y0+((y1-y0)/(x1-x0))*(pos-x0);

	pos+=delta;
}

return;
}


/**Sum a 1D math_xy whilst taking the modulus of the data.
@param in input math_xy
*/
long double inter_sum_mod(struct math_xy* in)
{
int i;
long double sum=0.0;

for (i=0;i<in->len;i++)
{
	sum+=fabs(in->data[i]);
}
return sum;
}

/**Get the average value of the data in a 1D math_xy between two points
@param in input math_xy
@param start start point
@param stop stop point

*/
long double inter_avg_range(struct math_xy* in,long double start,long double stop)
{
int i;
long double sum=0.0;
long double points=0.0;
for (i=0;i<in->len;i++)
{
	if ((in->x[i]>start)&&(in->x[i]<stop))
	{
		sum+=in->data[i];
		points+=1.0;
	}
}
return sum/points;
}

/**Sum a 1D math_xy (no modulus)
@param in input math_xy
*/
long double inter_sum(struct math_xy* in)
{
int i;
long double sum=0.0;

for (i=0;i<in->len;i++)
{
	sum+=in->data[i];
}
return sum;
}

/**Convolve two math_xys
@param one input/output math_xy
@param two input math_xy
*/
void inter_convolve(struct math_xy* one,struct math_xy* two)
{
int i;
//long double sum=0.0;

for (i=0;i<one->len;i++)
{
	one->data[i]*=two->data[i];
}
}



long double inter_norm_to_one_range(struct math_xy* in,long double start,long double stop)
{
int i;
long double max=0.0;

for (i=0;i<in->len;i++)
{
	if (in->x[i]>start)
	{
		max=in->data[i];
		break;
	}
}

for (i=0;i<in->len;i++)
{
	if ((in->x[i]>start)&&(in->x[i]<stop))
	{
		if (in->data[i]>max) max=in->data[i];
	}
}

for (i=0;i<in->len;i++)
{
in->data[i]/=max;
}

return max;
}

/**Get maximum value of an math_xy
@param in input math_xy
*/
long double math_xy_get_max(struct math_xy* in)
{
long double max=0.0;

max=inter_get_max_range(in,0,in->len);

return max;
}

void math_xy_get_max_and_pos(struct math_xy* in,long double *max, long double *x)
{
	int i;
	*max=in->data[0];

	for (i=0;i<in->len;i++)
	{
		if (in->data[i]>*max)
		{
			*max=in->data[i];
			*x=in->x[i];
		}
	}
}

long double inter_get_max_range(struct math_xy* in,int start, int stop)
{
int i;
long double max=0.0;
if (start<in->len)
{
	max=in->data[start];
}

//if (in->len>0) max=in->data[0];
for (i=start;i<stop;i++)
{
	if (in->data[i]>max) max=in->data[i];
}

return max;
}

int inter_get_max_pos(struct math_xy* in)
{
int i;
int pos=0;
long double max=in->data[0];
//if (in->len>0) max=in->data[0];
for (i=0;i<in->len;i++)
{
	if (in->data[i]>max)
	{
		max=in->data[i];
		pos=i;
	}
}

return pos;
}

/**Perform log10 on data in math_xy and keep the sign
@param in input math_xy
*/
void inter_log_y_m(struct math_xy* in)
{
int i;
long double mull=1.0;

for (i=0;i<in->len;i++)
{
	mull=1.0;
	if (in->data[i]<0.0) mull= -1.0;
	in->data[i]=log10(fabs(in->data[i]))*mull;
}
}

/**Perform log10 on data in math_xy
@param in input math_xy
*/
void inter_log_y(struct math_xy* in)
{
int i;
for (i=0;i<in->len;i++)
{
	in->data[i]=log10(fabs(in->data[i]));
}
}
/**Perform log10 on x axis in math_xy
@param in input math_xy
*/
void inter_log_x(struct math_xy* in)
{
int i;
for (i=0;i<in->len;i++)
{
	if (in->x[i]>0.0) in->x[i]=log10(sqrt(in->x[i]*in->x[i]));
}
}


/**Remove zeros from the data stored in math_xy
@param in input math_xy
*/
void inter_purge_zero(struct math_xy* in)
{
int i;
int write=0;
int read=0;
for (i=0;i<in->len;i++)
{
	in->data[write]=in->data[read];
	in->x[write]=in->x[read];
	if (in->data[read]==0.0)
	{
		write--;
	}
	read++;
	write++;
}
in->len=write;

inter_realloc(in,in->len);

}

void inter_purge_x_zero(struct math_xy* in)
{
int i;
int write=0;
int read=0;
for (i=0;i<in->len;i++)
{
	in->data[write]=in->data[read];
	in->x[write]=in->x[read];

	if (in->x[read]==0.0)
	{
		write--;
	}
	read++;
	write++;
}

in->len=write;

inter_realloc(in,in->len);

}

/**Get the smallest data stored in an math_xy array
@param in input math_xy
*/
long double inter_get_min(struct math_xy* in)
{
int i=0;
long double min=in->data[i];
for (i=0;i<in->len;i++)
{
	if (in->data[i]<min) min=in->data[i];


}
return min;
}

/**Get the position of the smallest data stored in an math_xy array
@param in input math_xy
*/
int inter_get_min_pos(struct math_xy* in)
{
int i=0;
int pos=0;
long double min=in->data[i];
for (i=0;i<in->len;i++)
{
	if (in->data[i]<min)
	{
		min=in->data[i];
		pos=i;
	}

}
return pos;
}

/**Get the smallest data stored in an math_xy array
@param in input math_xy
*/
long double inter_get_min_range(struct math_xy* in,long double min, long double max)
{
int i=0;
long double ret=in->data[i];
for (i=0;i<in->len;i++)
{
	if ((in->x[i]>min)&&(in->x[i]<max))
	{
		if (in->data[i]<ret) ret=in->data[i];
	}

}
return ret;
}

/**Chop an math_xy array between two points
@param min min point
@param min max point
*/
void inter_chop(struct math_xy* in,long double min, long double max)
{
int i;
int write=0;
int read=0;
for (i=0;i<in->len;i++)
{
	in->data[write]=in->data[read];
	in->x[write]=in->x[read];
	write++;
	if (in->x[read]<min)
	{
		write--;
	}

	if (in->x[read]>max) break;
	read++;




}
in->len=write;

inter_realloc(in,in->len);
}

/**Divide the data in an math_xy by a value
@param div value to divide the data by
*/
void inter_div_long_double(struct math_xy* in,long double div)
{
int i;
for  (i=0;i<in->len;i++)
{
	in->data[i]/=div;
}

}

/**Rescale the scale and the data
@param in The structure holding the data
@param xmul multiply x axis by this
@param ymul multiply y axis by this
*/

void inter_rescale(struct math_xy* in,long double xmul, long double ymul)
{
int i;
for  (i=0;i<in->len;i++)
{
	in->x[i]*=xmul;
	in->data[i]*=ymul;
}

}



/**Add a value from every x element in the array
@param value value to subtract from data
*/
void inter_add_x(struct math_xy* in,long double value)
{
int i;
for  (i=0;i<in->len;i++)
{
in->x[i]+=value;
}

}

/**Subtract a value from every data element in the array
@param value value to subtract from data
*/
void inter_sub_long_double(struct math_xy* in,long double value)
{
int i;
for  (i=0;i<in->len;i++)
{
in->data[i]-=value;
}

}



/**Add a number to an math_xy
@param in input math_xy
@param value value to add to math_xy
*/
void inter_add_long_double(struct math_xy* in,long double value)
{
int i;
for  (i=0;i<in->len;i++)
{
in->data[i]+=value;
}

}

/**Normalize the area under a 1D math_xy to one multiplied by a constant
@param in input math_xy
@param mul number to multiply the math_xy by
*/
void math_xy_norm_area(struct math_xy* in,long double mul)
{
int i;
long double tot=0.0;
long double dx=0.0;
for  (i=0;i<in->len;i++)
{
	if (i==0)
	{
		dx=in->x[1]-in->x[0];
	}else
	if (i==in->len-1)
	{
		dx=in->x[i]-in->x[in->len-2];
	}else
	{
		dx=(in->x[i+1]-in->x[i])/2.0+(in->x[i]-in->x[i-1])/2.0;
	}

	tot+=dx*in->data[i];
}

for  (i=0;i<in->len;i++)
{

	in->data[i]/=tot;
	in->data[i]*=mul;

}

}





int inter_get_col_n(struct simulation *sim,char *name)
{
int i=0;
char temp[10000];
char *token;
int col=0;

FILE *file;
file=fopen(name,"r");
if (file == NULL)
{
	printf_log(sim,"inter_get_col_n can not open file %s\n",name);
	exit(0);
}

do
{
	memset(temp,0,10000);
	unused_pchar=fgets(temp, 10000, file);
	const char s[2] = " ";
	for (i=0;i<strlen(temp);i++)
	{
		if (temp[i]=='\t') temp[i]=' ';
	}

	if ((temp[0]!='#')&&(temp[0]!='\n')&&(temp[0]!='\r')&&(temp[0]!=0))
	{
		col=0;
		token = strtok(temp, s);

		do
		{
			token = strtok(NULL, s);
			if (token==NULL) break;
			if (token[0]!='\n') col++;
		}
		while(token!=NULL);

		col--;
		break;

	}


}while(!feof(file));
fclose(file);
return col;
}

void inter_load_by_col(struct simulation *sim,struct math_xy* in,char *name,int col)
{
int i=0;
char temp[1000];
long double x;
long double y;
char *token;
int icol=0;
strcpy(in->name,name);

FILE *file;
file=fopen(name,"r");
if (file == NULL)
{
	printf_log(sim,"inter_load_a can not open file %s\n",name);
	exit(0);
}

inter_init(sim,in);
inter_malloc(in,100);

do
{
	memset(temp,0,1000);
	unused_pchar=fgets(temp, 1000, file);
	const char s[2] = " ";
	for (i=0;i<strlen(temp);i++)
	{
		if (temp[i]=='\t') temp[i]=' ';
	}

	if ((temp[0]!='#')&&(temp[0]!='\n')&&(temp[0]!='\r')&&(temp[0]!=0))
	{
		token = strtok(temp, s);

		sscanf(token,"%Le",&(x));
		if (token != NULL)
		{
			icol=0;
			int ret=0;
			while( token != NULL )
			{
				if (col==icol)
				{
					ret=sscanf(token,"%Le",&(y));
					break;
				}
				token = strtok(NULL, s);
				icol++;
			}

			if (ret==1) inter_append(in,x,y);
		}

	}


}while(!feof(file));
fclose(file);
}


void inter_import_array(struct math_xy* in,long double *x,long double *y,int len,int alloc)
{
	int i;

	if (alloc==TRUE)
	{
		inter_malloc(in,len);
	}
	in->len=len;

	for  (i=0;i<in->len;i++)
	{
		in->x[i]=x[i];
		in->data[i]=y[i];
	}


}

/**Take the derivative with respect to the x axis of an math_xy
@param in input math_xy
@param output math_xy
*/
void inter_deriv(struct math_xy* out,struct math_xy* in)
{
int i;
long double yl=0.0;
long double yr=0.0;
long double xl=0.0;
long double xr=0.0;
long double dy=0.0;
for (i=0;i<in->len;i++)
{
if (i==0)
{
	xl=in->x[i];
	yl=in->data[i];
}else
{
	xl=in->x[i-1];
	yl=in->data[i-1];
}

if (i==(in->len-1))
{
	xr=in->x[i];
	yr=in->data[i];
}else
{
	xr=in->x[i+1];
	yr=in->data[i+1];
}
if (yr!=yl)
{
	dy=(yr-yl)/(xr-xl);
}else
{
	dy=0.0;
}
out->x[i]=in->x[i];
out->data[i]=dy;
}

}

/**Swap x and data column
@param in math_xy to operate on
*/
void inter_swap(struct math_xy* in)
{
	int i;
	long double x_temp=0.0;
	long double data_temp=0.0;


	for  (i=0;i<in->len;i++)
	{
		data_temp=in->data[i];
		x_temp=in->x[i];

		in->data[i]=x_temp;
		in->x[i]=data_temp;
	}
}



int inter_search_token(struct simulation *sim,long double *value,char *token,char *name)
{
int found=FALSE;
char temp[1000];
char temp2[1000];
FILE *file;
file=fopen(name,"r");
if (file == NULL)
{
	printf_log(sim,"inter_load can not open file %s\n",name);
	return -1;
}

do
{
	temp[0]=0;
	gpvdm_fgets(temp, 1000, file);

	if (strcmp_begin(temp,token)==0)
	{
		sscanf(temp,"%s %Le",temp2,value);
		found=TRUE;
		break;
	}


}while(!feof(file));
fclose(file);

if (found==TRUE)
{
	return 0;
}

return -1;
}


void math_xy_set_value(struct math_xy* in,long double value)
{
int i=0;
for  (i=0;i<in->len;i++)
{
	in->data[i]=value;
}

}

/**Take segments of dx and multiply them by the y-axis.
@param in struct to work on
*/
void inter_y_mul_dx(struct math_xy* in)
{
int i=0;

long double dx=0.0;
long double d0=0.0;
long double d1=0.0;
for  (i=0;i<in->len;i++)
{
		if (i==0)
		{
			d0=(in->x[0]);
		}else
		{
			d0=(in->x[i-1]);
		}

		if (i==in->len-1)
		{
			d1=(in->x[i]);
		}else
		{
			d1=(in->x[i+1]);
		}

		dx=(d1-d0)/2.0;
		in->data[i]=in->data[i]*dx;
}

}

/**Make a cumulative graph.
@param in struct to work on
*/
void inter_make_cumulative(struct math_xy* in)
{
int i=0;
long double dx=0.0;
long double d0=0.0;
long double d1=0.0;
long double tot=0.0;
for  (i=0;i<in->len;i++)
{
	if (i==0)
	{
		d0=(in->x[0]);
	}else
	{
		d0=(in->x[i-1]);
	}

	if (i==in->len-1)
	{
		d1=(in->x[i]);
	}else
	{
		d1=(in->x[i+1]);
	}

	dx=(d1-d0)/2.0;
	tot+=in->data[i]*dx;
	in->data[i]=tot;
}

}


int inter_search_pos(struct math_xy* in,long double x)
{
return search(in->x,in->len,x);
}



/**Get interpolated data from a data set
@param in The structure holding the data
@param x the position of the data.
@return the interpolated data value
*/
long double inter_get(struct math_xy* in,long double x)
{
long double x0;
long double x1;
long double y0;
long double y1;

long double ret;
int i=0;

//if (x>in->x[in->len-1]) return 0.0;
if (x<in->x[0])
{

return 0.0;
}


if (x>=in->x[in->len-1])
{
	i=in->len-1;
	x0=in->x[i-1];
	x1=in->x[i];

	y0=in->data[i-1];
	y1=in->data[i];

}else
{
	i=search(in->x,in->len,x);

	x0=in->x[i];
	x1=in->x[i+1];

	y0=in->data[i];
	y1=in->data[i+1];

}
ret=y0+((y1-y0)/(x1-x0))*(x-x0);
return ret;
}

long double inter_get_hard(struct math_xy* in,long double x)
{
//long double x0;
//long double x1;
//long double y0;
//long double y1;

//long double ret;
//int i=0;

if (x>in->x[in->len-1])
{

return 0.0;
}

return inter_get(in,x);
}

long double inter_get_noend(struct math_xy* in,long double x)
{

long double x0;
long double x1;
long double y0;
long double y1;

long double ret;
int i=0;

if (x<in->x[0])
{
	return in->data[0];
}

if (x>=in->x[in->len-1])
{
	return in->data[in->len-1];
}


	i=search(in->x,in->len,x);
	x0=in->x[i];
	x1=in->x[i+1];

	y0=in->data[i];
	y1=in->data[i+1];

long double eval=0.0;

if ((y1-y0)==0.0)
{
	eval=0.0;
}else
if ((x-x0)==0.0)
{
	eval=0.0;
}else
{
	eval=((y1-y0)/(x1-x0))*(x-x0);
}

ret=y0+eval;
return ret;
}


void inter_reset(struct math_xy* in)
{
in->len=0;
in->max_len=0;
}


void inter_join_bins(struct math_xy* in,long double delta)
{
int i;
long double tot=0.0;
int pos=0;
long double bin=in->x[0];
int move_on=FALSE;
for (i=0;i<in->len;i++)
{
	move_on=FALSE;

	if (fabs(bin-in->x[i])<delta)
	{
		tot+=in->data[i];

	}else
	{
		move_on=TRUE;
	}

	if (i==in->len-1)
	{
		move_on=TRUE;
	}

	if (move_on==TRUE)
	{
		in->data[pos]=tot;
		in->x[pos]=bin;
		bin=in->x[i];
		tot=in->data[i];
		pos++;

	}

}
in->len=pos;
return;
}



