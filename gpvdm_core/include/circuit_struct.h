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
