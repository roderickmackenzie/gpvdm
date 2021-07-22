Gpvdm for Ubuntu 20.04 README
-----------------------------

This readme is for the compiled version of gpvdm which is distributed in a .tar file for Linux. To run gpvdm follow the following steps:

1. Download the zip/tar file and unzip it.

2. In the terminal navigate to the directory where you have unzipped gpvdm.

5. As su run:

```bash
install_packages.sh
```
This will download the packages need for gpvdm to run.  On a VM which is a fresh install of Ubuntu with a fast internet connection it takes about half an hour to finish.  I try to keep this script up to date, but if packages get renamed you may have to edit it.

5. Type ./gpvdm in the gpvdm directory

6. Gpvdm will start and insert the following line in your .bashrc:

```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/your/install/gpvdm_core
```

7. Close gpvdm and close the bash terminal for the above line in your .bashrc to take effect.

8. Open a new bash terminal, navigate to the gvdpm directory and launch gpvdm again.

9. That's it! Enjoy :)

