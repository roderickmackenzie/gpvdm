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

/** @file dump_energy_slice.c
@brief dump an energy slice, code needs updating
*/

#include <string.h>
#include <stdio.h>
#include <dos.h>
#include "sim.h"
#include "dump.h"
#include "dat_file.h"

void dump_energy_slice(struct simulation *sim,struct device *in,int x, int y, int z)
{
	struct dat_file buf;
	dat_file_init(&buf);
	char out_dir[PATH_MAX];
	struct snapshots snap;
	struct shape *s;
	int band=0;

	snapshots_init(&snap);
	strcpy(snap.type,"snapshots");
	strcpy(snap.plot_type,"2d");
	strcpy(snap.name,"trap_dist");
	strcpy(snap.path,in->output_path);
	dump_make_snapshot_dir(sim,out_dir, in->snapshot_number,&snap);

	struct dimensions *dim=&in->ns.dim;
	//struct epitaxy *epi=&(in->my_epitaxy);

	if ((y>=dim->ylen)||(x>=dim->xlen)||(z>=dim->zlen))
	{
		return;
	}

	struct math_xy dump1;
	inter_init(sim,&dump1);

	struct math_xy dump2;
	inter_init(sim,&dump2);

	struct math_xy dump3;
	inter_init(sim,&dump3);

	struct math_xy dump4;
	inter_init(sim,&dump4);

	struct math_xy dump5;
	inter_init(sim,&dump5);

	struct math_xy dump6;
	inter_init(sim,&dump6);

	struct math_xy dump9;
	inter_init(sim,&dump9);

	struct math_xy dump10;
	inter_init(sim,&dump10);

	struct math_xy dump_nt;
	inter_init(sim,&dump_nt);

	struct math_xy dump_pt;
	inter_init(sim,&dump_pt);

	s=in->obj_zxy[z][x][y]->s;
	for (band=dim->srh_bands-1;band>=0;band--)
	{
		inter_append(&dump1,get_dos_E_n(s,band)-in->Xi[z][x][y],in->n[z][x][y]*in->srh_n_r1[z][x][y][band]-in->srh_n_r2[z][x][y][band]);
		inter_append(&dump2,-get_dos_E_p(s,band)-in->Xi[z][x][y]-in->Eg[z][x][y],in->p[z][x][y]*in->srh_p_r1[z][x][y][band]-in->srh_p_r2[z][x][y][band]);
		inter_append(&dump3,get_dos_E_n(s,band)-in->Xi[z][x][y],in->nt[z][x][y][band]-in->ntb_save[z][x][y][band]);
		inter_append(&dump4,-get_dos_E_p(s,band)-in->Xi[z][x][y]-in->Eg[z][x][y],in->pt[z][x][y][band]-in->ptb_save[z][x][y][band]);
		inter_append(&dump5,get_dos_E_n(s,band)-in->Xi[z][x][y],in->p[z][x][y]*in->srh_n_r3[z][x][y][band]-in->srh_n_r4[z][x][y][band]);
		inter_append(&dump6,-get_dos_E_p(s,band)-in->Xi[z][x][y]-in->Eg[z][x][y],in->n[z][x][y]*in->srh_p_r3[z][x][y][band]-in->srh_p_r4[z][x][y][band]);
		inter_append(&dump9,get_dos_E_n(s,band)-in->Xi[z][x][y],in->Fnt[z][x][y][band]);
		inter_append(&dump10,-get_dos_E_p(s,band)-in->Xi[z][x][y]-in->Eg[z][x][y],in->Fpt[z][x][y][band]);
		inter_append(&dump_nt,get_dos_E_n(s,band)-in->Xi[z][x][y],in->nt[z][x][y][band]);
		inter_append(&dump_pt,-get_dos_E_p(s,band)-in->Xi[z][x][y]-in->Eg[z][x][y],in->pt[z][x][y][band]);

	}
	//getchar();

	//sprintf(outpath,"%senergy_slice_nt_cap_%s",out_dir,".dat");
	//inter_save(&dump1,outpath);
	inter_free(&dump1);

	//sprintf(outpath,"%senergy_slice_pt_cap_%s",out_dir,".dat");
	//inter_save(&dump2,outpath);
	inter_free(&dump2);


	//sprintf(outpath,"%senergy_slice_nt_delta_%s",out_dir,".dat");
	//inter_save(&dump3,outpath);
	inter_free(&dump3);

	//sprintf(outpath,"%senergy_slice_pt_delta_%s",out_dir,".dat");
	//inter_save(&dump4,outpath);
	inter_free(&dump4);


	//sprintf(outpath,"%senergy_slice_nt_recom_%s",out_dir,".dat");
	//inter_save(&dump5,outpath);
	inter_free(&dump5);

	//sprintf(outpath,"%senergy_slice_pt_recom_%s",out_dir,".dat");
	//inter_save(&dump6,outpath);
	inter_free(&dump6);

	//sprintf(outpath,"%senergy_slice_fn_%s",out_dir,".dat");
	//inter_save(&dump9,outpath);
	inter_free(&dump9);

	//sprintf(outpath,"%senergy_slice_fp_%s",out_dir,".dat");
	//inter_save(&dump10,outpath);
	inter_free(&dump10);

	if (buffer_set_file_name(sim,in,&buf,"energy_slice_nt.csv")==0)
	{
		buffer_malloc(&buf);
		strcpy(buf.title,"Energy - trap ocupation");
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,"Energy");
		strcpy(buf.data_label,"Ocupation");
		strcpy(buf.y_units,"eV");
		strcpy(buf.data_units,"m^{-3} eV^{-1}");
		buf.logscale_x=0;
		buf.logscale_y=1;
		dat_file_add_xy_data(sim,&buf,dump_nt.x, dump_nt.data, dump_nt.len);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"energy_slice_pt.csv")==0)
	{
		buffer_malloc(&buf);
		strcpy(buf.title,"Energy - trap ocupation");
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,"Energy");
		strcpy(buf.data_label,"Ocupation");
		strcpy(buf.y_units,"eV");
		strcpy(buf.data_units,"m^{-3} eV^{-1}");
		buf.logscale_x=0;
		buf.logscale_y=1;
		dat_file_add_xy_data(sim,&buf,dump_pt.x, dump_pt.data, dump_pt.len);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

	inter_free(&dump_nt);
	inter_free(&dump_pt);

}
