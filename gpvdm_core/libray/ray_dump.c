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
#include <dat_file.h>
#include <string.h>
#include <color.h>
#include <dump.h>
#include <util.h>
#include <detector.h>

/** @file ray.c
	@brief Ray tracing for the optical model, this should really be split out into it's own library.
*/


void ray_dump_triangle(struct simulation *sim,struct device *dev,struct image *in,struct triangle *tri)
{

	char temp[200];
	struct world *my_world=&(dev->w);
	//printf("file dump\n");
	struct dat_file buf;
	dat_file_init(&buf);

	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,"Ray trace triange file");
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
	buf.y=1;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#name %s\n",my_world->obj[tri->object_uid].name);
	buffer_add_string(&buf,temp);


	sprintf(temp,"%le %le %le\n",tri->xy0.z,tri->xy0.x,tri->xy0.y);
	buffer_add_string(&buf,temp);

	sprintf(temp,"%le %le %le\n",tri->xy1.z,tri->xy1.x,tri->xy1.y);
	buffer_add_string(&buf,temp);

	sprintf(temp,"%le %le %le\n",tri->xy2.z,tri->xy2.x,tri->xy2.y);
	buffer_add_string(&buf,temp);

	sprintf(temp,"%le %le %le\n",tri->xy0.z,tri->xy0.x,tri->xy0.y);
	buffer_add_string(&buf,temp);

	sprintf(temp,"\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"\n");
	buffer_add_string(&buf,temp);


	//sprintf(temp,"triangle_%d.dat",tri->tri_uid);
	//buffer_dump_path(sim,"",temp,&buf);
	//buffer_free(&buf);
}

void dump_plane_to_file(struct simulation *sim,char *file_name,struct image *in,struct device *dev)
{
	int i;

	int r;
	int g;
	int b;

	char temp[200];

	int iw=0;

	struct ray_worker *worker;
	struct world *w=&(dev->w);
	//printf("file dump\n");
	struct dat_file buf;
	dat_file_init(&buf);

	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,"Ray trace file");
	strcpy(buf.type,"poly");
	strcpy(buf.y_label,"Position");
	strcpy(buf.x_label,"Position");
	strcpy(buf.data_label,"Position");

	strcpy(buf.y_units,"m");
	strcpy(buf.x_units,"m");
	strcpy(buf.data_units,"m");

	wavelength_to_rgb(&r,&g,&b,in->lam[in->worker[0].l]);
	sprintf(buf.rgb,"%.2x%.2x%.2x",r,g,b);

	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=1;
	buf.y=w->triangles;
	buf.z=1;
	buffer_add_info(sim,&buf);

	for (iw=0;iw<in->worker_max;iw++)
	{
		worker=&(in->worker[iw]);

		//printf("%d %d\n",w,worker->nrays);
		for (i=0;i<worker->nrays;i++)
		{
			if (worker->rays[i].state==DONE)
			{
				sprintf(temp,"%le %le %le\n",
											worker->rays[i].xy.z,		worker->rays[i].xy.x,		worker->rays[i].xy.y);
				buffer_add_string(&buf,temp);

				sprintf(temp,"%le %le %le\n",
											worker->rays[i].xy_end.z,	worker->rays[i].xy_end.x,	worker->rays[i].xy_end.y);
				buffer_add_string(&buf,temp);

				sprintf(temp,"\n");
				buffer_add_string(&buf,temp);

				sprintf(temp,"\n");
				buffer_add_string(&buf,temp);
			}

		}
	}

	buffer_dump_path(sim,"",file_name,&buf);
	buffer_free(&buf);

}


void dump_ray_to_file(struct simulation *sim,struct image *in,struct ray *my_ray,struct device *dev)
{

	int r;
	int g;
	int b;

	char temp[200];
	struct world *w=&(dev->w);

	struct dat_file buf;
	dat_file_init(&buf);

	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,"Ray trace file");
	strcpy(buf.type,"poly");
	strcpy(buf.y_label,"Position");
	strcpy(buf.x_label,"Position");
	strcpy(buf.data_label,"Position");

	strcpy(buf.y_units,"m");
	strcpy(buf.x_units,"m");
	strcpy(buf.data_units,"m");

	wavelength_to_rgb(&r,&g,&b,600e-9);
	sprintf(buf.rgb,"%.2x%.2x%.2x",r,g,b);

	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=1;
	buf.y=w->triangles;
	buf.z=1;
	buffer_add_info(sim,&buf);


	struct vec v;

	vec_cpy(&v,&(my_ray->xy_end));
	vec_sub(&v,&(my_ray->xy));
	vec_mul(&v,0.25);
	vec_add(&v,&(my_ray->xy));

	sprintf(temp,"%le %le %le\n",
								my_ray->xy.z,		my_ray->xy.x,		my_ray->xy.y);
	buffer_add_string(&buf,temp);

	sprintf(temp,"%le %le %le\n",
								my_ray->xy_end.z,	my_ray->xy_end.x,	my_ray->xy_end.y);
	buffer_add_string(&buf,temp);

	sprintf(temp,"%le %le %le\n",
								v.z,	v.x,	v.y);
	buffer_add_string(&buf,temp);

	sprintf(temp,"\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"\n");
	buffer_add_string(&buf,temp);


	sprintf(temp,"ray_%d.dat",my_ray->uid);
	printf("dumped %s \n",temp);
	buffer_dump_path(sim,"",temp,&buf);
	buffer_free(&buf);

}

void dump_plane(struct simulation *sim,struct device *dev,struct image *in)
{
	int i=0;
	int w=0;

	printf_log(sim,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	struct object *obj;
	struct ray_worker *worker;
	struct world *my_world=&(dev->w);

	printf_log(sim,"start:\n");
	ray_src_dump(sim,dev);

	printf_log(sim,"triangles:\n");
	int o=0;

	//int n=0;
	for (o=0;o<my_world->objects;o++)
	{
		obj=&(my_world->obj[o]);
		printf_log(sim,"%s\n",obj->name);
		for (i=0;i<obj->tri.len;i++)
		{
			printf(" type=%d\n",obj->tri.data[i].object_type);
			//getchar();
			printf_log(sim," (%le,%le,%le)\n",obj->tri.data[i].xy0.x,obj->tri.data[i].xy0.y,obj->tri.data[i].xy0.z);
			printf_log(sim," (%le,%le,%le)\n",obj->tri.data[i].xy1.x,obj->tri.data[i].xy1.y,obj->tri.data[i].xy1.z);
			printf_log(sim," (%le,%le,%le)\n",obj->tri.data[i].xy2.x,obj->tri.data[i].xy2.y,obj->tri.data[i].xy2.z);
			printf_log(sim,"\n");


	//in->tri[i].xy1.x,in->tri[i].xy1.y,in->tri[i].edge);
		}
	}

	printf_log(sim,"rays x,y,x_vec,y_vec:\n");

	for (w=0;w<in->worker_max;w++)
	{
		worker=&(in->worker[w]);
		for (i=0;i<worker->nrays;i++)
		{
			printf_log(sim,"%d (%le,%le,%le) (%le,%le,%le) (%lf,%lf,%lf) mag=%lf state=%d\n",
					worker->rays[i].state,
					worker->rays[i].xy.x		,	worker->rays[i].xy.y	,	worker->rays[i].xy.z,
					worker->rays[i].xy_end.x	,	worker->rays[i].xy_end.y,	worker->rays[i].xy_end.z,
					worker->rays[i].dir.x		,	worker->rays[i].dir.y	,	worker->rays[i].dir.z,
					worker->rays[i].mag, worker->rays[i].state);

		}
	}
	printf_log(sim,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

}

void dump_extraction_efficiency(struct simulation *sim,struct device *dev,struct image *in)
{
	int i;
	int layer;
	char temp[200];
	char path[PATH_MAX];
	struct dat_file buf;
	dat_file_init(&buf);

	for (layer=0;layer<dev->my_epitaxy.layers;layer++)
	{

		if (dev->my_epitaxy.layer[layer].pl_enabled==TRUE)
		{
			buffer_malloc(&buf);
			buf.y_mul=1.0;
			buf.y_mul=1e9;
			sprintf(buf.title,"Escape probability for layer %d",layer);
			strcpy(buf.type,"linegraph");
			strcpy(buf.y_label,"Wavelength");
			strcpy(buf.data_label,"Probability");
			strcpy(buf.y_units,"nm");
			strcpy(buf.data_units,"a.u.");
			buf.logscale_x=0;
			buf.logscale_y=0;
			buf.x=1;
			buf.y=in->ray_wavelength_points;
			buf.z=1;
			buffer_add_info(sim,&buf);

			for (i=0;i<in->ray_wavelength_points;i++)
			{
				sprintf(temp,"%le %le\n",in->lam[i],dev->my_epitaxy.layer[layer].photon_extract_eff[i]);
				buffer_add_string(&buf,temp);
			}

			sprintf(path,"escape_probability_%d.dat",layer);
			buffer_dump_path(sim,"",path,&buf);
			buffer_free(&buf);
		}
	}
}

void dump_ang_escape(struct simulation *sim,struct image *in)
{
	int x;
	int y;
	char temp[200];
	struct dat_file buf;
	dat_file_init(&buf);

	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,"Photon escape probability");
	strcpy(buf.type,"heat");
	strcpy(buf.y_label,"Wavelength");
	strcpy(buf.x_label,"Angle");
	strcpy(buf.data_label,"Probability");

	strcpy(buf.y_units,"nm");
	strcpy(buf.x_units,"Degrees");
	strcpy(buf.data_units,"a.u.");

	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=in->ray_wavelength_points;
	buf.y=in->escape_bins;
	buf.z=1;
	buffer_add_info(sim,&buf);

	for (x=0;x<in->ray_wavelength_points;x++)
	{
		for (y=0;y<in->escape_bins;y++)
		{
			sprintf(temp,"%le %Le %Le\n",in->lam[x],in->angle[y],in->ang_escape[x][y]);
			buffer_add_string(&buf,temp);
		}
		buffer_add_string(&buf,"\n");
	}

	buffer_dump_path(sim,"","ang_escape.dat",&buf);
	buffer_free(&buf);
}

void dump_ang_escape_as_rgb(struct simulation *sim,struct image *in)
{
	long double X;
	long double Y;
	long double Z;
	int R;
	int G;
	int B;

	int x;
	int y;
	char temp[200];
	struct dat_file buf_rgb;

	struct dat_file buf_X;
	struct dat_file buf_Y;
	struct dat_file buf_Z;

	struct dat_file buf_x;
	struct dat_file buf_y;
	struct dat_file buf_z;

	struct math_xy spec_out;

	dat_file_init(&buf_rgb);

	dat_file_init(&buf_X);
	dat_file_init(&buf_Y);
	dat_file_init(&buf_Z);

	dat_file_init(&buf_x);
	dat_file_init(&buf_y);
	dat_file_init(&buf_z);

	buffer_malloc(&buf_rgb);
	buf_rgb.y_mul=1.0;
	buf_rgb.x_mul=1e9;
	strcpy(buf_rgb.title,"RGB color");
	strcpy(buf_rgb.type,"rgb");
	strcpy(buf_rgb.y_label,"Angle");
	strcpy(buf_rgb.data_label,"Color");
	strcpy(buf_rgb.y_units,"Degrees");
	strcpy(buf_rgb.data_units,"a.u.");
	buf_rgb.x=1;
	buf_rgb.y=in->escape_bins;
	buf_rgb.z=1;
	buffer_add_info(sim,&buf_rgb);


	buffer_malloc(&buf_X);
	buf_X.y_mul=1.0;
	buf_X.data_mul=1.0;
	strcpy(buf_X.title,"CIE 1931 color space - X");
	strcpy(buf_X.type,"xy");
	strcpy(buf_X.y_label,"Angle");
	strcpy(buf_X.data_label,"Color");
	strcpy(buf_X.y_units,"Degrees");
	strcpy(buf_X.data_units,"a.u.");
	buf_X.x=1;
	buf_X.y=in->escape_bins;
	buf_X.z=1;
	buffer_add_info(sim,&buf_X);

	buffer_malloc(&buf_Y);
	buf_Y.y_mul=1.0;
	buf_Y.data_mul=1.0;
	strcpy(buf_Y.title,"CIE 1931 color space - Y");
	strcpy(buf_Y.type,"xy");
	strcpy(buf_Y.y_label,"Angle");
	strcpy(buf_Y.data_label,"Color");
	strcpy(buf_Y.y_units,"Degrees");
	strcpy(buf_Y.data_units,"a.u.");
	buf_Y.x=1;
	buf_Y.y=in->escape_bins;
	buf_Y.z=1;
	buffer_add_info(sim,&buf_Y);

	buffer_malloc(&buf_Z);
	buf_Z.y_mul=1.0;
	buf_Z.data_mul=1.0;
	strcpy(buf_Z.title,"CIE 1931 color space - Z");
	strcpy(buf_Z.type,"xy");
	strcpy(buf_Z.y_label,"Angle");
	strcpy(buf_Z.data_label,"Color");
	strcpy(buf_Z.y_units,"Degrees");
	strcpy(buf_Z.data_units,"a.u.");
	buf_Z.x=1;
	buf_Z.y=in->escape_bins;
	buf_Z.z=1;
	buffer_add_info(sim,&buf_Z);

	buffer_malloc(&buf_x);
	buf_x.y_mul=1.0;
	buf_x.data_mul=1.0;
	strcpy(buf_x.title,"CIE 1931 color space - x");
	strcpy(buf_x.type,"xy");
	strcpy(buf_x.y_label,"Angle");
	strcpy(buf_x.data_label,"Color");
	strcpy(buf_x.y_units,"Degrees");
	strcpy(buf_x.data_units,"a.u.");
	buf_x.x=1;
	buf_x.y=in->escape_bins;
	buf_x.z=1;
	buffer_add_info(sim,&buf_x);

	buffer_malloc(&buf_y);
	buf_y.y_mul=1.0;
	buf_y.data_mul=1.0;
	strcpy(buf_y.title,"CIE 1931 color space - y");
	strcpy(buf_y.type,"xy");
	strcpy(buf_y.y_label,"Angle");
	strcpy(buf_y.data_label,"Color");
	strcpy(buf_y.y_units,"Degrees");
	strcpy(buf_y.data_units,"a.u.");
	buf_y.x=1;
	buf_y.y=in->escape_bins;
	buf_y.z=1;
	buffer_add_info(sim,&buf_y);

	buffer_malloc(&buf_z);
	buf_z.y_mul=1.0;
	buf_z.data_mul=1.0;
	strcpy(buf_z.title,"CIE 1931 color space - z");
	strcpy(buf_z.type,"xy");
	strcpy(buf_z.y_label,"Angle");
	strcpy(buf_z.data_label,"Color");
	strcpy(buf_z.y_units,"Degrees");
	strcpy(buf_z.data_units,"a.u.");
	buf_z.x=1;
	buf_z.y=in->escape_bins;
	buf_z.z=1;
	buffer_add_info(sim,&buf_z);


	for (y=0;y<in->escape_bins;y++)
	{
		inter_init(sim,&spec_out);

		for (x=0;x<in->ray_wavelength_points;x++)
		{
			inter_append(&spec_out,in->lam[x],in->ang_escape[x][y]);
		}

		color_cie_cal_XYZ(sim,&X,&Y,&Z,&spec_out,FALSE);

		color_XYZ_to_rgb(&R,&G,&B,X,Y,Z);

		sprintf(temp,"%Le %.2x%.2x%.2x\n",in->angle[y],R,G,B);
		buffer_add_string(&buf_rgb,temp);

		sprintf(temp,"%Le %Le\n",in->angle[y],X);
		buffer_add_string(&buf_X,temp);

		sprintf(temp,"%Le %Le\n",in->angle[y],Y);
		buffer_add_string(&buf_Y,temp);

		sprintf(temp,"%Le %Le\n",in->angle[y],Z);
		buffer_add_string(&buf_Z,temp);
		long double tot=X+Y+Z;
		if (tot==0)
		{
			tot=1.0;
			X=0.0;
			Y=0.0;
			X=0.0;

		}
		sprintf(temp,"%Le %Le\n",in->angle[y],X/(tot));
		buffer_add_string(&buf_x,temp);

		sprintf(temp,"%Le %Le\n",in->angle[y],Y/(tot));
		buffer_add_string(&buf_y,temp);

		sprintf(temp,"%Le %Le\n",in->angle[y],Z/(tot));
		buffer_add_string(&buf_z,temp);

		inter_free(&spec_out);
	}

	buffer_dump_path(sim,"","theta_RGB.dat",&buf_rgb);
	buffer_free(&buf_rgb);

	buffer_dump_path(sim,"","theta_X.dat",&buf_X);
	buffer_free(&buf_X);

	buffer_dump_path(sim,"","theta_Y.dat",&buf_Y);
	buffer_free(&buf_Y);

	buffer_dump_path(sim,"","theta_Z.dat",&buf_Z);
	buffer_free(&buf_Z);

	buffer_dump_path(sim,"","theta_small_x.dat",&buf_x);
	buffer_free(&buf_x);

	buffer_dump_path(sim,"","theta_small_y.dat",&buf_y);
	buffer_free(&buf_y);

	buffer_dump_path(sim,"","theta_small_z.dat",&buf_z);
	buffer_free(&buf_z);

	 detector_dump_bins(sim,in);
}

void dump_rendered_image(struct simulation *sim,char *out_dir, struct world *w, struct image *in)
{
	int d;
	char temp[100];
	struct detector *det;
	struct dat_file buf;

	for (d=0;d<w->detectors;d++)
	{
		det=&(w->det[d]);
		if (det->viewpoint_enabled==TRUE)
		{

			dat_file_init(&buf);

			buffer_malloc(&buf);
			buf.y_mul=1.0;
			buf.x_mul=1.0;
			strcpy(buf.title,"Rendered image");
			strcpy(buf.type,"rgb");
			strcpy(buf.y_label,"x");
			strcpy(buf.x_label,"no used");
			strcpy(buf.data_label,"y");

			strcpy(buf.y_units,"Camera pixels");
			strcpy(buf.x_units,"Camera pixels");
			strcpy(buf.data_units,"Camera pixels");

			buf.logscale_x=0;
			buf.logscale_y=0;
			buf.x=in->viewpoint_dim.zlen;
			buf.y=in->viewpoint_dim.xlen;
			buf.z=1;
			buffer_add_info(sim,&buf);

			buffer_add_zxy_rgb_data(sim,&buf,(&in->viewpoint_dim),in->viewpoint_image);

			sprintf(temp,"RAY_image%d.dat",d);
			buffer_dump_path(sim,out_dir,temp,&buf);
			buffer_free(&buf);
		}
	}
}

void dump_rendered_cross_section(struct simulation *sim,char *out_dir, struct world *w, struct image *in)
{


	int l;
	char file_name[100];
	char unit[10];
	long double mul;
	int d;
	struct detector *det;
	struct dat_file buf;

	for (d=0;d<w->detectors;d++)
	{
		det=&(w->det[d]);
		if (det->viewpoint_enabled==TRUE)
		{

			dat_file_init(&buf);

			for (l=0;l<in->viewpoint_dim.ylen;l++)
			{
				get_wavelength_dim(unit,&mul,in->viewpoint_dim.y[l]);

				buffer_malloc(&buf);

				dim_info_to_buf(&buf,&(in->viewpoint_dim));
				sprintf(buf.title,"Emission profile for %.0Lf %s",in->viewpoint_dim.y[l]*mul,unit);
				strcpy(buf.type,"3d");
				strcpy(buf.y_label,"x");
				strcpy(buf.x_label,"no used");
				strcpy(buf.data_units,"?");
				strcpy(buf.data_label,"Emission intensity");



				//strcpy(buf.y_units,"Camera pixels");
				//strcpy(buf.x_units,"Camera pixels");
				//strcpy(buf.data_units,"Camera pixels");

				buf.logscale_x=0;
				buf.logscale_y=0;
				buf.x=1;
				buf.y=in->viewpoint_dim.xlen;
				buf.z=1;
				buffer_add_info(sim,&buf);

				//buffer_add_zxy_rgb_data(sim,&buf,(&in->viewpoint_dim),in->viewpoint_image);
				buffer_add_zxy_data_y_slice(sim,&buf,(&in->viewpoint_dim),in->viewpoint_image,0);

				sprintf(file_name,"RAY_cross_section%d_x_%.0Lf%s.dat",d,in->viewpoint_dim.y[l]*mul,unit);
				buffer_dump_path(sim,out_dir,file_name,&buf);

				buffer_free(&buf);
			}
		}
	}
}
