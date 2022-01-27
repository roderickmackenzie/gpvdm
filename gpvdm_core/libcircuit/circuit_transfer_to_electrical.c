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

/** @file circuit_transfer_to_electrical.c
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



void circuit_transfer_to_electrical_mesh(struct simulation * sim,struct circuit *cir,struct device *dev)
{
	int x=0;
	int y=0;
	int z=0;
	int n=0;
	//int band=0;
	long double A;

	struct circuit_node *node;
	struct dimensions *dim=&(dev->ns.dim);
	struct circuit_node *nodes=cir->nodes;

	for(z=0;z<dim->zlen;z++)
	{
		for(x=0;x<dim->xlen;x++)
		{
			dev->Jn_y0[z][x]=0.0;
			dev->Jp_y0[z][x]=0.0;
			dev->Jn_y1[z][x]=0.0;
			dev->Jp_y1[z][x]=0.0;
		}
	}

	if (cir->abstract_circuit==FALSE)
	{
		for (n=0;n<cir->nodes_len;n++)
		{
			node=&(nodes[n]);
			if ((node->type==CIR_CHANGE_X)||(node->type==CIR_KNOWN))
			{

				if (node->y==0)
				{
					A=dim->dX[node->x]*dim->dZ[node->z];
					dev->Jn_y0[node->z][node->x]=-circuit_node_get_I(sim,cir,n)/A;
					dev->Jp_y0[node->z][node->x]=0.0;
				}else
				{
					long double current=circuit_node_get_I(sim,cir,n);
					//printf("%d %d %d %Le %le\n",node->z,node->x,node->y,current,node->V);
					A=dim->dX[node->x]*dim->dZ[node->z];
					dev->Jn_y1[node->z][node->x]=0.0;
					dev->Jp_y1[node->z][node->x]=current/A;
				}

			}
		}
		//getchar();
	}else
	{
		for (n=0;n<cir->nodes_len;n++)
		{
			node=&(nodes[n]);
			if (node->type==CIR_CHANGE_X)
			{
				for(z=0;z<dim->zlen;z++)
				{
					for(x=0;x<dim->xlen;x++)
					{
						A=dim->dX[x]*dim->dZ[z];
						dev->Jn_y0[z][x]=-circuit_node_get_I(sim,cir,n)/A;
						dev->Jp_y0[z][x]=0.0;
					}
				}
			}else
			if (node->type==CIR_KNOWN)
			{
				for(z=0;z<dim->zlen;z++)
				{
					for(x=0;x<dim->xlen;x++)
					{
						A=dim->dX[x]*dim->dZ[z];
						dev->Jn_y1[z][x]=0.0;
						dev->Jp_y1[z][x]=circuit_node_get_I(sim,cir,n)/A;
					}
				}
			}
		}
	}

	/*FILE *file=fopen("out.dat","a");
	if (file!=NULL)
	{
		fprintf(file,"%Le %Le\n",contact_get_voltage(sim,dev,0),get_J(dev));
		fclose(file);
	}*/

	//printf("%Le %Le\n",contacts_get_Jleft(dev),contacts_get_Jright(dev));
	//circuit_printf_links(sim,cir);
	//getchar();
	
	for(z=0;z<dim->zlen;z++)
	{
		for(x=0;x<dim->xlen;x++)
		{

			for (y=0;y<dim->ylen;y++)
			{

				/*for (band=0;band<dim->srh_bands;band++)
				{

					dev->nt_r1[z][x][y][band]=0.0;
					dev->nt_r2[z][x][y][band]=0.0;
					dev->nt_r3[z][x][y][band]=0.0;
					dev->nt_r4[z][x][y][band]=0.0;

					dev->pt_r1[z][x][y][band]=0.0;
					dev->pt_r2[z][x][y][band]=0.0;
					dev->pt_r3[z][x][y][band]=0.0;
					dev->pt_r4[z][x][y][band]=0.0;

				}*/

				dev->Jn[z][x][y]=0.0;
				dev->Jp[z][x][y]=0.0;

				dev->Jn_x[z][x][y]=0.0;
				dev->Jp_x[z][x][y]=0.0;

				/*dev->nrelax[z][x][y]=0.0;
				dev->ntrap_to_p[z][x][y]=0.0;
				dev->prelax[z][x][y]=0.0;
				dev->ptrap_to_n[z][x][y]=0.0;

				dev->Rn[z][x][y]=0.0;
				dev->Rp[z][x][y]=0.0;

				dev->Rn_srh[z][x][y]=0.0;
				dev->Rp_srh[z][x][y]=0.0;*/

			}


		}
	}

	
	//dump_zx(sim,dev,"none");
	//circuit_dump_to_obj_file(sim,"circuit_voltages.dat",dev,cir);
}



