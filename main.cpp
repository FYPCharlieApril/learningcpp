#include <mlpack/core.hpp>
#include <iostream>
#include <armadillo>
#include "hypergraph.h"
#include "subgradient.h"
#include <fstream>

#define PREC_C 1000000
int main(int argc, char* argv[])
{ 
  string datasetname = argv[1];
  double precision = atof(argv[2]);
  bool front = atoi(argv[3]);
  int from = atoi(argv[4]);  
  int to = atoi(argv[5]);
  int foldsize = atoi(argv[6]);
  ofstream myfile;
  myfile.open ("log/record1.txt", ios::out);
  myfile << "[Precision] "+to_string(precision)<<endl;
  for (int i=from; i<=to; i++){
    myfile << "[Number of labeled data] " + to_string(foldsize * i) <<endl;
    for (int j=0; j<5; j++){
          
	  Hypergraph *hg = new Hypergraph;
 
	  bool res = hg->constructHMat("dataset/"+datasetname, front);
	  mlpack::data::Save("log/oh_fea.txt", hg->hMat);
	  mlpack::data::Save("log/oh_la.txt", hg->lMat);
	  
          //place to decide the precision of the training, n_rows of hMat means how many information we have, 
          //while lMat.n_rows shows how complicate the problem is 
          if (precision == 0.0){
            precision = 1.0/(PREC_C*hg->lMat.n_rows/hg->hMat.n_rows);
          }
	  //record the start time
	  struct timeval tp;
	  gettimeofday(&tp, NULL);
	  long int starttime = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	 
	  Subgradient *sg = new Subgradient;

	  //core work here
	  Mat<unsigned int> result = sg->fitPredict(hg, foldsize * i, precision);

          myfile << "[accuracy] " + to_string(j) + ": " <<sg->accuracy<<endl;
	  //record the end time of the task
	  gettimeofday(&tp, NULL);
	  long int endtime = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	  //write result to log
	  mlpack::data::Save("log/result.txt", result);
	  
	  //change the time to minutes and seconds' form
	  int sec = (endtime - starttime)/1000;
	  int min = sec / 60;
	  sec = sec % 60;
          
          myfile << "[time] "<<min<<" mins "<<sec<<" secs"<<endl;
	  printf("Spent time: %d minutes %d seconds\n", min, sec);
          
          // deal with the useless pointer
          delete hg, sg;
          hg = NULL; sg = NULL;
    }
    myfile<<endl;
  }
  myfile.close();
  return 0;
}
