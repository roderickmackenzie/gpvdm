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

#include <stdio.h>
#include <ray.h>
#include <gpvdm_const.h>
#include <math.h>
#include <stdlib.h>
#include <cal_path.h>
#include <log.h>

/** @file wavelength_to_rgb.c
	@brief Turn a wavelegngth to an RGB color.
*/

void wavelength_to_rgb(int *r,int *g,int *b,double wavelength)
{
	double gamma=0.80;
	double intensity_max=1.0;
	double factor=0.0;

	double red=0.0;
	double green=0.0;
	double blue=0.0;

	if ((wavelength >= 380e-9) && (wavelength<440e-9))
	{
		red = -(wavelength - 440e-9) / (440e-9 - 380e-9);
		green = 0.0;
		blue = 1.0;
	}else
	if ((wavelength >= 440e-9) &&  (wavelength<490e-9))
	{
		red = 0.0;
		green = (wavelength - 440e-9) / (490e-9 - 440e-9);
		blue = 1.0;
	}else
	if ((wavelength >= 490e-9) &&  (wavelength<510e-9))
	{
		red = 0.0;
		green = 1.0;
		blue = -(wavelength - 510e-9) / (510e-9 - 490e-9);
	}else
	if ((wavelength >= 510e-9) &&  (wavelength<580e-9))
	{
		red = (wavelength - 510e-9) / (580e-9 - 510e-9);
		green = 1.0;
		blue = 0.0;
	}else
	if ((wavelength >= 580e-9) &&  (wavelength<645e-9))
	{
		red = 1.0;
		green = -(wavelength - 645e-9) / (645e-9 - 580e-9);
		blue = 0.0;
	}else
	if ((wavelength >= 645e-9) &&  (wavelength<781e-9))
	{
		red = 1.0;
		green = 0.0;
		blue = 0.0;
	}else
	{
		red = 0.0;
		green = 0.0;
		blue = 0.0;
	}

	if (wavelength<420e-9)
	{
		factor = 0.3 + 0.7*(wavelength - 380e-9) / (420e-9 - 380e-9);
	}else
	if ((wavelength >= 420e-9) && (wavelength<701e-9))
	{
		factor = 1.0;
	}else
	if ((wavelength >= 701e-9) &&  (wavelength<781e-9))
	{
		factor = 0.3 + 0.7*(780e-9 - wavelength) / (780e-9 - 700e-9);
	}else
	{
		factor = 0.0;
	}

	if (red != 0.0)
	{
		red = (intensity_max * pow(red * factor, gamma));
	}

	if (green != 0)
	{
		green = (intensity_max * pow(green * factor, gamma));
	}

	if (blue != 0)
	{
		blue = (intensity_max * pow(blue * factor, gamma));
	}


	if ((red==0.0)&&(green==0.0)&&(blue==0.0))
	{
		red=0.392634;
		green=0.000000;
		blue=0.397315;
	}

	*r=255.0*red;
	*g=255.0*green;
	*b=255.0*blue;
}
