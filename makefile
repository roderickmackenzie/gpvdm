all:
	cd gpvdm_core && make -j 4
	cd gpvdm_gui && make -j 4

clean:
	cd gpvdm_core && make clean
	rm *.c *.exe -f

easy:
	for i in `ls ./gpvdm_core|grep lib`; do echo $i; done
