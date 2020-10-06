gpvdm - Simulate Organic Solar Cells and  Inorganic solar cells
===============================================================

Gpvdm is a tool to simulate thin film optoelectronic devices including, organic solar cells, perovskite solar cells, thin film light emitting diodes and transistors.  The model contains 1/2D electrical and an optical 
solvers, enabling both current/voltage characteristics to be simulated as well 
as the optical properties of a device. The model and it's easy to use 
graphical interface is available for both Linux and Windows, x86 and ARM.
The model can simulate:

- Steady state measurements such as JV curves, and Suns-Voc
- Transient measurements such as CELIV, Transient photo current measurements and Transient photocurrent measurements.
- Full optical model taking into account reflection at interfaces and absorption.
- Calculation of reflection profile
- Fitting to experimental data

The physical model solves both electron and hole drift-diffusion, and carrier 
continuity equations in position space to describe the movement of charge 
within the device. The model also solves Poisson's equation to calculate the 
internal electrostatic potential. Recombination and carrier trapping are 
described within the model using a Shockley-Read-Hall (SRH) formalism, the 
distribution of trap sates can be arbitrarily defined. A fuller description of 
the model can be found in the at https://www.gpvdm.com, in the associated
publications  and in the manual.

The model makes it easy to study the influence of material parameters such as 
mobility, energetic disorder, doping and recombination cross-sections on device 
performance. All internal device parameters such as current density, charge 
density, distribution of trapped carriers in position and energy space are 
accessible either through the graphical interface or directly through output 
files. 

Installing/building gpvdm
==============

1 Windows
----------

I would recommend downloading the binary from the gpvdm web page.  Double click on the installer and follow the instructions.  I always keep the windows exe up-to date and on the latest stable release.

2 Linux
--------

Download the gpvdm by issuing the command 

~~~~
git clone  https://github.com/roderickmackenzie/gpvdm
~~~~

Find your operating system in
~~~~
build_system/dependency_scripts
~~~~

This script should install all the packages you need to run/compile gpvdm for a given OS.  I don't always keep them up to date, so if you have a new version of an OS and the packages have been renamed you may have to hunt around.


Then run:

~~~~
./build
~~~~

Then select, (compile), and (auto).  Then hit return to build.


Help using gpvdm
----------------
I'm very happy to provide help in using gpvdm, or if you wold prefer I am 
equally happy to collaborate and model your data for you. See the contact page.


More information
----------------
More information can be found on the home page https://www.gpvdm.com

Licensing
---------
gpvdm comes in three parts with different licenses:
- gpvdm_core: This is licensed under a 3-clause BSD license.
- gpvdm_gui: This is licensed under a GPLv2 license.
- gpvdm_build_system: This is also licensed under a BSD 3-clause license
- gpvdm_data: Creative Commons BY-CC.
See the individual license files for details.
