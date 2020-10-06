gpvdm_gui - Simulate Organic Solar Cells, Inorganic solar cells, OLEDs and OFETs for free
=========================================================================================

This is the graphical user interface to gpvdm.  Gpvdm is a tool to simulate thin film optoelectronic devices including, organic solar cells, perovskite solar cells, thin film light emitting diodes and transistors.  The model contains 1/2D electrical and an optical 
solvers, enabling both current/voltage characteristics to be simulated as well 
as the optical properties of a device. The model and it's easy to use 
graphical interface is available for both Linux and Windows, x86 and ARM.
The model can simulate:

    -Dark JV curves
    -Light JV curves
    -Dark CELIV transients
    -Light CELIV transients
    -Voltage transients of an arbitrary shape
    -Full optical model taking into account reflection at interfaces and absorption.
    -Calculation of reflection profile
    -Ability to simulate OLEDs

The physical model solves both electron and hole drift-diffusion, and carrier 
continuity equations in position space to describe the movement of charge 
within the device. The model also solves Poisson's equation to calculate the 
internal electrostatic potential. Recombination and carrier trapping are 
described within the model using a Shockley-Read-Hall (SRH) formalism, the 
distribution of trap sates can be arbitrarily defined. A fuller description of 
the model can be found in the at https://www.gpvdm.com, in the associated
publications  and in the manual.
Example simulations

The model makes it easy to study the influence of material parameters such as 
mobility, energetic disorder, doping and recombination cross-sections on device 
performance. All internal device parameters such as current density, charge 
density, distribution of trapped carriers in position and energy space are 
accessible either through the graphical interface or directly through output 
files. 

More information
----------------
More information can be found on the home page https://www.gpvdm.com

LICENSE
-------
Gpvdm_gui is free software licensed under the GPL v2 license, please see the LICENSE.txt for further details.  Gpvdm_gui relies on other open source software to work these include:

*PyQt5 (GPL3) https://github.com/PyQt5/PyQt/blob/master/LICENSE

*matplotlib (BSD) https://matplotlib.org/3.2.2/users/license.html

*numpy (BSD) https://numpy.org/doc/1.13/license.html

*i18n (MIT) https://github.com/danhper/python-i18n/blob/master/LICENSE

*OpenGL (BSD-style) https://github.com/mcfletch/pyopengl/blob/master/license.txt

*PIL (BSD-style) https://github.com/python-pillow/Pillow/blob/master/LICENSE

Translations
------------
Gpvdm_gui has been translated into the following languages:

*Portuguese by Luciano Azevedo Neves, in 2019

*Chinese by Liu Di (刘迪) and Zhao Chenyao (赵辰尧), in 2018


