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

void circuit_init(struct circuit *cir)
{
	cir->nodes_len=-1;
	cir->links_len=-1;
	cir->nodes=NULL;
	cir->links=NULL;
	cir->nodes_max=-1;
	cir->links_max=-1;
	matrix_init(&(cir->mx));

	cir->nodes_len=0;
	cir->links_len=0;
	cir->unknowns=0;
	cir->nodes_max=-1;//1000;
	cir->links_max=-1;//1000;
	cir->nodes=NULL;//malloc(sizeof(struct circuit_node)*cir->nodes_max);
	cir->links=NULL;//malloc(sizeof(struct circuit_link)*cir->links_max);
	cir->config_nlines=-1;
	cir->config_lines=NULL;
	cir->abstract_circuit=FALSE;
	cir->enabled=FALSE;
	matrix_solver_memory_init(&(cir->msm));
	cir->quite=FALSE;
}


