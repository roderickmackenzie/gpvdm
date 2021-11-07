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

void circuit_cpy(struct simulation * sim,struct circuit *out,struct circuit *in)
{
	out->nodes_len=in->nodes_len;
	out->links_len=in->links_len;
	out->nodes_max=in->nodes_max;
	out->links_max=in->links_max;
	out->unknowns=in->unknowns;
	matrix_cpy(sim,&(out->mx),&(in->mx));

	//printf("n=%ld\n",in->nodes_max*sizeof(struct circuit_node));
	//printf("l=%ld\n",in->links_max* sizeof(struct circuit_link));
	cpy_1d_alloc((void **)(&out->nodes), (void **)(&in->nodes), in->nodes_max, sizeof(struct circuit_node));

	cpy_1d_alloc((void **)(&out->links), (void **)(&in->links), in->links_max, sizeof(struct circuit_link));

	out->config_nlines=in->config_nlines;
	//printf("c=%ld\n",in->config_nlines* sizeof(struct circuit_config_line));
	cpy_1d_alloc((void **)(&out->config_lines), (void **)(&in->config_lines), in->config_nlines, sizeof(struct circuit_config_line));

	out->abstract_circuit=in->abstract_circuit;		//The circuit does not follow a device structure
	out->enabled=in->enabled;
	matrix_solver_memory_init(&(out->msm));

}

