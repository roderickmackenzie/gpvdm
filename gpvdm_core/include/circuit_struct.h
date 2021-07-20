// 
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// The model can simulate OLEDs, Perovskite cells, and OFETs.
// 
// Copyright (C) 2008-2020 Roderick C. I. MacKenzie
// 
// https://www.gpvdm.com
// r.c.i.mackenzie at googlemail.com
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the GPVDM nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL Roderick C. I. MacKenzie BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

/** @file circuit.h
@brief Header files for nodal analysis
*/

#ifndef circuit_struct_h
#define circuit_struct_h
#include <sim_struct.h>
#include <matrix_solver_memory.h>

#define CIR_KNOWN 0
#define CIR_UNKNOWN 1
#define CIR_CHANGE_X 2

struct circuit_config_line
{
	char component[100];
	int x0;
	int y0;
	int z0;
	int x1;
	int y1;
	int z1;
	double R;
	double C;
	double L;
	double nid;
	double I0;
	double Dphotoneff;
	char layer_name[100];
};

struct circuit_link
{
	int start;
	int stop;
	char type;
	double L;
	double C;
	double R;
	double I0;
	double Isc;
	int enable_Isc;
	double n0;
	double i;
	double Dphotoneff;
	//For 3d electrical not use for the calculation
	//double J0;
	//double Jsc;
	//double j;
	double dl;		//Length between nodes
	//double rho;

	//only used for diodes and to figure out where they are.
	int layer;
	int x;
	int z;
	int id;

};

struct circuit_node
{
	double V;
	double V_last;
	char type;
	int matrix_pos;
	int z;
	int x;
	int y;

	double z_pos;
	double x_pos;
	double y_pos;

	int links[10];
	int nlinks;
	char selected;

	int node_index;
};

struct circuit
{
	int nodes_len;
	int links_len;
	int nodes_max;
	int links_max;
	int unknowns;
	struct matrix mx;
	struct circuit_node *nodes;
	struct circuit_link *links;
	int config_nlines;
	struct circuit_config_line * config_lines;
	int abstract_circuit;		//The circuit does not follow a device structure
	int enabled;
	struct matrix_solver_memory msm;
	int quite;
};

#endif
