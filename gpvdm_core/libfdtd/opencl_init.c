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

/** @file opencl_init.c
	@brief Bootstrap OpenCL, and pick the correct CPU/GPU unit.
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


void opencl_init(struct simulation *sim, struct fdtd_data *data)
{
	#ifdef use_open_cl
	int i;
	int ii;
	int count=0;
	int device_to_use=0;
	int platform_to_use=0;

	cl_int error;
	cl_device_id* devices;
	cl_platform_id* platforms;
	cl_ulong long_entries;
	size_t p_size;
	cl_uint entries;
	cl_uint platform_count;
	size_t value_size;
	char* value;
	cl_uint device_count;
	cl_int ret;

    clGetPlatformIDs(0, NULL, &platform_count);
    platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id) * platform_count);
    clGetPlatformIDs(platform_count, platforms, NULL);

	for (i = 0; i < platform_count; i++)
	{
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &device_count);
        devices = (cl_device_id*) malloc(sizeof(cl_device_id) * device_count);

        ret=clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, device_count, devices, NULL);
		if (ret==CL_SUCCESS)
		{
		    
			for (ii = 0; ii < device_count; ii++)
			{
				if (count==device_to_use)
				{
					printf("*");
					memcpy(&(data->device),&(devices[ii]), sizeof(cl_device_id));
					platform_to_use=i;
				}

				clGetDeviceInfo(devices[ii], CL_DEVICE_NAME, 0, NULL, &value_size);
				value = (char*) malloc(value_size);
				clGetDeviceInfo(devices[ii], CL_DEVICE_NAME, value_size, value, NULL);
				printf("%d %d:%d. Device: %s\n", count,i,ii, value);
				free(value);

				clGetDeviceInfo(devices[ii], CL_DRIVER_VERSION, 0, NULL, &value_size);
				value = (char*) malloc(value_size);
				clGetDeviceInfo(devices[ii], CL_DRIVER_VERSION, value_size, value, NULL);
				printf_log(sim,"\tDriver version = %s\n", value);
				free(value);

				clGetDeviceInfo(devices[ii],CL_DEVICE_GLOBAL_MEM_SIZE,sizeof(cl_ulong),&long_entries,NULL);
				printf_log(sim,"\tGlobal Memory (MB):\t%llu\n",long_entries/1024/1024);

				clGetDeviceInfo(devices[ii],CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,sizeof(cl_ulong),&long_entries,NULL);
				printf_log(sim,"\tGlobal Memory Cache (MB):\t%llu\n",long_entries/1024/1024);

				clGetDeviceInfo(devices[ii],CL_DEVICE_LOCAL_MEM_SIZE,sizeof(cl_ulong),&long_entries,NULL);
				printf_log(sim,"\tLocal Memory (KB):\t%llu\n",long_entries/1024);

				clGetDeviceInfo(devices[ii],CL_DEVICE_MAX_CLOCK_FREQUENCY,sizeof(cl_ulong),&long_entries,NULL);
				printf_log(sim,"\tMax clock (MHz) :\t%llu\n",long_entries);

				clGetDeviceInfo(devices[ii],CL_DEVICE_MAX_WORK_GROUP_SIZE,sizeof(size_t),&p_size,NULL);
				printf_log(sim,"\tMax Work Group Size:\t%d\n",p_size);

				clGetDeviceInfo(devices[ii],CL_DEVICE_MAX_COMPUTE_UNITS,sizeof(cl_uint),&entries,NULL);
				printf_log(sim,"\tNumber of parallel compute cores:\t%d\n",entries);
				count++;
			}

		}

		free(devices);
	}

	// Note that nVidia's OpenCL requires the platform property
	cl_context_properties properties[]={CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[platform_to_use],0};
	data->context=clCreateContext(properties, 1, &(data->device), NULL, NULL, &error);

	if ( error != CL_SUCCESS )
	{
		printf_log(sim, "clCreateContext error\n" );
		printf_log(sim,"\n Error number %d", error);
		exit(0);
     }

	free(platforms);
	#endif

}
