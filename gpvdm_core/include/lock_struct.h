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

//<stripall>

/** @file lock_struct.h
	@brief Header file for lock_struct.c
*/
#ifndef lock_struct_h
#define lock_struct_h

#define crypt_key "8367c849c6ec3d42d45f04fdda468766f340523e6975de94fe878f09760b7636"

#include <sim_struct.h>
#include <json_struct.h>
struct lock
{
	char status[100];
	int use_count;

	char uid[100];
	char mac[100];
	char win_id[100];
	char reg_ver[100];
	char lver[10];
	long long renew_date;
	int loaded_ok;
	int use_count_check_web;
	int use_count_max;
	long long li_renew_date;
	long long li_expire_date;
	int encode_output;
	char locked_items[4000];

	//simple lock
	char simple_id_tx[1000];
	char simple_id_ans[1000];
	char simple_id_file[1000];
	int unlocked;
	struct json j;
};

#endif
