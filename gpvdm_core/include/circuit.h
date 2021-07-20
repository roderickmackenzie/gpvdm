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

#ifndef circuit_h
#define circuit_h
#include <sim_struct.h>
#include <circuit_struct.h>

void circuit_init(struct circuit *cir);
void circuit_malloc(struct simulation * sim,struct circuit *cir);
void circuit_free(struct simulation * sim,struct circuit *cir);
void circuit_load(struct simulation * sim,struct circuit *cir);
void circuit_cpy(struct simulation * sim,struct circuit *out,struct circuit *in);
void circuit_alloc_nodes_and_links(struct simulation * sim,struct circuit *cir);
void circuit_build_device(struct simulation * sim,struct circuit *cir,struct device *dev);
void circuit_solve(struct simulation * sim,struct circuit *cir,struct device *dev);
void circuit_apply_voltages(struct simulation * sim,struct circuit *cir,struct device *dev);
void circuit_transfer_to_electrical_mesh(struct simulation * sim,struct circuit *cir,struct device *dev);
double circuit_node_get_I(struct simulation * sim,struct circuit *cir,int n);
struct circuit_node *circuit_add_node(struct simulation * sim,struct circuit *cir,int x,int y,int z,int type);
int circuit_find_node_by_xyz(struct simulation * sim,struct circuit *cir,int x,int y,int z);
void circuit_node_set_type(struct simulation * sim, struct circuit_node *node, int type);
void circuit_print_nodes(struct simulation * sim,struct circuit *cir);
void circuit_print_links(struct simulation * sim,struct circuit *cir);
struct circuit_link *circuit_add_link(struct simulation * sim,struct circuit *cir,struct circuit_link *in_link);
int circuit_load_config(struct simulation * sim,struct circuit *cir,struct json_obj *json_circuit,struct epitaxy *epi);
void circuit_time_step(struct simulation * sim,struct circuit *cir);
void circuit_apply_photo_generation(struct simulation * sim,struct circuit *cir,struct device *dev);

void circuit_cal_resistance(struct simulation * sim,struct circuit *cir,struct device *dev);
void circuit_plot_resistance(struct simulation * sim,struct circuit *cir,struct device *dev);
int circuit_find_link(struct simulation * sim,struct circuit *cir,int z0,int x0,int y0, int z1,int x1,int y1);
double circuit_get_max_I(struct simulation * sim,struct circuit *cir);
void circuit_calculate_matrix_pos(struct simulation * sim,struct circuit *cir);
void circuit_cal_device_resistance(struct simulation * sim,struct device *dev);
int circuit_cal_max_y_node_level(struct simulation * sim,struct circuit *cir,struct device *dev);
int link_get_other_end(struct circuit *cir,struct circuit_link *link,int node);
void circuit_build_and_solve_matrix(struct simulation * sim,struct circuit *cir,struct device *dev, int dump_number);
void circuit_spm(struct simulation * sim,struct device *dev,long double x0,long double x1,long double z0,long double z1);
void circuit_malloc_matrix(struct simulation * sim,struct circuit *cir);

//dump
void circuit_dump_to_obj_file(struct simulation * sim,char *file_name,struct device *dev,struct circuit *cir);
void circuit_dump(struct simulation * sim,struct device *dev,struct circuit *cir);
void circuit_dump_I(struct simulation * sim,struct device *dev,char *out_dir);
void circuit_printf_links(struct simulation * sim,struct circuit *cir);

//nodes
void circuit_nodes_select(struct simulation * sim,struct circuit *cir,int value);
struct circuit_node * circuit_nodes_find_min_xyz(struct simulation * sim,struct circuit *cir);
void circuit_nodes_flood_fill(struct simulation * sim,struct circuit *cir,int node_index);
struct circuit_node * circuit_nodes_selected_find_maxx_maxy_maxz(struct simulation * sim,struct circuit *cir);
struct circuit_node * circuit_nodes_find_node_x_plus_one(struct simulation * sim,struct circuit *cir,struct circuit_node *base_node);
struct circuit_node * circuit_nodes_selected_find_miny(struct simulation * sim,struct circuit *cir,struct circuit_node *base_node);
void circuit_dump_snapshot(struct simulation * sim,struct device *dev,char *out_dir);
void circuit_dump_I_layers(struct simulation * sim,struct device *dev,char *out_dir);
#endif
