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
