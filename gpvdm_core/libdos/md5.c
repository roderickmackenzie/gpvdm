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

/** @file checksum.c
	@brief An MD5 like check sum for deciding if files have been changed.  Implemented because I do not want openssl dependencies.
*/

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <gpvdm_const.h>
#include "inp.h"
#include "util.h"
#include "cal_path.h"
#include "md5.h"

static int unused __attribute__((unused));

uint32_t leftrotate (uint32_t x, uint32_t c)
{
    return ((x << c) | (x >> (32 -c)));
}

uint32_t s[64]={ 7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22 ,
			     5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20 ,
			     4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23 ,
			     6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 };

uint32_t K[64]={	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
 					0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
					0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
					0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
					0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
					0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
					0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
					0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
					0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
					0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
					0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
					0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
					0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
					0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
					0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
					0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

void md5_init(struct md5* in)
{
	in->a0=0x67452301;
	in->b0=0xefcdab89;
	in->c0=0x98badcfe;
	in->d0=0x10325476;
}

void md5_update(struct md5* in,char *data,int len)
{
int i;
int ii;
uint32_t F;
int g;
uint32_t A;
uint32_t B;
uint32_t C;
uint32_t D;

int len_bits=len*8+8;
int add_bits=512-(len_bits % 512);
int new_len_bits=len_bits+add_bits;
int new_len_bytes=new_len_bits/8;
uint32_t* new_data=malloc(new_len_bytes*sizeof(char));
memset(new_data, 0, new_len_bytes);
memcpy(new_data, data, len);
((char*)new_data)[len]=0x80;



int chunks=new_len_bits/512;



for (i=0;i<chunks;i++)
{
	A = in->a0;
	B = in->b0;
	C = in->c0;
	D = in->d0;

    for (ii=0;ii<64;ii++)
	{
        if (ii<16)
		{
            F = (B & C) | ((~B) & D);
            g = ii;
		}else
		if (ii<32)
		{
            F = (D & B) | (( ~D) & C);
            g = (5*ii + 1) % 16;
		}else
		if (ii<48)
		{
            F = B ^ C ^ D;
            g = (3*ii + 5) % 16;
        }else
		if (ii<64)
		{
            F = C ^ (B | (~ D));
            g = (7*ii) % 16;
		}

		F = F + A + K[ii] + new_data[g+i*16];
		A = D;
        D = C;
        C = B;
        B = B + leftrotate(F, s[ii]);

	}

    in->a0 = in->a0 + A;
    in->b0 = in->b0 + B;
    in->c0 = in->c0 + C;
    in->d0 = in->d0 + D;

}

free(new_data);
}

void md5_to_str(char *out,struct md5 *in)
{
	sprintf(out,"%.8x%.8x%.8x%.8xL",in->a0,in->b0,in->c0,in->d0);
}


