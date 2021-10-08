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

/** @file log.h
@brief write log files to disk
*/


#ifndef _log
#define _log
#include <sim_struct.h>

#define log_level_none 0
#define log_level_screen 1
#define log_level_disk 2
#define log_level_screen_and_disk 3

void textcolor(struct simulation *sim,int color);
void textcolor_rgb(struct simulation *sim, int r, int g, int b);
void set_logging_level(struct simulation *sim, int value);
void log_clear(struct simulation *sim);
void printf_log(struct simulation *sim, const char *format, ...);
void waveprint(struct simulation *sim, char *in,double wavelength);
void rainbow_print(struct simulation *sim, double start, double stop, const char *format, ...);
void log_time_stamp(struct simulation *sim);
int log_search_error(char *path);
void log_write_file_access(struct simulation *sim,char * file,char mode);
void log_tell_use_where_file_access_log_is(struct simulation *sim);
#endif
