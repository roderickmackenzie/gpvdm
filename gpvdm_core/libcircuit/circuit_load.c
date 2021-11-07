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

int circuit_load_config_lines(struct simulation * sim,struct circuit *cir,struct json_obj *json_circuit)
{
	int i;
	int enabled;
	char temp[100];
	cir->config_nlines=0;
	struct json_obj *json_circuit_diagram;
	struct json_obj *json_component;

	struct circuit_config_line * lines=NULL;

	json_get_english(sim,json_circuit, &(enabled),"enabled");

	if (enabled==FALSE)
	{
		return -1;
	}

	json_circuit_diagram=json_obj_find(json_circuit, "circuit_diagram");

	json_get_int(sim,json_circuit_diagram, &(cir->config_nlines),"segments");

	cir->config_lines=malloc(sizeof(struct circuit_config_line)*cir->config_nlines);
	lines=cir->config_lines;

	for (i=0;i<cir->config_nlines;i++)
	{
		sprintf(temp,"segment%d",i);
		json_component=json_obj_find(json_circuit_diagram, temp);
		if (json_component==NULL)
		{
			ewe(sim,"Can not find %s\n",temp);
		}
		//printf("%s %d\n",temp,cir->config_nlines);
		json_get_string(sim,json_component, lines[i].component,"name");
		json_get_string(sim,json_component,lines[i].layer_name,"layer");

		json_get_int(sim,json_component, &lines[i].x0,"x0");
		json_get_int(sim,json_component, &lines[i].y0,"y0");
		lines[i].z0=0;

		json_get_int(sim,json_component, &lines[i].x1,"x1");
		json_get_int(sim,json_component, &lines[i].y1,"y1");
		lines[i].z1=0;

		json_get_double(sim,json_component, &lines[i].R,"R");
		json_get_double(sim,json_component, &lines[i].C,"C");
		json_get_double(sim,json_component, &lines[i].L,"L");
		json_get_double(sim,json_component, &lines[i].nid,"nid");
		json_get_double(sim,json_component, &lines[i].I0,"I0");
		json_get_double(sim,json_component, &lines[i].Dphotoneff,"Dphotoneff");
	}

	cir->abstract_circuit=TRUE;

	return 0;
}

void circuit_config_add_nodes(struct simulation * sim,struct circuit *cir)
{
	int l=0;
	int n=0;
	struct circuit_node *node;
	struct circuit_config_line * line;
	struct circuit_config_line * lines=cir->config_lines;

	//Add nodes for components
	for (l=0;l<cir->config_nlines;l++)
	{
		line=&(cir->config_lines[l]);
		if (strcmp(line->component,"resistor")==0)
		{
			circuit_add_node(sim,cir,line->x0,line->y0,line->z0,CIR_UNKNOWN);
			circuit_add_node(sim,cir,line->x1,line->y1,line->z1,CIR_UNKNOWN);
		}

		if (strcmp(lines[l].component,"diode")==0)
		{
			circuit_add_node(sim,cir,line->x0,line->y0,line->z0,CIR_UNKNOWN);
			circuit_add_node(sim,cir,line->x1,line->y1,line->z1,CIR_UNKNOWN);
		}

		if (strcmp(lines[l].component,"inductor")==0)
		{
			circuit_add_node(sim,cir,line->x0,line->y0,line->z0,CIR_UNKNOWN);
			circuit_add_node(sim,cir,line->x1,line->y1,line->z1,CIR_UNKNOWN);
		}

		if (strcmp(lines[l].component,"capacitor")==0)
		{
			circuit_add_node(sim,cir,line->x0,line->y0,line->z0,CIR_UNKNOWN);
			circuit_add_node(sim,cir,line->x1,line->y1,line->z1,CIR_UNKNOWN);
		}

	}

	//Go back through and search for sources and grounds
	for (l=0;l<cir->config_nlines;l++)
	{
		line=&(cir->config_lines[l]);
		if (strcmp(line->component,"bat")==0)
		{
			n=circuit_find_node_by_xyz(sim,cir,line->x0,line->y0,line->z0);
			if (n!=-1)
			{
				node=&(cir->nodes[n]);
				circuit_node_set_type(sim, node, CIR_CHANGE_X);
			}

			n=circuit_find_node_by_xyz(sim,cir,line->x1,line->y1,line->z1);
			if (n!=-1)
			{
				node=&(cir->nodes[n]);
				circuit_node_set_type(sim, node, CIR_CHANGE_X);
			}
		}

		if (strcmp(lines[l].component,"ground")==0)
		{

			n=circuit_find_node_by_xyz(sim,cir,line->x0,line->y0,line->z0);
			if (n!=-1)
			{
				node=&(cir->nodes[n]);
				circuit_node_set_type(sim, node, CIR_KNOWN);
				node->V=0.0;
			}

			n=circuit_find_node_by_xyz(sim,cir,line->x1,line->y1,line->z1);
			if (n!=-1)
			{
				node=&(cir->nodes[n]);
				circuit_node_set_type(sim, node, CIR_KNOWN);
				node->V=0.0;
			}
		}


	}
}


void circuit_config_shorten_link(struct simulation * sim,struct circuit *cir,int x, int y,int z,int x_new, int y_new,int z_new)
{
	int l;
	struct circuit_config_line * line;
	for (l=0;l<cir->config_nlines;l++)
	{
		line=&(cir->config_lines[l]);
		if (strcmp(line->component,"none")!=0)
		{
			if ((line->x0==x)&&(line->y0==y)&&(line->z0==z))
			{
					line->x0=x_new;
					line->y0=y_new;
					line->z0=z_new;
			}

			if ((line->x1==x)&&(line->y1==y)&&(line->z1==z))
			{
					line->x1=x_new;
					line->y1=y_new;
					line->z1=z_new;
			}

		}
	}
}

void circuit_config_remove_wires(struct simulation * sim,struct circuit *cir)
{
	int l;
	struct circuit_config_line * line;
	for (l=0;l<cir->config_nlines;l++)
	{
		line=&(cir->config_lines[l]);

		if (strcmp(line->component,"wire")==0)
		{
			strcpy(line->component,"none");
			circuit_config_shorten_link(sim,cir,line->x0, line->y0,line->z0,line->x1, line->y1,line->z1);
			//printf("%s\n",line->component);

		}
	}
}

void circuit_config_add_links(struct simulation * sim,struct circuit *cir,struct epitaxy *epi)
{
	int start=0;
	int stop=0;
	int add=FALSE;
	int l=0;
	int la=0;
	struct circuit_link local_link;
	struct circuit_config_line * line;

	//Now add in the links
	for (l=0;l<cir->config_nlines;l++)
	{
		line=&(cir->config_lines[l]);
		add=FALSE;
		local_link.layer=-1;
		local_link.x=0;
		local_link.z=0;
		local_link.enable_Isc=FALSE;
		if (strcmp(line->component,"resistor")==0)
		{
			add=TRUE;
			local_link.type='R';
			line->nid=0.0;		//Clean up
			line->I0=0.0;

		}else
		if (strcmp(line->component,"diode")==0)
		{
			add=TRUE;
			local_link.type='D';
			line->R=0.0;
			if (strcmp(line->layer_name,"none")!=0)
			{
				for (la=0;la<epi->layers;la++)
				{
					if (strcmp(epi->layer[la].s.name,line->layer_name)==0)
					{
						local_link.layer=la;
						break;
					}
				}
			}
		}else
		if (strcmp(line->component,"inductor")==0)
		{
			add=TRUE;
			local_link.type='I';
		}else
		if (strcmp(line->component,"capacitor")==0)
		{
			add=TRUE;
			local_link.type='C';
		}

		if (add==TRUE)
		{
			start=circuit_find_node_by_xyz(sim,cir,line->x0,line->y0,line->z0);
			stop=circuit_find_node_by_xyz(sim,cir,line->x1,line->y1,line->z1);
			if ((start!=-1)&&(stop!=-1))
			{
				local_link.start=start;
				local_link.stop=stop;
				local_link.R=fabs(line->R);
				local_link.C=fabs(line->C);
				local_link.L=fabs(line->L);
				local_link.n0=fabs(line->nid);
				local_link.I0=fabs(line->I0);
				local_link.Isc=0.0;
				local_link.enable_Isc=FALSE;
				local_link.Dphotoneff=fabs(line->Dphotoneff);
				if ((local_link.type=='D')&&(line->Dphotoneff>0.0))
				{
					local_link.enable_Isc=TRUE;
				}
				if (cir->abstract_circuit==TRUE)
				{
					local_link.dl=1.0;
				}

				circuit_add_link(sim,cir,&local_link);
			}
			//printf("%d\n",local_link.layer);
		}

	}
	//getchar();
}

int circuit_load_config(struct simulation * sim,struct circuit *cir,struct json_obj *json_circuit_diagram,struct epitaxy *epi)
{
	cir->links_len=0;
	cir->abstract_circuit=FALSE;
	if (circuit_load_config_lines(sim,cir,json_circuit_diagram)==0)
	{
		circuit_config_remove_wires(sim,cir);
		circuit_config_add_nodes(sim,cir);
		circuit_config_add_links(sim,cir,epi);
		free_1d( (void **)(&cir->config_lines), sizeof(struct circuit_config_line));

		circuit_print_nodes(sim,cir);
		circuit_print_links(sim,cir);

		//printf("Circuit load unknowns=%d\n",cir->unknowns);
		return 0;
	}

	return -1;
}



