#include <mlpack/core.hpp>
#include <iostream>
#include <armadillo>
#include "hypergraph.h"
#include "subgradient.h"
#include <fstream>
int main(int argc, char* argv[])
{ 
  string datasetname = argv[1];
  double precision = atof(argv[2]);
  bool front = atoi(argv[3]);
  ofstream myfile;
  myfile.open ("log/record1.txt", ios::out);
  myfile << "[Precision] "+to_string(precision)<<endl;
  for (int i=1; i<=10; i++){
    myfile << "[Number of labeled data] " + to_string(20 * i) <<endl;
    for (int j=0; j<5; j++){
          
	  Hypergraph *hg = new Hypergraph;
 
	  bool res = hg->constructHMat("dataset/"+datasetname, front);
	  mlpack::data::Save("log/oh_fea.txt", hg->hMat);
	  mlpack::data::Save("log/oh_la.txt", hg->lMat);

	  
	  //record the start time
	  struct timeval tp;
	  gettimeofday(&tp, NULL);
	  long int starttime = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	 
	  Subgradient *sg = new Subgradient;

	  //core work here
	  Mat<unsigned int> result = sg->fitPredict(hg, 20*i, precision);

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
          
          myfile << "[time] "<<min<<" mins"<<sec<<" secs"<<endl;
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
