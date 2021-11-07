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
#include <circuit.h>
#include <matrix_solver_memory.h>

void circuit_alloc_nodes_and_links(struct simulation * sim,struct circuit *cir)
{
	cir->nodes_max=1000;
	cir->links_max=1000;
	malloc_1d((void **)&(cir->nodes),cir->nodes_max, sizeof(struct circuit_node));
	malloc_1d((void **)&(cir->links),cir->links_max, sizeof(struct circuit_link));
	//cir->nodes=malloc(sizeof(struct circuit_node)*cir->nodes_max);
	//cir->links=malloc(sizeof(struct circuit_link)*cir->links_max);
}

void circuit_malloc_matrix(struct simulation * sim,struct circuit *cir)
{
	cir->mx.M=cir->unknowns;		//col
	cir->mx.nz=1000;		//col
	cir->mx.complex_matrix=FALSE;
	cir->mx.build_from_non_sparse=TRUE;
	matrix_malloc(sim,&cir->mx);
	cir->mx.nz=0;		//col
	cir->mx.nz_max=1000;		//col
}

void circuit_malloc(struct simulation * sim,struct circuit *cir)
{
	matrix_solver_memory_load_dll(sim,&(cir->msm));
	circuit_malloc_matrix(sim,cir);
}

void circuit_free(struct simulation * sim,struct circuit *cir)
{
	if (cir->nodes!=NULL)
	{
		free(cir->nodes);
		cir->nodes=NULL;
	}

	if (cir->links!=NULL)
	{
		free(cir->links);
		cir->links=NULL;
	}
	matrix_solver_memory_free(sim,&(cir->msm));
	matrix_free(sim,&(cir->mx));

}

