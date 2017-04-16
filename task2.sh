for (( i=20; i<=200; i=i+20 ))
do
	echo "$i" >> log/mushroom_gau_temp.txt;
	./main_para mushroom.csv 1 0.001 $i x g mushroom_gau_temp;
	./main_para mushroom.csv 1 0.001 $i x g mushroom_gau_temp;
	./main_para mushroom.csv 1 0.001 $i x g mushroom_gau_temp;
	./main_para mushroom.csv 1 0.001 $i x g mushroom_gau_temp;
	./main_para mushroom.csv 1 0.001 $i x g mushroom_gau_temp;
done

