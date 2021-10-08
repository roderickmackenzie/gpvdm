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

#include <stdio.h>
#include <ray.h>
#include <gpvdm_const.h>
#include <math.h>
#include <stdlib.h>
#include <cal_path.h>
#include <log.h>
#include <dat_file.h>
#include <string.h>
#include <util.h>


/** @file triangle.c
	@brief Basic low level triangle functions
*/

void triangle_init(struct triangle *tri)
{
	tri->flag=TRUE;
	tri->deleted=FALSE;
	vec_init(&tri->cog);
}

long double get_sign (struct vec *p0, struct vec *p1, struct vec *p2)
{
    return (p0->x - p2->x) * (p1->z - p2->z) - (p1->x - p2->x) * (p0->z - p2->z);
}

int triangle_vec_within (struct triangle *tri,struct vec *pt)
{
    long double d0, d1, d2;
    int is_neg;
	int is_pos;

    d0 = get_sign(pt, &(tri->xy0), &(tri->xy1));
    d1 = get_sign(pt, &(tri->xy1), &(tri->xy2));
    d2 = get_sign(pt, &(tri->xy2), &(tri->xy0));

    is_neg = (d0 < 0) || (d1 < 0) || (d2 < 0);
    is_pos = (d0 > 0) || (d1 > 0) || (d2 > 0);

    return !(is_neg && is_pos);
}

void triangle_print(struct triangle *in)
{
	printf("(%le,%le,%le)\n",in->xy0.x,in->xy0.y,in->xy0.z);
	printf("(%le,%le,%le)\n",in->xy1.x,in->xy1.y,in->xy1.z);
	printf("(%le,%le,%le)\n",in->xy2.x,in->xy2.y,in->xy2.z);
	printf("\n");

}


void triangle_cal_cog(struct triangle *in)			//find the center of gravity
{
	in->cog.x=(in->xy0.x+in->xy1.x+in->xy2.x)/3.0;
	in->cog.y=(in->xy0.y+in->xy1.y+in->xy2.y)/3.0;
	in->cog.z=(in->xy0.z+in->xy1.z+in->xy2.z)/3.0;

}

void triangle_norm(struct vec *ret,struct triangle *my_obj)
{
	double mag=0.0;
	struct vec edge0;
	struct vec edge1;
	struct vec n;

	vec_init(&edge0);
	vec_init(&edge1);
	vec_init(&n);


	vec_cpy(&edge0,&(my_obj->xy1));
	vec_sub(&edge0,&(my_obj->xy0));

	vec_cpy(&edge1,&(my_obj->xy2));
	vec_sub(&edge1,&(my_obj->xy0));

	vec_cross(&n,&edge0,&edge1);
	mag=vec_fabs(&n);
	vec_div(&n,mag);

	vec_cpy(ret,&n);
}

double triangle_get_y_from_xz(struct triangle *tri,double x, double z)
{
	double y=0.0;
	struct vec edge0;
	struct vec edge1;
	struct vec n;

	vec_init(&edge0);
	vec_init(&edge1);
	vec_init(&n);

	vec_cpy(&edge0,&(tri->xy1));
	vec_sub(&edge0,&(tri->xy0));

	vec_cpy(&edge1,&(tri->xy2));
	vec_sub(&edge1,&(tri->xy0));

	vec_cross(&n,&edge0,&edge1);

	y=(n.x*(x-tri->xy0.x)+n.z*(z-tri->xy0.z))/(-n.y)+tri->xy0.y;
	return y;
}

void triangle_dump(char *file_name,struct triangle *tri)
{
	FILE *out;
	out=fopen(file_name,"w");
	fprintf(out,"%le %le %le\n",tri->xy0.z,tri->xy0.x,tri->xy0.y);
	fprintf(out,"%le %le %le\n",tri->xy1.z,tri->xy1.x,tri->xy1.y);
	fprintf(out,"%le %le %le\n",tri->xy2.z,tri->xy2.x,tri->xy2.y);
	fprintf(out,"%le %le %le\n",tri->xy0.z,tri->xy0.x,tri->xy0.y);
	fclose(out);

}


double triangle_get_min_y(struct triangle* tri)
{
	double min=tri->xy0.y;
	if (min>tri->xy1.y)
	{
		min=tri->xy1.y;
	}

	if (min>tri->xy2.y)
	{
		min=tri->xy2.y;
	}

	return min;
}

//Check if a triangle contains a line described by two vectors
int triangle_contains_zx_line(struct triangle* tri,struct vec *vec0,struct vec *vec1)
{
	int match0=-1;
	int match1=-1;

	if ((tri->xy0.z==vec0->z)&&(tri->xy0.x==vec0->x)) match0=0;
	if ((tri->xy1.z==vec0->z)&&(tri->xy1.x==vec0->x)) match0=1;
	if ((tri->xy2.z==vec0->z)&&(tri->xy2.x==vec0->x)) match0=2;

	if ((tri->xy0.z==vec1->z)&&(tri->xy0.x==vec1->x)) match1=0;
	if ((tri->xy1.z==vec1->z)&&(tri->xy1.x==vec1->x)) match1=1;
	if ((tri->xy2.z==vec1->z)&&(tri->xy2.x==vec1->x)) match1=2;

	if (match0!=-1)
	{
		if (match1!=-1)
		{
			return 0;
		} 
	}

	return -1;
}

//Findes the node of a triangle which is the furthest from a given point
struct vec * triangle_get_furthest_point(struct triangle* tri,struct vec *vec0)
{
	struct vec *ret=NULL;
	double max_dist=0.0;
	double dist=0.0;

	dist=vec_dist_zx(&(tri->xy0),vec0);
	if (dist>max_dist)
	{
		ret=&(tri->xy0);
		max_dist=dist;
	}

	dist=vec_dist_zx(&(tri->xy1),vec0);
	if (dist>max_dist)
	{
		ret=&(tri->xy1);
		max_dist=dist;
	}

	dist=vec_dist_zx(&(tri->xy2),vec0);
	if (dist>max_dist)
	{
		ret=&(tri->xy2);
		max_dist=dist;
	}

	return ret;
}

int triangle_contains_vec(struct triangle* tri,struct vec *vec0)
{

	if (vec_cmp_exact(&(tri->xy0),vec0)==0)	return 0;
	if (vec_cmp_exact(&(tri->xy1),vec0)==0)	return 0;
	if (vec_cmp_exact(&(tri->xy2),vec0)==0)	return 0;

	return -1;
}

int triangle_cmp(struct triangle* tri0,struct triangle* tri1)
{

	if (vec_cmp(&(tri0->xy0),&(tri1->xy0))==0)
	{
		if (vec_cmp(&(tri0->xy1),&(tri1->xy1))==0)
		{
			if (vec_cmp(&(tri0->xy2),&(tri1->xy2))==0)
			{
				return 0;
			}
		}
	}
	return -1;
}

double triangle_get_max_y(struct triangle* tri)
{
	double min=tri->xy0.y;
	if (min<tri->xy1.y)
	{
		min=tri->xy1.y;
	}

	if (min<tri->xy2.y)
	{
		min=tri->xy2.y;
	}

	return min;
}

void ray_tri_flip_y_axis(struct triangle* tri,double y)
{
	vec_flip_y_axis(&(tri->xy0),y);
	vec_flip_y_axis(&(tri->xy1),y);
	vec_flip_y_axis(&(tri->xy2),y);
}

void triangle_cal_edges(struct triangle* tri)
{
	//edge1 = vertex1 - vertex0;
	vec_cpy(&(tri->edge1),&(tri->xy1));
	vec_sub(&(tri->edge1),&(tri->xy0));

	//edge2 = vertex2 - vertex0;
	vec_cpy(&(tri->edge2),&(tri->xy2));
	vec_sub(&(tri->edge2),&(tri->xy0));
}

void triangle_cal_angles(struct vec *angles,struct triangle* tri)
{
	struct vec edge0;
	struct vec edge1;
	struct vec edge2;

	vec_cpy(&(edge0),&(tri->xy0));
	vec_sub(&(edge0),&(tri->xy1));

	vec_cpy(&(edge1),&(tri->xy1));
	vec_sub(&(edge1),&(tri->xy2));

	vec_cpy(&(edge2),&(tri->xy2));
	vec_sub(&(edge2),&(tri->xy0));

	angles->x=vec_ang(&edge1,&edge0)*360.0/(2.0*3.14159);
	if (angles->x>91)
	{
		angles->x-=90;
	}

	angles->y=vec_ang(&edge1,&edge2)*360.0/(2.0*3.14159);
	if (angles->y>91)
	{
		angles->y-=90;
	}

	angles->z=vec_ang(&edge2,&edge0)*360.0/(2.0*3.14159);
	if (angles->z>91)
	{
		angles->z-=90;
	}

}

double triangle_cal_area(struct triangle* tri)
{
	double ret=0.0;
	struct vec temp;
	vec_cross(&temp,&(tri->edge1),&(tri->edge2));
	ret=vec_fabs(&temp)/2.0;
	return ret;
}

double triangle_cal_area_zx(struct triangle* tri)
{
	double ret=0.0;
	struct vec temp;
	struct vec edge0;
	struct vec edge1;

	vec_cpy(&edge0,&(tri->edge1));
	vec_cpy(&edge1,&(tri->edge2));

	edge0.y=0.0;
	edge1.y=0.0;

	vec_cross(&temp,&(edge0),&(edge1));
	ret=vec_fabs(&temp)/2.0;
	return ret;
}

void triangle_cpy(struct triangle* tri0,struct triangle* tri1)
{
	memcpy(tri0, tri1, sizeof(struct triangle));
}

void ray_tri_sub_y(struct triangle* tri,double y)
{
	struct vec a;
	vec_set(&a,0,y,0);
	vec_sub(&(tri->xy0),&a);
	vec_sub(&(tri->xy1),&a);
	vec_sub(&(tri->xy2),&a);

}

void ray_tri_add_y(struct triangle* tri,double y)
{
	struct vec a;
	vec_set(&a,0,y,0);
	vec_add(&(tri->xy0),&a);
	vec_add(&(tri->xy1),&a);
	vec_add(&(tri->xy2),&a);

}
