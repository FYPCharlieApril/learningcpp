for (( i=10; i<=100; i=i+10 ))
do
	echo "$i" >> log/car_gau_temp.txt;
	./main_para car.csv 0 0.001 $i x g car_gau_temp;
	./main_para car.csv 0 0.001 $i x g car_gau_temp;
	./main_para car.csv 0 0.001 $i x g car_gau_temp;
	./main_para car.csv 0 0.001 $i x g car_gau_temp;
	./main_para car.csv 0 0.001 $i x g car_gau_temp;
done

