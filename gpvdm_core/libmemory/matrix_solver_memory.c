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

/** @file memory.c
@brief get/free memory
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lang.h>
#include "sim.h"
#include "dump.h"
#include "mesh.h"
#include <math.h>
#include "log.h"
#include <solver_interface.h>
#include <circuit.h>
#include "memory.h"
#include "ray_fun.h"
#include "newton_tricks.h"
#include "shape.h"

void matrix_solver_memory_init(struct matrix_solver_memory *msm)
{
		//Matrix solver -	external dll
	msm->last_col=0;
	msm->last_nz=0;
	msm->x=NULL;
	msm->Ap=NULL;
	msm->Ai=NULL;
	msm->Ax=NULL;
	msm->b=NULL;
	msm->Tx=NULL;

	//Complex matrix solver -	external dll
	msm->c_last_col=0;
	msm->c_last_nz=0;
	msm->c_x=NULL;
	msm->c_xz=NULL;
	msm->c_Ap=NULL;
	msm->c_Ai=NULL;
	msm->c_Ax=NULL;
	msm->c_Az=NULL;
	msm->c_b=NULL;
	msm->c_bz=NULL;
	msm->c_Tx=NULL;
	msm->c_Txz=NULL;

	msm->fd_ext_solver=-1;
	msm->ext_solver_buf_size=-1;
	msm->ext_solver_buf=NULL;
	strcpy(msm->ext_solver_pipe_name,"");
	strcpy(msm->fname_from_solver,"");
	strcpy(msm->fname_to_solver,"");
	msm->fd_from_solver=-1;

	msm->x_matrix_offset=0;
}

void matrix_solver_memory_free(struct matrix_solver_memory *msm)
{

	//Real part
	if (msm->x!=NULL)
	{
		free(msm->x);
		msm->x=NULL;	
	}

	if (msm->Ap!=NULL)
	{
		free(msm->Ap);
		msm->Ap=NULL;	
	}

	if (msm->Ai!=NULL)
	{
		free(msm->Ai);
		msm->Ai=NULL;	
	}

	if (msm->Ax!=NULL)
	{
		free(msm->Ax);
		msm->Ax=NULL;	
	}

	if (msm->b!=NULL)
	{
		free(msm->b);
		msm->b=NULL;	
	}

	if (msm->Tx!=NULL)
	{
		free(msm->Tx);
		msm->Tx=NULL;	
	}

	//Complex part
	if (msm->c_x!=NULL)
	{
		free(msm->c_x);
		msm->c_x=NULL;	
	}

	if (msm->c_xz!=NULL)
	{
		free(msm->c_xz);
		msm->c_xz=NULL;	
	}

	if (msm->c_b!=NULL)
	{
		free(msm->c_b);
		msm->c_b=NULL;	
	}

	if (msm->c_bz!=NULL)
	{
		free(msm->c_bz);
		msm->c_bz=NULL;	
	}

	if (msm->c_Ap!=NULL)
	{
		free(msm->c_Ap);
		msm->c_Ap=NULL;
	}

	if (msm->c_Ai!=NULL)
	{
		free(msm->c_Ai);
		msm->c_Ai=NULL;
	}

	if (msm->c_Ax!=NULL)
	{
		free(msm->c_Ax);
		msm->c_Ax=NULL;
	}

	if (msm->c_Az!=NULL)
	{
		free(msm->c_Az);
		msm->c_Az=NULL;
	}

	if (msm->c_Tx!=NULL)
	{
		free(msm->c_Tx);
		msm->c_Tx=NULL;
	}

	if (msm->c_Txz!=NULL)
	{
		free(msm->c_Txz);
		msm->c_Txz=NULL;
	}

	msm->c_last_col=0;
	msm->c_last_nz=0;

	msm->last_col=0;
	msm->last_nz=0;
}

