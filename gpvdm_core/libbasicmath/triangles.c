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
#include <triangles.h>
#include <memory.h>
#include <vectors.h>
#include <util.h>
/** @file triangles.c
	@brief Basic low level triangle functions
*/

void triangle_load_from_file(struct simulation *sim,struct triangles *in,char *file_name)
{
int i;
char line[10000];
struct dat_file dat;
dat_file_load_info(sim,&dat,file_name);
in->len=dat.y;
in->max_len=dat.y+10;
in->data=(struct triangle*)malloc(sizeof(struct triangle)*in->max_len);

for (i=0;i<in->len;i++)
{
	triangle_init(&(in->data[i]));
}

FILE *file;
file=fopen(file_name,"r");
int point=0;
int items=0;
int triangle_line=FALSE;
int last_triangle_line=FALSE;
if (file == NULL)
{
	ewe(sim,"Can not load the file %s\n",file_name);
}
int data_start=FALSE;
do
{
	memset(line,0,10000);
	if (fgets(line, 10000, file)==NULL)
	{
		break;
	}

	if (data_start==FALSE)
	{
		if (strcmp_begin(line,"#begin")==0)
		{
			data_start=TRUE;
		}
	}else
	{
		last_triangle_line=triangle_line;
		triangle_line=FALSE;
		if (line[0]!='#')
		{
			if (is_number(line[0])==TRUE)
			{
				if (point==0)
				{
					sscanf(line,"%le %le %le",&(in->data[items].xy0.z),&(in->data[items].xy0.x),&(in->data[items].xy0.y));
					triangle_line=TRUE;
					point++;
				}else
				if (point==1)
				{
					sscanf(line,"%le %le %le",&(in->data[items].xy1.z),&(in->data[items].xy1.x),&(in->data[items].xy1.y));
					triangle_line=TRUE;
					point++;
				}else
				if (point==2)
				{
					sscanf(line,"%le %le %le",&(in->data[items].xy2.z),&(in->data[items].xy2.x),&(in->data[items].xy2.y));
					triangle_line=TRUE;
					point++;
				}
			}

		}
		//printf("%s %d %d\n",line,point,triangle_line);
		if ((triangle_line==FALSE)&&(last_triangle_line==TRUE))
		{
			point=0;
			items++;
		}

		if (items>in->len)
		{
			break;
		}

	}


}while(!feof(file));
fclose(file);

if (items!=in->len)
{
	ewe(sim,"number of triangles does not equal the number I'm expecting %d %d\n",in->len,items);
}
//getchar();
}


void triangles_free(struct triangles *tri)
{
	free_1d( (void *)(&tri->data), sizeof(struct triangle));

	tri->len=0;
	tri->max_len=0;

}

void triangles_cpy(struct triangles *out,struct triangles *in)
{
	//printf("here\n");
	free_1d( (void *)(&out->data), sizeof(struct triangle));
	//printf("here\n");
	out->len=in->len;
	out->max_len=in->max_len;
	//printf("here %d %d\n",out->len,out->max_len);
	malloc_1d((void *) (&out->data),out->max_len, sizeof(struct triangle));
	memcpy(out->data, in->data, in->len*sizeof(struct triangle));

}

void triangles_set_object_type(struct triangles *in,int object_type)
{
int i;
	for (i=0;i<in->len;i++)
	{
		in->data[i].object_type=object_type;
	}

}

void triangles_save(struct simulation *sim,char *file_name,struct triangles *in)
{
	struct triangle *tri;
	struct dat_file buf;
	dat_file_init(&buf);

	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.x_mul=1.0;
	strcpy(buf.title,"Triangle file");
	strcpy(buf.type,"poly");
	strcpy(buf.y_label,"Position");
	strcpy(buf.x_label,"Position");
	strcpy(buf.data_label,"Position");

	strcpy(buf.y_units,"m");
	strcpy(buf.x_units,"m");
	strcpy(buf.data_units,"m");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=1;
	buf.y=triangles_count(in);
	buf.z=1;
	buffer_add_info(sim,&buf);

	triangles_to_dat_file(&buf,in);

	buffer_dump_path(sim,"",file_name,&buf);
	buffer_free(&buf);

}

void triangles_to_dat_file(struct dat_file *buf,struct triangles *in)
{
	int i;
	char temp[1000];
	struct triangle *tri;

	for (i=0;i<in->len;i++)
	{
		//printf("%d\n",i);
		tri=&(in->data[i]);
		if (tri->deleted==FALSE)
		{
			sprintf(temp,"%le %le %le\n",tri->xy0.z,tri->xy0.x,tri->xy0.y);
			buffer_add_string(buf,temp);

			sprintf(temp,"%le %le %le\n",tri->xy1.z,tri->xy1.x,tri->xy1.y);
			buffer_add_string(buf,temp);

			sprintf(temp,"%le %le %le\n",tri->xy2.z,tri->xy2.x,tri->xy2.y);
			buffer_add_string(buf,temp);

			sprintf(temp,"%le %le %le\n",tri->xy0.z,tri->xy0.x,tri->xy0.y);
			buffer_add_string(buf,temp);

			sprintf(temp,"\n");
			buffer_add_string(buf,temp);

			sprintf(temp,"\n");
			buffer_add_string(buf,temp);
		}
	}

}

void triangles_sub_vec(struct triangles *in,struct vec *v)
{
	int i;
	for (i=0;i<in->len;i++)
	{
		vec_sub(&(in->data[i].xy0),v);
		vec_sub(&(in->data[i].xy1),v);
		vec_sub(&(in->data[i].xy2),v);
	}
}

void triangles_rotate_y(struct triangles *in,double ang)
{
	int i;
	for (i=0;i<in->len;i++)
	{
		vec_rotate_y(&(in->data[i].xy0), ang);
		vec_rotate_y(&(in->data[i].xy1), ang);
		vec_rotate_y(&(in->data[i].xy2), ang);
	}
}

void triangles_rotate_x(struct triangles *in,double ang)
{
	int i;
	for (i=0;i<in->len;i++)
	{
		vec_rotate_x(&(in->data[i].xy0), ang);
		vec_rotate_x(&(in->data[i].xy1), ang);
		vec_rotate_x(&(in->data[i].xy2), ang);
	}
}

void triangles_add_vec(struct triangles *in,struct vec *v)
{
	int i;
	for (i=0;i<in->len;i++)
	{
		vec_add(&(in->data[i].xy0),v);
		vec_add(&(in->data[i].xy1),v);
		vec_add(&(in->data[i].xy2),v);
	}
}

void triangles_div_vec(struct triangles *in,struct vec *v)
{
	int i;
	for (i=0;i<in->len;i++)
	{
		vec_div_vec(&(in->data[i].xy0),v);
		vec_div_vec(&(in->data[i].xy1),v);
		vec_div_vec(&(in->data[i].xy2),v);
	}
}

void triangles_calculate_cog(struct triangles *in)
{
	int i;
	double x;
	double y;
	double z;
	struct triangle *tri;

	for (i=0;i<in->len;i++)
	{
		tri=&(in->data[i]);
		triangle_cal_cog(tri);
	}
}

void triangles_find_by_vec(	struct triangles *out, struct triangles *in,struct vec *search)
{
	int i;
	struct triangle *tri;

	for (i=0;i<in->len;i++)
	{
		tri=&(in->data[i]);
		if (tri->deleted==FALSE)
		{
			if (triangle_contains_vec(tri,search)==0)
			{
				triangles_add_triangle(out,tri);
			}
		}

	}

	triangles_calculate_cog(out);
}

void triangles_remove_triangles(struct triangles *full_list, struct triangles *to_remove)
{
	int i;
	int ii;
	struct triangle *tri0;
	struct triangle *tri1;


	for (i=0;i<full_list->len;i++)
	{
		tri0=&(full_list->data[i]);
		for (ii=0;ii<to_remove->len;ii++)
		{
			tri1=&(to_remove->data[ii]);
			if (tri0->deleted==FALSE)
			{
				if (triangle_cmp(tri0,tri1)==0)
				{
					tri0->deleted=TRUE;
				}
			}
		}

	}

}

void triangles_add_many(struct triangles *full_list, struct triangles *in)
{
	int i=0;
	int pos=0;
	struct triangle *tri0;
	struct triangle *tri1;

	for (i=0;i<full_list->len;i++)
	{
		tri0=&(full_list->data[i]);
		if (tri0->deleted==TRUE)
		{
			tri1=&(in->data[pos]);
			if (tri1->deleted==FALSE)
			{
				triangle_cpy(tri0,tri1);
				tri0->deleted=FALSE;
			}

			pos++;
			if (pos==in->len)
			{
				break;
			}
		}

	}

}

void triangles_replace_vec(	struct triangles *in,struct vec *find,struct vec *replace)
{
	int i;
	int recalculate=FALSE;
	struct triangle *tri;

	for (i=0;i<in->len;i++)
	{
		recalculate=FALSE;
		tri=&(in->data[i]);
		if (vec_cmp(&tri->xy0,find)==0)
		{
			recalculate=TRUE;
			vec_cpy(&tri->xy0,replace);
		}

		if (vec_cmp(&tri->xy1,find)==0)
		{
			recalculate=TRUE;
			vec_cpy(&tri->xy1,replace);
		}

		if (vec_cmp(&tri->xy2,find)==0)
		{
			recalculate=TRUE;
			vec_cpy(&tri->xy2,replace);
		}

		if (recalculate==TRUE)
		{
			triangle_cal_cog(tri);
			triangle_cal_edges(tri);
		}
	}


}

struct triangle *triangles_find_by_zx(struct triangles *in,double z0,double x0,double z1,double x1,double z2,double x2)
{
	int i;
	int ii;
	int tot=0;
	struct triangle *ret;
	struct triangle *tri;
	double z[3];
	double x[3];
	z[0]=z0;
	x[0]=x0;

	z[1]=z1;
	x[1]=x1;

	z[2]=z2;
	z[1]=z1;

	ret=NULL;
	tot=0;
	for (i=0;i<in->len;i++)
	{
		tri=&(in->data[i]);
		tot=0;
		for (ii=0;ii<3;ii++)
		{
			if ((z[ii]==tri->xy0.z)&&(x[ii]==tri->xy0.x))
			{
				tot++;
			}else
			if ((z[ii]==tri->xy1.z)&&(x[ii]==tri->xy1.x))
			{
				tot++;
			}else
			if ((z[ii]==tri->xy2.z)&&(x[ii]==tri->xy2.x))
			{
				tot++;
			}
		}

		if (tot==3)
		{
			return tri;
		}
	}

	return NULL;
}

int triangles_count(struct triangles *in)
{
	int i;
	int count=0;
	struct triangle *tri;
	for (i=0;i<in->len;i++)
	{
		tri=&(in->data[i]);
		if (tri->deleted==FALSE)
		{
			count++;
		}

	}

	return count;
}

void triangles_mul_vec(struct triangles *in,struct vec *v)
{
	int i;
	for (i=0;i<in->len;i++)
	{
		vec_mul_vec(&(in->data[i].xy0),v);
		vec_mul_vec(&(in->data[i].xy1),v);
		vec_mul_vec(&(in->data[i].xy2),v);
	}
}

void triangles_print(struct triangles *in)
{
	int i;
	for (i=0;i<in->len;i++)
	{
		printf("(%le,%le,%le)\n",in->data[i].xy0.x,in->data[i].xy0.y,in->data[i].xy0.z);
		printf("(%le,%le,%le)\n",in->data[i].xy1.x,in->data[i].xy1.y,in->data[i].xy1.z);
		printf("(%le,%le,%le)\n",in->data[i].xy2.x,in->data[i].xy2.y,in->data[i].xy2.z);
		printf("\n");
	}

	printf("%d triangles\n",in->len);

}

void triangles_remove_y_zero_triangles(struct triangles *in)
{
	int i;
	struct triangle *tri;

	for (i=0;i<in->len;i++)
	{
		if (in->data[i].xy0.y==0.0)
		{
			if (in->data[i].xy1.y==0.0)
			{
				if (in->data[i].xy2.y==0.0)
				{
					tri=&(in->data[i]);
					tri->deleted=TRUE;
				}
			}

		}

	}


}

void triangles_make_btm_layer_from_top(struct triangles *in)
{
	int i;
	struct triangle *tri;
	struct triangle tmp;
	int len=in->len;
	for (i=0;i<len;i++)
	{
		tri=&(in->data[i]);
		triangle_cpy(&tmp,tri);
		tmp.xy0.y=0.0;
		tmp.xy1.y=0.0;
		tmp.xy2.y=0.0;
		triangles_add_triangle(in, &tmp);
	}
}

void triangles_add_triangle(struct triangles *obj, struct triangle *tri)
{
	memcpy(&(obj->data[obj->len]), tri, sizeof(struct triangle));
/*	triangle_init(&(obj->data[obj->len]));

	obj->data[obj->len].xy0.x=x0;
	obj->data[obj->len].xy0.y=y0;
	obj->data[obj->len].xy0.z=z0;

	obj->data[obj->len].xy1.x=x1;
	obj->data[obj->len].xy1.y=y1;
	obj->data[obj->len].xy1.z=z1;

	obj->data[obj->len].xy2.x=x2;
	obj->data[obj->len].xy2.y=y2;
	obj->data[obj->len].xy2.z=z2;

	obj->data[obj->len].object_type=object_type;
	obj->data[obj->len].object_uid=uid;*/
	obj->len++;
	if (obj->len>=obj->max_len)
	{
		obj->max_len+=1000;
		obj->data=(struct triangle *)realloc(obj->data,obj->max_len*sizeof(struct triangle));
		if (obj->data==NULL)
		{
			printf("triangle memory errror\n");
		}
		//printf("%d %d\n",in->triangles,in->triangles_max);
	}

}

void triangles_malloc(struct triangles *tri)
{
	tri->max_len=30;
	malloc_1d((void *) (&tri->data),tri->max_len, sizeof(struct triangle));
//	tri->data=(struct triangle *)malloc(tri->max_len*sizeof(struct triangle));

	if (tri->data==NULL)
	{
		printf("triangle memory errror\n");
	}
}

void triangles_to_vectors(struct vectors* out,struct triangles *in)
{
	int i;
	struct triangle *tri;
	struct vectors vec_list;


	for (i=0;i<in->len;i++)
	{
		tri=&(in->data[i]);
		vectors_append_no_rep(out,&tri->xy0);
		vectors_append_no_rep(out,&tri->xy1);
		vectors_append_no_rep(out,&tri->xy2);

	}
}

void triangles_cal_angles(struct vectors* out,struct triangles* in)
{
	int i;
	struct triangle *tri;
	struct vec angles;

	for (i=0;i<in->len;i++)
	{
		tri=&(in->data[i]);
		if (tri->deleted==FALSE)
		{
			//triangle_dump("tri.dat",tri);
			triangle_cal_angles(&angles,tri);
			vectors_append(out,&angles);
			//vec_print(&angles);
			//getchar();
		}
	}

}
void triangles_vectors_to_triangles(struct triangles *out,struct vectors* in, int start)
{
	int i;
	double A;
	struct triangle *tri;

	struct triangle new_triangle;
	//start=&(in[0]);

//
	for (i=0;i<in->len-1;i++)
	{
		if (i!=start)
		{
			//tri=&(in->data[i]);
			triangle_init(&new_triangle);
			vec_cpy(&new_triangle.xy0,&in->data[start]);
			vec_cpy(&new_triangle.xy1,&in->data[i]);
			vec_cpy(&new_triangle.xy2,&in->data[i+1]);
			triangle_cal_edges(&new_triangle);
			A=triangle_cal_area(&new_triangle);
			if (A!=0.0)
			{
				triangles_add_triangle(out,&new_triangle);
				printf("%d\n",out->len);
			}
			printf("%le\n",A);

			triangles_dump(out);

		}
	}
}

void triangles_delete_zero_area_triangles(struct triangles *in)
{
	int i;
	double A;
	struct triangle *tri;


	for (i=0;i<in->len;i++)
	{
		tri=&(in->data[i]);

		A=triangle_cal_area_zx(tri);
		if (A==0.0)
		{
			tri->deleted=TRUE;
		}
		//printf("%le\n",A);

	}
}

double triangles_get_area(struct triangles *in)
{
	int i;
	double A=0.0;
	struct triangle *tri;

	for (i=0;i<in->len;i++)
	{
		tri=&(in->data[i]);
		if (tri->deleted==FALSE)
		{
			A+=triangle_cal_area(tri);
		}
	}

	return A;
}

double triangles_get_area_zx(struct triangles *in)
{
	int i;
	double A=0.0;
	struct triangle *tri;

	for (i=0;i<in->len;i++)
	{
		tri=&(in->data[i]);
		if (tri->deleted==FALSE)
		{
			//printf("area=%le\n",triangle_cal_area_zx(tri));
			A+=triangle_cal_area_zx(tri);
		}
	}

	return A;
}

void triangles_init(struct triangles *tri)
{
	tri->len=0;
	tri->max_len=0;
	tri->data=NULL;
	tri->edges_calculated=FALSE;

}

void triangles_cal_edges(struct triangles *in)
{
	int i;
	struct triangle *tri;

	for (i=0;i<in->len;i++)
	{
		tri=&(in->data[i]);
		triangle_cal_edges(tri);
	}

	in->edges_calculated=TRUE;

}

void triangles_dump(struct triangles *in)
{
	int i=0;
	struct triangle *tri;
	for (i=0;i<in->len;i++)
	{
		tri=&(in->data[i]);
		printf("(%lf %lf %lf),(%lf %lf %lf),(%lf %lf %lf) %d\n", tri->xy0.z,tri->xy0.x,tri->xy0.y,
																tri->xy1.z,tri->xy1.x,tri->xy1.y,
																tri->xy2.z,tri->xy2.x,tri->xy2.y,
																tri->object_uid);
	}

}

/*double triangles_compact(struct triangles *in)
{
	int i;
	struct triangle *tri;

	for (i=0;i<in->len;i++)
	{
		tri=&(many->data[i]);
		y_many=triangles_interpolate(many,&(tri_many->cog));
		y_few=triangles_interpolate(few,&(tri_many->cog));
		sum+=fabs(y_many-y_few);
	}

	return sum;
}*/

double triangles_get_delta(struct triangles *many,struct triangles *few)
{
	int i;
	struct triangle *tri_many;
	struct vec tri_cog;
	double sum=0.0;
	double y_many=0.0;
	double y_few=0.0;
	for (i=0;i<many->len;i++)
	{
		tri_many=&(many->data[i]);
		y_many=triangles_interpolate(many,&(tri_many->cog));
		y_few=triangles_interpolate(few,&(tri_many->cog));
		sum+=fabs(y_many-y_few);
	}

	return sum;
}

double triangles_interpolate(struct triangles *in,struct vec *p)
{
	int i;
	char temp[100];
	struct triangle *tri;
	double ret;
	double max=-1e6;
	double m=0.0;
	int i_max=0;
	for (i=0;i<in->len;i++)
	{
		tri=&(in->data[i]);
		if (triangle_vec_within (tri,p)==TRUE)
		{
			m=triangle_get_max_y(tri);
			if (m>max)
			{
				max=m;
				i_max=i;
			}

		}
		//printf("%d %d\n",i,triangle_vec_within (tri,p));
	}

	//sprintf(temp,"tri.dat",i_max);
	//triangle_dump(temp,&(in->data[i_max]));

	//FILE *out=fopen("p.dat","w");
	//fprintf(out,"%le %le %le\n",p->x, p->z,,p->x, p->z));
	//fclose(out);
	ret=triangle_get_y_from_xz(&(in->data[i_max]),p->x, p->z);
	return ret;
}
