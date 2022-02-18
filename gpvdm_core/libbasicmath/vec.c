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


/** @file vec.c
	@brief Basic vector manipulation routines
*/

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#include <stdio.h>
#include <math.h>
#include <gpvdm_const.h>
#include "vec.h"


///Return |Vector|
double vec_mod(struct vec *my_vec)
{
return sqrt(pow(my_vec->x,2.0)+pow(my_vec->y,2.0)+pow(my_vec->z,2.0));
}

///Make all vector component signs positive
void vec_plus(struct vec *my_vec1)
{
if (my_vec1->x<0) my_vec1->x*= -1.0;
if (my_vec1->y<0) my_vec1->y*= -1.0;
if (my_vec1->z<0) my_vec1->z*= -1.0;
}


///Print a vector to a file
void vec_fprintf(FILE *out,struct vec *my_vec)
{
	fprintf(out,"%le %le %le\n",my_vec->x,my_vec->y,my_vec->z);
}

///Convert from rad to deg
double deg(double in)
{
	return in*360.0/(2.0*PIf);
}


///Subtract two vectors
void vec_sub(struct vec *my_vec1,struct vec *my_vec2)
{
my_vec1->x-=my_vec2->x;
my_vec1->y-=my_vec2->y;
my_vec1->z-=my_vec2->z;
}


///Add two vectors together
void vec_add(struct vec *my_vec1,struct vec *my_vec2)
{
my_vec1->x+=my_vec2->x;
my_vec1->y+=my_vec2->y;
my_vec1->z+=my_vec2->z;
}

///Divide one vector by a double
void vec_div(struct vec *my_vec1,double n)
{
my_vec1->x/=n;
my_vec1->y/=n;
my_vec1->z/=n;
}

void vec_div_vec(struct vec *my_vec1,struct vec *my_vec2)
{
my_vec1->x/=my_vec2->x;
my_vec1->y/=my_vec2->y;
my_vec1->z/=my_vec2->z;
}

int vec_within_box(struct vec *l,struct vec *h,struct vec *v)
{
	if ((v->x>=l->x)&&(v->x<=h->x))
	{
		if ((v->y>=l->y)&&(v->y<=h->y))
		{
			if ((v->z>=l->z)&&(v->z<=h->z))
			{
				return 0;
			}

		}

	}

return -1;
}

void vec_mul_vec(struct vec *my_vec1,struct vec *my_vec2)
{
my_vec1->x*=my_vec2->x;
my_vec1->y*=my_vec2->y;
my_vec1->z*=my_vec2->z;
}

void vec_mul(struct vec *my_vec1,double n)
{
my_vec1->x*=n;
my_vec1->y*=n;
my_vec1->z*=n;
}


double vec_fabs(struct vec *my_vec)
{
return sqrt(pow(my_vec->x,2.0)+pow(my_vec->y,2.0)+pow(my_vec->z,2.0));
}

void vec_rotate(struct vec *my_vec,double ang)
{
	double x=0.0;
	double y=0.0;

	x=my_vec->x*cos(ang)-my_vec->y*sin(ang);
	y=my_vec->x*sin(ang)+my_vec->y*cos(ang);
	my_vec->x=x;
	my_vec->y=y;
}

//Rotate arround the y-axis
void vec_rotate_y(struct vec *my_vec,double ang)
{
	double x=0.0;
	double y=0.0;
	double z=0.0;
	x = my_vec->x*cos(ang) + my_vec->z*sin(ang);
	y = my_vec->y;
	z = my_vec->z*cos(ang) - my_vec->x*sin(ang);

	my_vec->x=x;
	my_vec->y=y;
	my_vec->z=z;
}

//Rotate arround the x-axis
void vec_rotate_x(struct vec *my_vec,double ang)
{
	double x=0.0;
	double y=0.0;
	double z=0.0;
	x = my_vec->x;
	y = my_vec->y*cos(ang) - my_vec->z*sin(ang);
	z = my_vec->y*sin(ang) + my_vec->z*cos(ang);

	my_vec->x=x;
	my_vec->y=y;
	my_vec->z=z;
}

///Perform a dot product between two vectors
double vec_dot(struct vec *a,struct vec *b)
{
return (a->x*b->x)+(a->y*b->y)+(a->z*b->z);
}

void vec_init(struct vec *my_vec)
{
	my_vec->x=0.0;
	my_vec->y=0.0;
	my_vec->z=0.0;
}

///Perform the cross product of two vectors
void vec_cross(struct vec *ret,struct vec *a,struct vec *b)
{
double x=0.0;
double y=0.0;
double z=0.0;

x=(a->y*b->z)-(a->z*b->y);
y=(a->z*b->x)-(a->x*b->z);
z=(a->x*b->y)-(a->y*b->x);

ret->x=x;
ret->y=y;
ret->z=z;

}

void vec_swap(struct vec *my_vec)
{
double temp=0.0;
temp=my_vec->x;
my_vec->x=my_vec->y;
my_vec->y=temp;
}

double vec_dist(struct vec *a,struct vec *b)
{
	return sqrt(pow(a->x-b->x,2.0)+pow(a->y-b->y,2.0)+pow(a->z-b->z,2.0));
}

double vec_dist_zx(struct vec *a,struct vec *b)
{
	return sqrt(pow(a->x-b->x,2.0)+pow(a->z-b->z,2.0));
}

///Copy a vector
void vec_cpy(struct vec *my_vec1,struct vec *my_vec2)
{
my_vec1->x=my_vec2->x;
my_vec1->y=my_vec2->y;
my_vec1->z=my_vec2->z;
}

///Normalize a vector
void vec_norm(struct vec *my_vec)
{
	double mag=vec_fabs(my_vec);
	my_vec->x/=mag;
	my_vec->y/=mag;
	my_vec->z/=mag;
}

///Print a vector to stdout
void vec_print(struct vec *my_vec)
{
	printf("%lf %lf %lf\n",my_vec->x,my_vec->y,my_vec->z);
}

///Set a vector
void vec_set(struct vec *my_vec,double x, double y, double z)
{
my_vec->x=x;
my_vec->y=y;
my_vec->z=z;
}

double overlap(double x0,double x1)
{
	double a=x0;
	double b=x1;

	if (a>b)
	{
		a=x1;
		b=x0;
	}

	if ((a<=0.0)&&(b>=0.0))
	{
		return 0;
	}else
	if ((a<0.0)&&(b<0.0))
	{
		return fabs(b);
	}else
	if ((a>=0.0)&&(b>=0.0))
	{
		return fabs(a);
	}

	return -2.0;

}

double vec_overlap(struct vec *a,struct vec *b)
{
	return overlap(a->x,b->x)+overlap(a->y,b->y)+overlap(a->z,b->z);
}

double vec_overlap_test(struct vec *a,struct vec *b)
{

	struct vec test_a;
	struct vec test_b;

	vec_cpy(&test_a,a);
	vec_cpy(&test_b,b);

	//The normal will not be relative to the incident ray therefore
	//change the sign until it points in the same direction as the incident ray
	double a_res=vec_overlap(&test_a,&test_b);
	vec_mul(&test_a,-1.0);
	vec_mul(&test_b,-1.0);
	double b_res=vec_overlap(&test_a,&test_b);

	if (a_res>b_res)
	{
		return 1.0;
	}

	return -1.0;
}

/** @brief Basic vector manipulation routines
this function needs four points and will calculate the dihedral angle

 a            d
  \          /
   \        /
    b------c
*/
double vec_get_dihedral(struct vec *a,struct vec *b,struct vec *c,struct vec *d)
{
double cos_;
	struct vec b1;
	struct vec b2;
	struct vec b3;
	struct vec b23;
	struct vec b12;

	vec_cpy(&b1,b);
	vec_sub(&b1,a);

	vec_cpy(&b2,c);
	vec_sub(&b2,b);

	vec_cpy(&b3,d);
	vec_sub(&b3,c);

	vec_cross(&b23,&b2,&b3);
	vec_cross(&b12,&b1,&b2);

	double mb2=vec_mod(&b2);
	double aa=vec_dot(&b1,&b23);

	double cross=0.0;;
	cross=vec_dot(&b12,&b23);

	cos_=(atan2(mb2*aa,cross)/(3.1415026))*180.0;
	if (cos_<0) cos_=360-sqrt(pow(cos_,2.0));

return cos_;
}

/**Rotate a vector around an arbitrary axis
@arg in   Input vector
@arg unit A unit vector by around which the vector should be rotated
@arg base ??
@arg ang  Angle by which the rotation should be performed
*/

void vec_rot(struct vec *in,struct vec *unit,struct vec *base, double ang)
{
	struct vec temp;
	struct vec rot;

	double c;
	double s;

	vec_cpy(&rot,in);
	vec_sub(&rot,base);
	c=cos(ang);
	s=sin(ang);

	temp.x=rot.x*(unit->x*unit->x*(1.0-c)+c) + rot.y*(unit->x*unit->y*(1.0-c)-unit->z*s)	+ rot.z*(unit->x*unit->z*(1.0-c)+unit->y*s);
	temp.y=rot.x*(unit->x*unit->y*(1-c)+unit->z*s)	+ rot.y*(unit->y*unit->y*(1-c)+c)	+ rot.z*(unit->y*unit->z*(1.0-c)-unit->x*s);
	temp.z=rot.x*(unit->x*unit->z*(1.0-c)-unit->y*s) + rot.y*(unit->y*unit->z*(1.0-c)+unit->x*s) + rot.z*(unit->z*unit->z*(1.0-c)+c);

	vec_add(&temp,base);
	vec_cpy(in,&temp);
}

///Add to a vector
void vec_add_values(struct vec *my_vec, double x,double y, double z)
{
	my_vec->x+=x;
	my_vec->y+=y;
	my_vec->z+=z;
}

///Compare two vectors
int vec_cmp_tol(struct vec *my_vec1,struct vec *my_vec2, double tol)
{
	if (fabs(my_vec2->z-my_vec1->z)<tol)
	{
		if (fabs(my_vec2->x-my_vec1->x)<tol)
		{
			if (fabs(my_vec2->y-my_vec1->y)<tol)
			{
				return 0;
			}
		}
	}
		
	return 1;
}

///Compare two vectors
int vec_cmp(struct vec *my_vec1,struct vec *my_vec2)
{
	return vec_cmp_tol(my_vec1,my_vec2, 1e-12);
}

int vec_cmp_exact(struct vec *my_vec1,struct vec *my_vec2)
{
	if (my_vec2->z==my_vec1->z)
	{
		if (my_vec2->x==my_vec1->x)
		{
			if (my_vec2->y==my_vec1->y)
			{
				return 0;
			}
		}
	}
		
	return 1;
}


///Calculate the angle between two vectors
double vec_ang(struct vec *one,struct vec *two)
{
	double dot;
	double ret;

	dot=vec_dot(one,two);

	ret=acos(dot/(vec_mod(one)*vec_mod(two)));

	//if (ang>PI/2.0)
	//{
	//	ang-=PI/2.0;
	//}

	return ret;
}


///Flips a vector in the z axis
void vec_flip_y_axis(struct vec *one,double y)
{
	double dy=(one->y-y);
	one->y-=2.0*dy;
}

void vec_dump_to_file(char *file_name,struct vec *one)
{
	FILE *out=fopen(file_name,"w");
	fprintf(out,"%le %le %le\n",one->z,one->x,one->y);
	fclose(out);
}


