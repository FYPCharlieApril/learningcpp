for (( i=20; i<=100; i=i+20 ))
do
	echo "$i" >> log/cartemp.txt;
	./main audiology.csv 0 0.001 $i x x audiotemp;
done
