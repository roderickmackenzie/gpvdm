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
#include <ray_fun.h>
#include <util.h>
#include <triangle.h>
#include <triangles.h>
#include <objects.h>
#include <string.h>

/** @file ray_search_intersect.c
	@brief Ray tracing for the optical model, this should really be split out into it's own library.
*/

struct triangle *search_triangle(struct simulation *sim,struct device *dev,struct ray *my_ray,int do_update)
{
int i;
int found=-1;
int o=0;

double min_dist=1000.0;
double dist=0;

struct vec ret;
vec_init(&ret);

struct vec tmp;
vec_init(&tmp);

struct vec store;
vec_init(&store);
struct object *obj;
struct world *w=&(dev->w);
struct triangle *tri_ret;
int search=TRUE;
tri_ret=NULL;

	for (o=0;o<w->objects;o++)
	{
		obj=&(w->obj[o]);
		search=TRUE;
		if ((obj->min.y>my_ray->xy.y)&&(obj->max.y>my_ray->xy.y))
		{
			if (my_ray->dir.y<0)
			{
				search=FALSE;
			}
		}else
		if ((obj->min.y<my_ray->xy.y)&&(obj->max.y<my_ray->xy.y))
		{
			if (my_ray->dir.y>0)
			{
				search=FALSE;
			}
		}

		if (search==TRUE)
		{
			for (i=0;i<obj->tri.len;i++)
			{
				//dump_plane(sim,in);
				//dump_plane_to_file("triangles.dat",in);
				found=ray_intersect(&ret,&(obj->tri.data[i]),my_ray);
				//vec_print(&ret);

				//getchar();

				if (found==TRUE)
				{
					vec_cpy(&tmp,&ret);
					//vec_print(&ret);
					//getchar();
					//vec_print(&(in->rays[ray].xy));
					vec_sub(&tmp,&(my_ray->xy));
					dist=vec_fabs(&tmp);

					if (dist>1e-12)
					{
						if (dist<min_dist)
						{
							tri_ret=&(obj->tri.data[i]);
							if (do_update==TRUE)
							{
								vec_cpy(&(my_ray->xy_end),&ret);
							}
							min_dist=dist;
						}
					}
				}

			}
		}
	}


return tri_ret;
}


//Experimental acceleration can be removed not relible
void ray_label_triangles(struct simulation *sim,struct device *dev)
{
int o;
int i;
struct vec cog;
struct vec n;
struct vec n_inv;

struct vec tmp;
struct vec pos;
struct triangle *tri;

struct object *obj;
struct object *obj0;
struct object *obj1;
struct world *w=&(dev->w);

//struct ray my_ray;

	for (o=0;o<w->objects;o++)
	{
		obj=&(w->obj[o]);

		for (i=0;i<obj->tri.len;i++)
		{
			tri=&(obj->tri.data[i]);

			tri->obj_left=-1;
			tri->obj_right=-1;

			triangle_cal_cog(tri);
			vec_cpy(&cog,&(tri->cog));

			triangle_norm(&n,tri);

			vec_cpy(&n_inv,&n);
			vec_mul(&n_inv,-1.0);

			//rhs
			vec_cpy(&pos,&cog);
			vec_cpy(&tmp,&n);
			vec_mul(&tmp,1e-10);
			vec_add(&pos,&tmp);

			obj0=ray_obj_search_xyz(sim,dev,&pos);

			//lhs
			vec_cpy(&pos,&cog);
			vec_cpy(&tmp,&n_inv);
			vec_mul(&tmp,1e-10);
			vec_add(&pos,&tmp);

			obj1=ray_obj_search_xyz(sim,dev,&pos);

			//vec_print(&cog);

			if (obj0!=NULL)
			{
				//printf("%s\n",obj0->name);
				tri->obj_left=obj0->uid;
			}

			if (obj1!=NULL)
			{
				//printf("%s\n",obj1->name);
				tri->obj_right=obj1->uid;
			}

			//getchar();


		}
	}

	//getchar();
}

struct intersections
{
	int intersections;
	int uid;
	struct vec points[30];
	int n_points;
};

int vec_in_list(struct intersections *ft, struct vec *in)
{
int i;
for (i=0;i<ft->n_points;i++)
{
	//printf("%le %le %le\n",ft->points[i].x,ft->points[i].y,ft->points[i].z);
	//printf("%le %le %le\n\n",in->x,in->y,in->z);

	if (vec_cmp_tol(&(ft->points[i]),in,1e-11)==0)
	{
		//printf("0\n");
		return 0;
	}
}
//printf("-1\n");
return -1;
}
//Searches which object a given xyz point is within.
struct object *ray_obj_search_xyz(struct simulation *sim,struct device *dev,struct vec *xyz)
{
int i;
int found=-1;
//printf("%le %le %le\n",xyz->x,xyz->y,xyz->z);
//objects_dump(sim,in);
struct object *obj;
struct world *w=&(dev->w);
int min_dist_index=-1;
//double min_dist=1000.0;

int objs_found=0;

int FT_MAX=100;
struct intersections ft[FT_MAX];	//found triangles

//double dist[100];
int uid=0;
int ii;
int in_list=FALSE;
int o;

for (ii=0;ii<100;ii++)
{
	ft[ii].n_points=0;
}


struct vec ret;
vec_init(&ret);

struct vec tmp;
vec_init(&tmp);

struct ray my_ray;
vec_init(&tmp);
vec_set(&tmp,0.0,1.0,0.0);

vec_cpy(&(my_ray.xy),xyz);
vec_cpy(&(my_ray.dir),&tmp);

	//FILE *out;
	//out=fopen("inter.dat","w");
	for (o=0;o<w->objects;o++)
	{
		obj=&(w->obj[o]);

		//printf("%s\n",obj->name);
		//getchar();
		if (vec_within_box(&(obj->min),&(obj->max),&(my_ray.xy))==0)
		{
			//printf("%s %le %le %le\n",obj->name,my_ray.xy.x,my_ray.xy.y,my_ray.xy.z);
			for (i=0;i<obj->tri.len;i++)
			{
				if (obj->tri.edges_calculated==FALSE)
				{
					ewe(sim,"The edges have not been calculated for this object %s\n",obj->name);
				}
				//printf("bing->%s\n",obj->name);
				//getchar();
				//dump_plane(sim,in);
				//dump_plane_to_file("triangles.dat",in);

				found=ray_intersect(&ret,&(obj->tri.data[i]),&(my_ray));


				/*if (found!=0)
				{
					printf("found %d \n",found);
				}*/
				//if (found!=0)
				//{
				//	exit(0);
				//}
				uid=obj->tri.data[i].object_uid;

				if (found!=0)
				{

					//ray_dump_triangle(sim,in,i);
					//getchar();
					in_list=FALSE;

					for (ii=0;ii<objs_found;ii++)
					{
						if (ft[ii].uid==uid)
						{
							in_list=TRUE;
							if (vec_in_list(&(ft[ii]),&ret)!=0)		//If two triangles share the same plane then don't add (these would be triangles sharing an edge)
							{
								//if (uid==2)
								//{
								//	fprintf(out,"%le %le %le\n\n",ret.x,ret.y,ret.z);
								//}
								//printf("%d %d %d\n",ii,objs_found,ft[ii].n_points);
								vec_cpy(&(ft[ii].points[ft[ii].n_points]),&ret);
								ft[ii].intersections++;
								ft[ii].n_points++;
								
								//vec_print(&(points[ii]));
								//vec_print(&ret);
								//FILE *oh1=fopen("oh.dat","a");
								//fprintf(oh1,"%le %le %le\n",ret.z,ret.x,ret.y);
								//fclose(oh1);

								//printf("here2 %d %d %s\n",ii,uid,obj->name);

							}
							break;
						}
					}

					if (in_list==FALSE)
					{
						ft[objs_found].uid=uid;


						vec_cpy(&(ft[objs_found].points[0]),&ret);	//store xyz intersect point

						//if (uid==2)
						//{
						//	fprintf(out,"%le %le %le\n\n",ret.x,ret.y,ret.z);
						//}
						//vec_print(&ret);

						//dist[objs_found]=vec_fabs(&tmp);
						ft[objs_found].intersections=1;
						ft[objs_found].n_points=1;
						//FILE *oh=fopen("oh.dat","w");
						//fclose(oh);
						//printf("here1\n");
						objs_found++;
						if (objs_found>FT_MAX)
						{
							ewe(sim,"objs_found>FT_MAX");
						}
					}

				}

			}
		}
	}

	//fclose(out);
	//printf(">>>>%le %le %le %d %d\n",xyz->x,xyz->y,xyz->z,objs_found);

	//vec_dump_to_file("point.dat",xyz);
	//if ((fabs(xyz->x-1.000000e-10)<1e-10)&&(fabs(xyz->y-0.000000e+00)<1e-10)&&(fabs(xyz->z-5.333333e-07)<1e-10))
	//{
	//	vec_print(&points[0]);
	//	printf(">>>>%d\n",intersections[0]);

	//	printf("quit\n");
	//	getchar();
		//exit(0);
	//}
	//getchar();
	//printf("objs_found=%d\n",objs_found);
	if (objs_found==0)
	{
		//So we think the point is outside the box. We could have a floating point issue so just check we are not in the big box.
		if (w->objects>0)
		{
			obj=&(w->obj[0]);

			if (vec_within_box(&(obj->min),&(obj->max),&(my_ray.xy))==0)
			{
				//printf_log(sim,"Floating point issue\n");
				return obj;
			}
		}
		return NULL;
	}


	/*for (ii=objs_found-1;ii>=0;ii--)
	{
		uid=ft[ii].uid;
		printf(">>%d inter=%d %s %d\n",uid,ft[ii].intersections,w->obj[uid].name,ft[ii].intersections %2);
		for (n=0;n<ft[ii].n_points;n++)
		{
			printf("%le %le %le\n",ft[ii].points[n].x,ft[ii].points[n].y,ft[ii].points[n].z);
		}
	}*/

	for (ii=objs_found-1;ii>=0;ii--)
	{
		//printf("%d\n",ii);
		if ((ft[ii].intersections % 2) != 0 )
		{
			min_dist_index=ft[ii].uid;
			break;
		}
		//printf(">>%d inter=%d %le %s %d\n",uids[ii],intersections[ii],dist[ii],dev->obj[uids[ii]].name,min_dist_index);
	}

	//getchar();
	if (min_dist_index==-1)
	{
		//Again quick fix for windows foating point issue
		if (w->objects>0)
		{
			obj=&(w->obj[0]);

			if (vec_within_box(&(obj->min),&(obj->max),&(my_ray.xy))==0)
			{
				//printf_log(sim,"Floating point issue\n");
				return obj;
			}
		}
		ewe(sim,"ray is outside the box 2");
	}

	//printf(">>>>>>>>>>>>> %s %s\n",dev->obj[min_dist_index].name,dev->obj[min_dist2].name);
	//getchar();

return &w->obj[min_dist_index];
}

//Which object does a ray start within
//This function just projects the ray forwards and counts the number of triangles it intersects
//If a ray intersects an object once it is within the triangle
//Then calculate the closest triangle and we have the object.
struct object *ray_obj_search(struct simulation *sim,struct device *dev,struct ray *in_ray)
{
int i;
int found=-1;

struct object *obj;
int min_dist_index=-1;
//double min_dist=1000.0;

int nfound=0;

int uids[100];
struct vec points[100];
int intersections[100];
struct world *w=&(dev->w);
//double dist[100];
int uid=0;
int ii;
int in_list=FALSE;
int o;
struct vec ret;
vec_init(&ret);

struct vec tmp;
vec_init(&tmp);


	for (o=0;o<w->objects;o++)
	{
		obj=&(w->obj[o]);
		//vec_print(&(in_ray->xy));
		//vec_print(&(obj->min));
		//vec_print(&(obj->max));
		//printf("%d\n",vec_within_box(&(obj->min),&(obj->max),&(in_ray->xy)));

		if (vec_within_box(&(obj->min),&(obj->max),&(in_ray->xy))==0)
		{
			for (i=0;i<obj->tri.len;i++)
			{
				//printf("%d %d %d\n",o,i,obj->tri[i].object_uid);
				//dump_plane(sim,in);
				//dump_plane_to_file("triangles.dat",in);
				found=ray_intersect(&ret,&(obj->tri.data[i]),in_ray);
				uid=obj->tri.data[i].object_uid;

				if (found!=0)
				{
					//printf("found %d %d %s\n",i,uid,dev->obj[uid].name);
					//ray_dump_triangle(sim,in,i);
					//getchar();
					in_list=FALSE;
					for (ii=0;ii<nfound;ii++)
					{
						if (uids[ii]==uid)
						{
							in_list=TRUE;
							//vec_print(&(points[ii]));
							//vec_print(&ret);
							if (vec_cmp(&(points[ii]),&ret)!=0)		//If two triangles share the same plane then don't add
							{
								intersections[ii]++;
							}
							break;
						}
					}
					if (in_list==FALSE)
					{
						uids[nfound]=uid;

						//vec_cpy(&tmp,&ret);
						//vec_sub(&tmp,&(in_ray->xy));

						vec_cpy(&(points[nfound]),&ret);	//store xyz intersect point

						//dist[nfound]=vec_fabs(&tmp);
						intersections[nfound]=1;
						nfound++;
					}

				}

			}
		}
		//getchar();
	}


	if (nfound==0)
	{
		return NULL;
	}

	/*for (ii=0;ii<nfound;ii++)
	{
		if (intersections[ii]==1)
		{
			if (dist[ii]<min_dist)
			{
				min_dist=dist[ii];
				min_dist_index=uids[ii];
			}
		}
		printf(">>%d inter=%d %le %s %d\n",uids[ii],intersections[ii],dist[ii],dev->obj[uids[ii]].name,min_dist_index);
	}*/

	//int min_dist2=0;
	for (ii=nfound-1;ii>=0;ii--)
	{
		//printf("%d\n",ii);
		if (intersections[ii]==1)
		{
			min_dist_index=uids[ii];
			break;
		}
		//printf(">>%d inter=%d %le %s %d\n",uids[ii],intersections[ii],dist[ii],dev->obj[uids[ii]].name,min_dist_index);
	}

	//getchar();
	if (min_dist_index==-1)
	{
		ewe(sim,"ray is outside the box >2");
	}

	//printf(">>>>>>>>>>>>> %s %s\n",dev->obj[min_dist_index].name,dev->obj[min_dist2].name);
	//getchar();
return &w->obj[min_dist_index];
}

int search_object(struct simulation *sim,struct device *dev,struct ray *my_ray)
{
	//search_get_intersects(sim,in,my_ray);
	struct triangle *tri;
	tri=search_triangle(sim,dev,my_ray,TRUE);
	if (tri==NULL)
	{
		return -1;
	}

	return tri->object_uid;
}

struct object *ray_obj_search_by_name(struct simulation *sim,struct device *dev,char *serach_name)
{
int o;
struct object *obj;
struct world *w=&(dev->w);

	for (o=0;o<w->objects;o++)
	{
		obj=&(w->obj[o]);
		if (strcmp(obj->name,serach_name)==0)
		{
			return obj;
		}

	}

return NULL;
}
