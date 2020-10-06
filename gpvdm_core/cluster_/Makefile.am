zip=zip


all: cluster.zip
	echo "cluster zipped"

cluster.zip: ../cluster/*.c ../cluster/*.h ../cluster/*.inp
	zip -r cluster.zip ../cluster/*.c ../cluster/*.h ../cluster/*.inp

#cluster.zip: ../gpvdm
#	cd ../cluster;$(zip) -rq ../cluster_/cluster.zip ./

clean:
	rm *.zip -f

