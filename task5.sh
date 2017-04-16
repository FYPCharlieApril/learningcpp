for (( i=10; i<=100; i=i+10 ))
do
	echo "$i" >> log/cartemp.txt;
	./main car.csv 0 0.001 $i s x cartemp;
	./main car.csv 0 0.001 $i s x cartemp;
	./main car.csv 0 0.001 $i s x cartemp;
	./main car.csv 0 0.001 $i s x cartemp;
	./main car.csv 0 0.001 $i s x cartemp;
done

for (( i=10; i<=100; i=i+10 ))
do
	echo "$i" >> log/cartemp.txt;
	./main car.csv 0 0.001 $i p x cartemp;
	./main car.csv 0 0.001 $i p x cartemp;
	./main car.csv 0 0.001 $i p x cartemp;
	./main car.csv 0 0.001 $i p x cartemp;
	./main car.csv 0 0.001 $i p x cartemp;
done

for (( i=10; i<=100; i=i+10 ))
do
	echo "$i" >> log/cartemp.txt;
	./main car.csv 0 0.001 $i n x cartemp;
	./main car.csv 0 0.001 $i n x cartemp;
	./main car.csv 0 0.001 $i n x cartemp;
	./main car.csv 0 0.001 $i n x cartemp;
	./main car.csv 0 0.001 $i n x cartemp;
done

for (( i=10; i<=100; i=i+10 ))
do
	echo "$i" >> log/cartemp.txt;
	./main car.csv 0 0.001 $i e x cartemp;
	./main car.csv 0 0.001 $i e x cartemp;
	./main car.csv 0 0.001 $i e x cartemp;
	./main car.csv 0 0.001 $i e x cartemp;
	./main car.csv 0 0.001 $i e x cartemp;
done


