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


/** @file list.c
	@brief Algorithms to make and deal with lists.
*/

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "vec.h"
#include "vectors.h"
#include <util.h>
#include <memory.h>

static int unused __attribute__((unused));

void vectors_init(struct vectors* in)
{
	in->len=0;
	in->max_len=0;
	in->data=NULL;

}

void vectors_malloc(struct vectors* in)
{
	in->max_len=10;
	malloc_1d((void *) (&in->data),in->max_len, sizeof(struct vec));
}

void vectors_append_no_rep(struct vectors* in,struct vec *test)
{
	int i;
	for (i=0;i<in->len;i++)
	{
		if (vec_cmp(&(in->data[i]),test)==0)
		{
			return;
		}
	}
	vectors_append(in, test);
}

void vectors_pop(struct vectors* in,int index)
{
	struct vec* temp;
	int len_to_move;

	if (index!=in->len-1)
	{
		len_to_move=in->len-index-1;
		if (len_to_move!=0)
		{
			temp=malloc(sizeof(struct vec)*len_to_move);
			memcpy(temp, &(in->data[index+1]), len_to_move * sizeof(struct vec));
			//printf("move %d %d %d %d\n",index,index+1,in->len-index-1,in->len);
			memcpy(&(in->data[index]), temp, len_to_move * sizeof(struct vec));
			free(temp);
		}
	}

	in->len--;

}

void vectors_append(struct vectors* in,struct vec *in_vec)
{
	vec_cpy(&(in->data[in->len]),in_vec);

	in->len++;
	if (in->len==in->max_len)
	{
		in->max_len+=10;
		in->data = (struct vec *) realloc(in->data,in->max_len*sizeof(struct vec ));
	}
}


void vectors_free(struct vectors* in)
{
	free_1d((void **)&(in->data), sizeof(struct vec));
	in->len=0;
	in->max_len=0;
}

int vectors_cmp(struct vectors* in,struct vec *test)
{
	int i;
	for (i=0;i<in->len;i++)
	{
		if (vec_cmp(&(in->data[i]),test)==0)
		{
			return i;
		}
	}
	return -1;
}


void vectors_minmax_y(struct vectors* in)
{
	int i;
	double min=in->data[0].y;
	double max=in->data[0].y;
	for (i=0;i<in->len;i++)
	{
		if (in->data[i].y<min) min=in->data[i].y;
		if (in->data[i].y>max) max=in->data[i].y;
	}

	in->min_y=min;
	in->max_y=max;
}

double vectors_min_value(struct vectors* in)
{
	int i;
	double min=in->data[0].y;

	for (i=0;i<in->len;i++)
	{
		if (in->data[i].y<min) min=in->data[i].y;
		if (in->data[i].x<min) min=in->data[i].x;
		if (in->data[i].z<min) min=in->data[i].z;

	}

	return min;

}

void vectors_remove_bump_down(struct vectors* in,int start)
{
	int i;
	double ly=0.0;
	double cy=0.0;
	double ry=0.0;

	for (i=start;i<in->len;i++)
	{

	if (i==start)
	{
		ly=in->data[i].y;
	}else
	{
		ly=in->data[i-1].y;
	}

	if (i==in->len-1)
	{
		ry=in->data[i].y;
	}else
	{
		ry=in->data[i+1].y;
	}
	cy=in->data[i].y;
	if ((ly>cy)&&(ry>cy))
	{
		in->data[i].y=(ly+cy+ry)/3.0;
	}

	}

}

void vectors_remove_bump_up(struct vectors* in,int start)
{
	int i;
	double ly=0.0;
	double cy=0.0;
	double ry=0.0;

	for (i=start;i<in->len;i++)
	{

	if (i==start)
	{
		ly=in->data[i].y;
	}else
	{
		ly=in->data[i-1].y;
	}

	if (i==in->len-1)
	{
		ry=in->data[i].y;
	}else
	{
		ry=in->data[i+1].y;
	}
	cy=in->data[i].y;
	if ((ly<cy)&&(ry<cy))
	{
		in->data[i].y=(ly+cy+ry)/3.0;
	}

	}

}

void vectors_dump(struct vectors* in)
{
	int i;
	for (i=0;i<in->len;i++)
	{
		printf("%lf %lf %lf\n",in->data[i].x,in->data[i].y,in->data[i].z);
	}
}

void vectors_save(struct simulation *sim,char *file_name,struct vectors* in)
{
	int i;
	FILE *file;
	file=fopen(file_name,"w");
	fprintf(file,"#%d\n",in->len);
	for (i=0;i<in->len;i++)
	{
		fprintf(file,"%lf %lf %lf\n",in->data[i].x,in->data[i].y,in->data[i].z);
	}
	fclose(file);
}

void vectors_cog_cal(struct vectors* in)
{
	int i;

	vec_set(&in->cog,0.0,0.0,0.0);
	for (i=0;i<in->len;i++)
	{
		vec_add(&in->cog,&(in->data[i]));
	}
	vec_div(&in->cog,(double)in->len);

}



void vectors_load(struct simulation *sim,struct vectors* in,char *file_name)
{
int i;
double a,b,c;
int length;
char temp[100];
struct vec temp_vec;
FILE *file;
if ((file=fopen(file_name,"r"))==NULL)
{
	ewe(sim,"List file not found %s\n",file_name);
}

unused=fscanf(file,"%s",temp);
sscanf((temp+1),"%d",&(length));
for (i=0;i<length;i++)
{
	unused=fscanf(file,"%lf %lf %lf",&(a),&(b),&(c));
	temp_vec.x=a;
	temp_vec.y=b;
	temp_vec.z=c;
	vectors_append(in, &temp_vec);
}

fclose(file);
}
