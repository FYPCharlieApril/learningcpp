for (( i=100; i<=1400; i=i+100 ))
do
	./main_para car.csv 0 0.001 $i p x cartemp;
	./main_para car.csv 0 0.001 $i p x cartemp;
	./main_para car.csv 0 0.001 $i p x cartemp;
	./main_para car.csv 0 0.001 $i p x cartemp;
	./main_para car.csv 0 0.001 $i p x cartemp;
done

for (( i=100; i<=1400; i=i+100 ))
do
	./main_para car.csv 0 0.001 $i n x cartemp;
	./main_para car.csv 0 0.001 $i n x cartemp;
	./main_para car.csv 0 0.001 $i n x cartemp;
	./main_para car.csv 0 0.001 $i n x cartemp;
	./main_para car.csv 0 0.001 $i n x cartemp;
done

for (( i=100; i<=1400; i=i+100 ))
do
	./main_para car.csv 0 0.001 $i e x cartemp;
	./main_para car.csv 0 0.001 $i e x cartemp;
	./main_para car.csv 0 0.001 $i e x cartemp;
	./main_para car.csv 0 0.001 $i e x cartemp;
	./main_para car.csv 0 0.001 $i e x cartemp;
done

