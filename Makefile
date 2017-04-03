CFLAGS = -I/home/posnet/Documents/mlpack-2.2.0/build/
MFLAG = -lmlpack
AFLAG = -larmadillo
OBJ = main.cpp hypergraph.cpp subgradient.cpp
SFLAG = -std=c++11
 
main: main.cpp hypergraph.cpp
	g++ $(SFLAG) $(CFLAGS) $(OBJ) $(MFLAG) $(AFLAG) -o main 

test: test.cpp
	g++ $(SFLAG) $(CFLAGS) test.cpp $(MFLAG) $(AFLAG) -o test 
