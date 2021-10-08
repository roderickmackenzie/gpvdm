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

/** @file checksum.c
	@brief OpenCL worker function.
*/

#include <math.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <inp.h>
#include <sim.h>
#include <log.h>
#include <fdtd.h>

#include "vec.h"

const char *getErrorString(int error)
{
#ifdef use_open_cl
switch(error){
    // run-time and JIT compiler errors
    case 0: return "CL_SUCCESS";
    case -1: return "CL_DEVICE_NOT_FOUND";
    case -2: return "CL_DEVICE_NOT_AVAILABLE";
    case -3: return "CL_COMPILER_NOT_AVAILABLE";
    case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5: return "CL_OUT_OF_RESOURCES";
    case -6: return "CL_OUT_OF_HOST_MEMORY";
    case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8: return "CL_MEM_COPY_OVERLAP";
    case -9: return "CL_IMAGE_FORMAT_MISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_PROGRAM_FAILURE";
    case -12: return "CL_MAP_FAILURE";
    case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    case -16: return "CL_LINKER_NOT_AVAILABLE";
    case -17: return "CL_LINK_PROGRAM_FAILURE";
    case -18: return "CL_DEVICE_PARTITION_FAILED";
    case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

    // compile-time errors
    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_CONTEXT";
    case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_QUEUE";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_PROGRAM";
    case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_BUFFER_SIZE";
    case -62: return "CL_INVALID_MIP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64: return "CL_INVALID_PROPERTY";
    case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66: return "CL_INVALID_COMPILER_OPTIONS";
    case -67: return "CL_INVALID_LINKER_OPTIONS";
    case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

    // extension errors
    case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
    case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
    case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
    case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    default: return "Unknown OpenCL error";
    }
#endif
return "No OpenCL installed";
}


size_t fdtd_opencl_load_code(struct simulation *sim,struct fdtd_data *data)
{
	size_t srcsize;
	srcsize=0;

	#ifdef use_open_cl


	size_t file_size=0;
	size_t len;
	char *error_msg;
	cl_int error;

	data->cq = clCreateCommandQueue(data->context, data->device, 0, &error);
	if ( error != CL_SUCCESS )
	{
		printf_log(sim, "clCreateCommandQueue error\n" );
		printf_log(sim,"\n Error number %d", error);
		exit(0);
	}


    FILE *in=fopen("./libfdtd/code.cl","r");
	if (in==NULL)
	{
		ewe(sim,"I could not find the OpenCL code\n");
	}

	fseek(in, 0, SEEK_END);
	file_size = ftell(in);
	fseek(in, 0, SEEK_SET);

	data->src_code=malloc(sizeof(char)*(file_size+1));

    fread(data->src_code, file_size, 1, in);
	//printf("%ld %d",srcsize,file_size);
	data->src_code[file_size]=0;
    fclose(in);

	//printf("%s\n",data->src_code);

	data->prog=clCreateProgramWithSource(data->context,1, (const char **)&(data->src_code), &file_size, &error);
	// and compile it (after this we could extract the compiled version)
	error=clBuildProgram(data->prog, 0, NULL, "", NULL, NULL);

    if ( error != CL_SUCCESS )
	{
		printf("Error on clbuildProgram \n" );
		printf("Error number %d", error);
		printf("RequestingInfo\n" );
		clGetProgramBuildInfo( data->prog, data->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &len );
		error_msg=malloc(sizeof(char)*len);
		clGetProgramBuildInfo( data->prog, data->device, CL_PROGRAM_BUILD_LOG, len, error_msg, NULL );
		printf( "Build Log for %s_program:\n%s\n", "example", error_msg );
		free(error_msg);
		exit(0);
    }
	#endif

	return srcsize;
}


void fdtd_opencl_push_to_gpu(struct simulation *sim,struct fdtd_data *data)
{
	#ifdef use_open_cl
	int i;
	cl_int error;
	memcpy(data->gy, data->y_mesh, sizeof(float)*data->ylen );
	error=clEnqueueWriteBuffer(data->cq, data->ggy, CL_FALSE, 0, data->ylen*sizeof(float), data->gy, 0, NULL, NULL);

	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}

	for (i=0;i<data->zlen;i++)
	{
		memcpy ( &(data->gEx[i*data->ylen]), data->Ex[i], sizeof(float)*data->ylen );
		memcpy ( &(data->gEy[i*data->ylen]), data->Ey[i], sizeof(float)*data->ylen );
		memcpy ( &(data->gEz[i*data->ylen]), data->Ez[i], sizeof(float)*data->ylen );
		memcpy ( &(data->gHx[i*data->ylen]), data->Hx[i], sizeof(float)*data->ylen );
		memcpy ( &(data->gHy[i*data->ylen]), data->Hy[i], sizeof(float)*data->ylen );
		memcpy ( &(data->gHz[i*data->ylen]), data->Hz[i], sizeof(float)*data->ylen );

		memcpy ( &(data->gEx_last[i*data->ylen]), data->Ex_last[i], sizeof(float)*data->ylen );
		memcpy ( &(data->gEy_last[i*data->ylen]), data->Ey_last[i], sizeof(float)*data->ylen );
		memcpy ( &(data->gEz_last[i*data->ylen]), data->Ez_last[i], sizeof(float)*data->ylen );

		memcpy ( &(data->gHx_last[i*data->ylen]), data->Hx_last[i], sizeof(float)*data->ylen );
		memcpy ( &(data->gHy_last[i*data->ylen]), data->Hy_last[i], sizeof(float)*data->ylen );
		memcpy ( &(data->gHz_last[i*data->ylen]), data->Hz_last[i], sizeof(float)*data->ylen );

		memcpy ( &(data->gepsilon_r[i*data->ylen]), data->epsilon_r[i], sizeof(float)*data->ylen );
	}


	error=clEnqueueWriteBuffer(data->cq, data->ggEx, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gEx, 0, NULL, NULL);
	error=clEnqueueWriteBuffer(data->cq, data->ggEy, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gEy, 0, NULL, NULL);
	error=clEnqueueWriteBuffer(data->cq, data->ggEz, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gEz, 0, NULL, NULL);

	error=clEnqueueWriteBuffer(data->cq, data->ggHx, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gHx, 0, NULL, NULL);
	error=clEnqueueWriteBuffer(data->cq, data->ggHy, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gHy, 0, NULL, NULL);
	error=clEnqueueWriteBuffer(data->cq, data->ggHz, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gHz, 0, NULL, NULL);

	error=clEnqueueWriteBuffer(data->cq, data->ggEx_last, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gEx_last, 0, NULL, NULL);
	error=clEnqueueWriteBuffer(data->cq, data->ggEy_last, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gEy_last, 0, NULL, NULL);
	error=clEnqueueWriteBuffer(data->cq, data->ggEz_last, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gEz_last, 0, NULL, NULL);

	error=clEnqueueWriteBuffer(data->cq, data->ggHx_last, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gHx_last, 0, NULL, NULL);
	error=clEnqueueWriteBuffer(data->cq, data->ggHy_last, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gHy_last, 0, NULL, NULL);
	error=clEnqueueWriteBuffer(data->cq, data->ggHz_last, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gHz_last, 0, NULL, NULL);

	error=clEnqueueWriteBuffer(data->cq, data->ggepsilon_r, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gepsilon_r, 0, NULL, NULL);

	#endif
}


void fdtd_opencl_write_ctrl_data(struct simulation *sim,struct fdtd_data *data)
{
	#ifdef use_open_cl
	int i;
	cl_int error;
	//float Cx=(data->dt2/(epsilon0f*data->dx));
	float Cy=(data->dt2/(epsilon0f*data->dy));
	float Cz=(data->dt2/(epsilon0f*data->dz));
	float Cmy=(data->dt2/(mu0f*data->dy));
	float Cmx=(data->dt2/(mu0f*data->dx));
	float Cmz=(data->dt2/(mu0f*data->dy));
	float time=0.0;
	float gC[17];
	gC[0]=1.0;
	gC[1]=Cy;
	gC[2]=Cz;
	gC[3]=(float)data->ylen;
	gC[4]=(float)data->zlen;
	gC[5]=(float)time;
	gC[6]=(float)data->omega;
	gC[7]=(float)data->lambda;
	gC[8]=(float)data->dx;
	gC[9]=(float)data->dy;
	gC[10]=(float)data->dz;
	gC[11]=(float)data->dt2;
	gC[12]=(float)Cmx;
	gC[13]=(float)Cmy;
	gC[14]=(float)Cmz;
	gC[15]=(float)data->stop;
	gC[16]=(float)data->excitation_mesh_point_y;//data->sithick;

	for (i=0;i<17;i++)
	{
		printf("gC%d=%lf\n",i,gC[i]);
	}

	error=clEnqueueWriteBuffer(data->cq, data->ggC, CL_FALSE, 0, 17*sizeof(float), gC, 0, NULL, NULL);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"error!!!!!!!!!!!!!!!!!!\n");
	}
	#endif
}

int fdtd_opencl_solve_step(struct simulation *sim,struct fdtd_data *data)
{
	#ifdef use_open_cl
	cl_int error;
	size_t global = (size_t)data->zlen*data->ylen;
	size_t local = (size_t)16;

	error=clEnqueueNDRangeKernel(data->cq, data->cal_E, 1, NULL, &global, &local, 0, NULL, NULL);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"Run error in cal_E kernel %s\n",getErrorString(error));
	}
	error=clFinish(data->cq);

	error=clEnqueueNDRangeKernel(data->cq, data->update_E, 1, NULL, &global, &local, 0, NULL, NULL);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"Run error in update_E kernel %s\n",getErrorString(error));
	}
	error=clFinish(data->cq);

	error=clEnqueueNDRangeKernel(data->cq, data->cal_H, 1, NULL, &global, &local, 0, NULL, NULL);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"Run error in H kernel\n");
	}
	error=clFinish(data->cq);


	error=clEnqueueNDRangeKernel(data->cq, data->update_H, 1, NULL, &global, &local, 0, NULL, NULL);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"Run error in E kernel\n");
	}
	error=clFinish(data->cq);

	return 0;
	#endif

return -1;

}

void fdtd_opencl_pull_data(struct simulation *sim,struct fdtd_data *data)
{
	#ifdef use_open_cl
	int i;
	cl_int error;

	error=clEnqueueReadBuffer(data->cq, data->ggHx, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gHx, 0, NULL, NULL);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"%s\n",getErrorString(error));
	}
	error=clEnqueueReadBuffer(data->cq, data->ggHy, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gHy, 0, NULL, NULL);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"%s\n",getErrorString(error));
	}
	error=clEnqueueReadBuffer(data->cq, data->ggHz, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gHz, 0, NULL, NULL);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"%s\n",getErrorString(error));
	}

	error=clEnqueueReadBuffer(data->cq, data->ggEx, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gEx, 0, NULL, NULL);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"%s\n",getErrorString(error));
	}

	error=clEnqueueReadBuffer(data->cq, data->ggEy, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gEy, 0, NULL, NULL);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"%s\n",getErrorString(error));
	}

	error=clEnqueueReadBuffer(data->cq, data->ggEz, CL_FALSE, 0, data->zlen*data->ylen*sizeof(float), data->gEz, 0, NULL, NULL);
	if (error!=CL_SUCCESS)
	{
		printf_log(sim,"%s\n",getErrorString(error));
	}

	for (i=0;i<data->zlen;i++)
	{
		memcpy ( data->Ex[i],&(data->gEx[i*data->ylen]), sizeof(float)*data->ylen );
		memcpy ( data->Ey[i],&(data->gEy[i*data->ylen]), sizeof(float)*data->ylen );
		memcpy ( data->Ez[i],&(data->gEz[i*data->ylen]), sizeof(float)*data->ylen );
	}

	for (i=0;i<data->zlen;i++)
	{
		memcpy ( data->Hx[i],&(data->gHx[i*data->ylen]), sizeof(float)*data->ylen );
		memcpy ( data->Hy[i],&(data->gHy[i*data->ylen]), sizeof(float)*data->ylen );
		memcpy ( data->Hz[i],&(data->gHz[i*data->ylen]), sizeof(float)*data->ylen );
	}

	error=clFinish(data->cq);
	#endif
}


