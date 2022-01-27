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

/** @file mesh.c
@brief This builds the electrical mesh
*/

#include "device.h"
#include <mesh.h>
#include "inp.h"
#include "util.h"
#include "gpvdm_const.h"
#include "hard_limit.h"
#include <log.h>
#include <cal_path.h>
#include <circuit.h>
#include <matrix.h>
#include <memory.h>
#include <contacts.h>
#include <epitaxy_struct.h>
#include <epitaxy.h>
#include <exp.h>
#include <dump.h>
#include <lang.h>

char *circuit_node_type_to_str(int type)
{
	if (type==CIR_KNOWN)
	{
		return "CIR_KNOWN";
	}else
	if (type==CIR_UNKNOWN)
	{
		return "CIR_UNKNOWN";
	}else
	if (type==CIR_CHANGE_X)
	{
		return "CIR_CHANGE_X";
	}

return "unknown";
}

void circuit_print_nodes(struct simulation * sim,struct circuit *cir)
{
	int n;
	textcolor_rgb(sim, 200, 30, 30);
	for (n=0;n<cir->nodes_len;n++)
	{
		printf_log(sim,"node %d (%d,%d): type=%s V=%le Vlast=%le\n",n,cir->nodes[n].x,cir->nodes[n].y,circuit_node_type_to_str(cir->nodes[n].type),cir->nodes[n].V,cir->nodes[n].V_last);
	}
	textcolor_rgb(sim, -1, -1, -1);

}

void circuit_print_links(struct simulation * sim,struct circuit *cir)
{
	int l;
	struct circuit_link *links=(cir->links);
	struct circuit_link *link;

	textcolor_rgb(sim, 30, 200, 30);

	for (l=0;l<cir->links_len;l++)
	{
		link=&links[l];
		if (links[l].type=='R')
		{
			printf_log(sim,"%d %d R=%le I=%le\n",link->start,link->stop,link->R,link->i);
		}else
		if (link->type=='D')
		{
			printf_log(sim,"%d %d I0=%le Isc=%le n0=%lf I=%le\n",link->start,link->stop,link->I0,link->Isc,link->n0,link->i);
		}else
		if (link->type=='C')
		{
			printf_log(sim,"%d %d C=%le I=%le\n",link->start,link->stop,link->C,link->i);
		}


	}

	textcolor_rgb(sim, -1, -1, -1);
}


void circuit_dump(struct simulation * sim,struct device *dev,struct circuit *cir)
{

	int l;
	struct dat_file buf;
	char line[200];
	dat_file_init(&buf);
	struct circuit_link *links=(cir->links);
	struct circuit_node *nodes=(cir->nodes);
	struct dimensions *dim=&dev->ns.dim;

	buffer_malloc(&buf);

	strcpy(buf.title,_("Electrical circuit"));
	strcpy(buf.type,"circuit");
	dim_info_to_buf(&buf,dim);
	strcpy(buf.data_label,_("Circuit"));
	strcpy(buf.data_units,"");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=1;
	buf.y=cir->links_len;
	buf.z=1;
	buffer_add_info(sim,&buf);

	for (l=0;l<cir->links_len;l++)
	{
		sprintf(line,"%le %le %le %le %le %le %c\n",
												nodes[links[l].start].z_pos,
												nodes[links[l].start].x_pos,
												nodes[links[l].start].y_pos,
												nodes[links[l].stop].z_pos,
												nodes[links[l].stop].x_pos,
												nodes[links[l].stop].y_pos,
												links[l].type);
		buffer_add_string(&buf,line);

	}

	buffer_dump_path(sim,"","circuit.dat",&buf);
	buffer_free(&buf);

}

void circuit_dump_gnuplot(struct simulation * sim,struct device *dev,struct circuit *cir)
{

	int l;
	struct dat_file buf;
	char line[200];
	dat_file_init(&buf);
	struct circuit_link *links=(cir->links);
	struct circuit_node *nodes=(cir->nodes);
	struct dimensions *dim=&dev->ns.dim;

	buffer_malloc(&buf);

	strcpy(buf.title,_("Electrical circuit"));
	strcpy(buf.type,"circuit");
	dim_info_to_buf(&buf,dim);
	strcpy(buf.data_label,_("Circuit"));
	strcpy(buf.data_units,"");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=1;
	buf.y=cir->links_len;
	buf.z=1;
	buffer_add_info(sim,&buf);

	for (l=0;l<cir->links_len;l++)
	{
		sprintf(line,"%le %le %le\n",	nodes[links[l].start].z_pos,
										nodes[links[l].start].x_pos,
										nodes[links[l].start].y_pos);
		buffer_add_string(&buf,line);
		sprintf(line,"%le %le %le\n",	nodes[links[l].stop].z_pos,
										nodes[links[l].stop].x_pos,
										nodes[links[l].stop].y_pos
										);
		buffer_add_string(&buf,line);
		buffer_add_string(&buf,"\n");
		buffer_add_string(&buf,"\n");
	}

	buffer_dump_path(sim,"","circuit_gnuplot.dat",&buf);
	buffer_free(&buf);

}

void circuit_dump_to_obj_file(struct simulation * sim,char *file_name,struct device *dev,struct circuit *cir)
{
	int l;
	int n;
	struct dat_file buf_json;
	char build[1024];
	char line[1024];
	dat_file_init(&buf_json);
	struct circuit_link *links=(cir->links);
	struct circuit_node *nodes=(cir->nodes);
	//struct dimensions *dim=&dev->ns.dim;
	struct circuit_link *link;
	struct circuit_node *node;
	buffer_malloc(&buf_json);

	double r=0.3;
	double g=0.3;
	double b=0.3;

	double x0=1.0;
	double y0=0.0;
	double z0=0.0;

	double r_apply=0.0;
	double g_apply=0.0;
	double b_apply=0.0;

	struct epitaxy* epi=&(dev->my_epitaxy);
	buffer_add_string(&buf_json,"{\n");

	sprintf(build,"\t\"obj%d\":{\n",0);
	buffer_add_string(&buf_json,build);

	sprintf(build,"\t\t\"sub_objects\": %d,\n",cir->links_len);
	buffer_add_string(&buf_json,build);

	for (l=0;l<cir->links_len;l++)
	{
		sprintf(build,"\t\t\"sub_object%d\":{\n",l);
		buffer_add_string(&buf_json,build);
		link=&(links[l]);
		//component type
		strcpy(line,"");
		if (link->type=='R')
		{
			sprintf(build,"\t\t\t\"type\":\"%s\",\n","resistor");
		}else
		if (link->type=='D')
		{
			sprintf(build,"\t\t\t\"type\":\"%s\",\n","diode");
		}else
		{
			sprintf(build,"\t\t\t\"type\":\"%s\",\n","unknown");

		}
		buffer_add_string(&buf_json,build);


		//start
		sprintf(build,"\t\t\t\"xyz\":%d,\n",1);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\"xyz0\":{ \n");
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\t\"x\":%le,\n",nodes[links[l].start].x_pos);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\t\"y\":%le,\n",nodes[links[l].start].y_pos);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\t\"z\":%le\n",nodes[links[l].start].z_pos);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t},\n");
		buffer_add_string(&buf_json,build);

		//dx, dy, dz
		sprintf(build,"\t\t\t\"dxyz\":{ \n");
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\t\"x\":%le,\n",nodes[links[l].stop].x_pos-nodes[links[l].start].x_pos);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\t\"y\":%le,\n",nodes[links[l].stop].y_pos-nodes[links[l].start].y_pos);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\t\"z\":%le\n",nodes[links[l].stop].z_pos-nodes[links[l].start].z_pos);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t},\n");
		buffer_add_string(&buf_json,build);



		r_apply=r;
		g_apply=g;
		b_apply=b;

		if ((links[l].layer%3)==0)
		{
			r_apply=0.0;
			g_apply=0.0;
		}else
		if ((links[l].layer%3)==1)
		{
			g_apply=0.0;
			b_apply=0.0;

		}else
		if ((links[l].layer%3)==2)
		{
			r_apply=0.0;
			b_apply=0.0;
		}

		r_apply=epi->layer[link->layer].s.color_r;
		g_apply=epi->layer[link->layer].s.color_g;
		b_apply=epi->layer[link->layer].s.color_b;

		sprintf(build,"\t\t\t\"r\" : \"%le\",\n",r_apply);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\"g\" : \"%le\",\n",g_apply);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\"b\" : \"%le\",\n",b_apply);
		buffer_add_string(&buf_json,build);

		r=r+0.1;
		g=g+0.1;
		b=b+0.1;
		if (r>1.0)
		{
			r=0.3;
		}

		if (b>1.0)
		{
			b=0.3;
		}

		if (g>1.0)
		{
			g=0.3;
		}

		sprintf(build,"\t\t\t\"R\" : \"%le\",\n",link->R);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\"i\" : \"%le\"\n",link->R);
		buffer_add_string(&buf_json,build);

		if (l!=cir->links_len-1)
		{
			sprintf(build,"\t\t},\n");
			buffer_add_string(&buf_json,build);
		}else
		{
			sprintf(build,"\t\t}\n");
			buffer_add_string(&buf_json,build);
		}
	}


	if (cir->nodes_len!=0)
	{
		sprintf(build,"\t},\n");
		buffer_add_string(&buf_json,build);
	}else
	{
		sprintf(build,"\t}\n");
		buffer_add_string(&buf_json,build);
	}


	for (n=0;n<cir->nodes_len;n++)
	{

		node=&nodes[n];
		x0=node->x_pos;
		y0=node->y_pos;
		z0=node->z_pos;

		r=0.0;
		g=0.0;
		b=1.0;

		if (node->selected==TRUE)
		{
			r=0.0;
			g=1.0;
			b=0.0;
		}

		if (node->type==CIR_UNKNOWN)
		{
			r=0.0;
			g=1.0;
			b=0.0;
		}

		if (node->type==CIR_KNOWN)
		{
			r=0.0;
			g=0.0;
			b=1.0;
		}

		sprintf(build,"\t\t\"obj%d\" :{\n",n+1);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\"type\":\"%s\",\n","marker_small");
		buffer_add_string(&buf_json,build);

		//start
		sprintf(build,"\t\t\t\"xyz\":%d,\n",1);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\"xyz0\":{ \n");
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\t\"x\":%le,\n",x0);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\t\"y\":%le,\n",y0);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\t\"z\":%le\n",z0);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t},\n");
		buffer_add_string(&buf_json,build);

		//dx, dy, dz
		sprintf(build,"\t\t\t\"dxyz\":{ \n");
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\t\"x\":%le,\n",0.0);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\t\"y\":%le,\n",0.0);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\t\"z\":%le\n",0.0);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t},\n");
		buffer_add_string(&buf_json,build);


		sprintf(build,"\t\t\t\"r\" : \"%le\",\n",r);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\"g\" : \"%le\",\n",g);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\"b\" : \"%le\",\n",b);
		buffer_add_string(&buf_json,build);

		sprintf(build,"\t\t\t\"V\" : \"%le\"\n",node->V);
		buffer_add_string(&buf_json,build);

		if (n!=cir->nodes_len-1)
		{
			sprintf(build,"\t\t},\n");
			buffer_add_string(&buf_json,build);
		}else
		{
			sprintf(build,"\t\t}\n");
			buffer_add_string(&buf_json,build);
		}

	}

	sprintf(build,"}\n");
	buffer_add_string(&buf_json,build);

	buffer_dump_path(sim,"",file_name,&buf_json);
	buffer_free(&buf_json);

}

void circuit_dump_I(struct simulation * sim,struct device *dev,char *out_dir)
{
	int l=0;
	int start=0;
	int stop=0;
	double x0=0.0;
	double y0=0.0;
	double z0=0.0;
	double x1=0.0;
	double y1=0.0;
	double z1=0.0;
	double dx=0.0;
	double dy=0.0;
	double dz=0.0;

	long double mul_x=0.0;
	long double mul_y=0.0;
	long double mul_z=0.0;
	long double Vexternal=0.0;
	char temp[200];
	double max_i=0.0;
	struct dat_file buf;
	struct dimensions *dim=&dev->ns.dim;
	struct circuit *cir=&dev->cir;
	struct circuit_link *links=(cir->links);
	struct circuit_node *nodes=(cir->nodes);
	struct circuit_link *link;

	Vexternal=get_equiv_V(sim,dev);

	max_i=circuit_get_max_I(sim,cir);

	if (max_i==0.0)
	{
		max_i=1.0;
	}

	dat_file_init(&buf);

	buffer_malloc(&buf);
	get_meter_dim(buf.x_units,&mul_x,dim->x[dim->xlen-1]);
	get_meter_dim(buf.y_units,&mul_y,dim->y[dim->ylen-1]);
	get_meter_dim(buf.z_units,&mul_z,dim->z[dim->zlen-1]);
	buf.y_mul=mul_y;
	buf.x_mul=mul_x;
	buf.z_mul=mul_z;
	sprintf(buf.title,"%s",_("Curretn vector plot"));
	strcpy(buf.type,"quiver");
	strcpy(buf.x_label,_("x-position"));
	strcpy(buf.y_label,_("y-position"));
	strcpy(buf.z_label,_("z-position"));
	strcpy(buf.data_label,_("Current density"));
	strcpy(buf.data_units,"A m^{-2}");
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=dev->time;
	buf.Vexternal=Vexternal;
	buf.x=1;
	buf.y=cir->links_len;
	buf.z=1;
	buffer_add_info(sim,&buf);

	for (l=0;l<cir->links_len;l++)
	{
		link=&(links[l]);
		start=link->start;
		stop=link->stop;
		x0=nodes[start].x_pos;
		y0=nodes[start].y_pos;
		z0=nodes[start].z_pos;
		x1=nodes[stop].x_pos;
		y1=nodes[stop].y_pos;
		z1=nodes[stop].z_pos;

		dx=(x1-x0)*link->i/max_i;
		dy=(y1-y0)*link->i/max_i;
		dz=(z1-z0)*link->i/max_i;

		if ((fabs(link->i)/max_i)>0.01)
		{
			sprintf(temp,"%le %le %le %le %le %le %le\n",x0,y0,z0,dx,dy,dz,fabs(link->i)/max_i);
			buffer_add_string(&buf,temp);
		}
	}

	buffer_dump_path(sim,out_dir,"J.dat",&buf);
	buffer_free(&buf);
}

void circuit_printf_links(struct simulation * sim,struct circuit *cir)
{
	int n;
	int l;
	struct circuit_link *links=(cir->links);
	struct circuit_node *nodes=(cir->nodes);
	//struct circuit_node *nodes=(cir->nodes);

	printf_log(sim,"links:\n");
	for (l=0;l<cir->links_len;l++)
	{
		printf_log(sim,"%d %c ",l,links[l].type);
		printf_log(sim,"start=%d stop=%d ",links[l].start,links[l].stop);
		printf_log(sim,"R=%.1le n0=%.1le ",links[l].R,links[l].n0);
		printf_log(sim,"I0=%.1le I=%.1le\n",links[l].I0 ,links[l].i);
	}

	printf_log(sim,"nodes:\n");
	for (n=0;n<cir->nodes_len;n++)
	{
		printf_log(sim,"%d: (%d,%d,%d) V=%lf type=%d\n",n,nodes[n].z,nodes[n].x,nodes[n].y,nodes[n].V,nodes[n].type);

	}
}

void circuit_plot_resistance(struct simulation * sim,struct circuit *cir,struct device *dev)
{
	int x=0;
	int y=0;
	int z=0;
	//int n=0;
	//int band=0;
	int l=0;
	long double mul_z;
	long double mul_x;
	long double mul_y;
	char line[200];
	struct dat_file buf;

	//struct circuit_node *node;
	//struct circuit_link *link;
	struct dimensions *dim=&(dev->ns.dim);
	//struct circuit_node *nodes=cir->nodes;
	struct circuit_link *links=(cir->links);

	dat_file_init(&buf);

	buffer_malloc(&buf);
	get_meter_dim(buf.x_units,&mul_x,dim->x[dim->xlen-1]);
	get_meter_dim(buf.y_units,&mul_y,dim->y[dim->ylen-1]);
	get_meter_dim(buf.z_units,&mul_z,dim->z[dim->zlen-1]);
	buf.y_mul=mul_y;
	buf.x_mul=mul_x;
	buf.z_mul=mul_z;
	strcpy(buf.title,_("Resistance"));
	strcpy(buf.type,"zxy-d");
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.data_label,_("Resistance"));
	strcpy(buf.data_units,"Ohmsm^{-2}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=dim->xlen;
	buf.y=dim->ylen-1;
	buf.z=dim->zlen;
	buffer_add_info(sim,&buf);

	for(z=0;z<dim->zlen;z++)
	{
		for(x=0;x<dim->xlen;x++)
		{

			for (y=0;y<dim->ylen-1;y++)
			{
				l=circuit_find_link(sim,cir,z,x,y, z,x,y+1);
				sprintf(line,"%Le %Le %Le %e\n",dim->z[z],dim->x[x],dim->y[y],links[l].R);
				buffer_add_string(&buf,line);
			}
		}
	}

	buffer_dump_path(sim,"","resistance.dat",&buf);
	buffer_free(&buf);

}

void circuit_dump_I_layers(struct simulation * sim,struct device *dev,char *out_dir)
{
	int x=0;
	int y=0;
	int z=0;
	//int n=0;
	//int band=0;
	int l=0;
	long double mul_z;
	long double mul_x;
	long double mul_y;
	char line[200];
	struct dat_file buf;
	char temp[200];
	//struct circuit_node *node;
	//struct circuit_link *link;
	struct dimensions *dim=&(dev->ns.dim);
	//struct circuit_node *nodes=cir->nodes;
	struct circuit *cir=&dev->cir;
	struct circuit_link *links=(cir->links);
	double val;
	dat_file_init(&buf);

	for (y=0;y<dim->ylen-1;y++)
	{
		buffer_malloc(&buf);
		get_meter_dim(buf.x_units,&mul_x,dim->x[dim->xlen-1]);
		get_meter_dim(buf.y_units,&mul_y,dim->y[dim->ylen-1]);
		get_meter_dim(buf.z_units,&mul_z,dim->z[dim->zlen-1]);
		buf.y_mul=mul_y;
		buf.x_mul=mul_x;
		buf.z_mul=mul_z;
		strcpy(buf.title,_("Resistance"));
		strcpy(buf.type,"3d");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.data_label,_("Resistance"));
		strcpy(buf.data_units,"Ohmsm^{-2}");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.x=dim->xlen;
		buf.y=dim->ylen-1;
		buf.z=1;
		buffer_add_info(sim,&buf);
		for(z=0;z<dim->zlen;z++)
		{
			for(x=0;x<dim->xlen;x++)
			{
				l=circuit_find_link(sim,cir,z,x,y, z,x,y+1);
				if (l==-1)
				{
					val=0.0;
				}else
				{
					val=links[l].i;
				}
				sprintf(line,"%Le %Le %e\n",dim->z[z],dim->x[x],val);
				buffer_add_string(&buf,line);
			}
			buffer_add_string(&buf,"\n");
		}

		sprintf(temp,"I_%d.dat",y);
		buffer_dump_path(sim,out_dir,temp,&buf);
		buffer_free(&buf);
	}


}
