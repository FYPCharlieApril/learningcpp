#include <mlpack/core.hpp>
#include <iostream>
#include <armadillo>
#include "hypergraph.h"
#include "subgradient.h"

int main(int argc, char* argv[])
{ 
  //read parameters for the learning
  //run example: ./main mushroom.csv 1 0.001 20 x x mushtemp
  string datasetname = argv[1];
  bool front = atoi(argv[2]);
  double precision = atof(argv[3]);  
  int train_size = atoi(argv[4]);
  char actfunc = *argv[5];
  char lossfunc = *argv[6];
  string resFile = argv[7];
  
  //stream to write log file  
  ofstream myfile;
  
  struct timeval tp;
  Hypergraph *hg = new Hypergraph;

  myfile.open ("log/"+resFile+".txt", ios::out | ios::app);
  myfile << "[Precision] "+to_string(precision)<<endl;
  myfile << "[Number of labeled data] " + to_string(train_size) <<endl;

  bool res = hg->constructHMat("dataset/"+datasetname, front);
  mlpack::data::Save("log/oh_fea.txt", hg->hMat);
  mlpack::data::Save("log/oh_la.txt", hg->lMat);
 
  Subgradient *sg = new Subgradient;
  
  //record the start time
  gettimeofday(&tp, NULL);
  long int starttime = tp.tv_sec * 1000 + tp.tv_usec / 1000;  

  //core work here
  Mat<unsigned int> result = sg->fitPredict(hg, train_size, precision, actfunc, lossfunc);
  
  //record the end time of the task
  gettimeofday(&tp, NULL);
  long int endtime = tp.tv_sec * 1000 + tp.tv_usec / 1000;
  
  //write accuracy to the log file
  myfile << "[accuracy] " <<sg->accuracy<<endl;
  //write result to log
  mlpack::data::Save("log/result.txt", result);
  
  //change the time to minutes and seconds' form
  int sec = (endtime - starttime)/1000;
  int min = sec / 60;
  sec = sec % 60;
  
  //write spent time to the log file
  myfile << "[time] "<<min<<" mins "<<sec<<" secs"<<endl;
  printf("Spent time: %d minutes %d seconds\n", min, sec);
  delete hg, sg;
  hg = NULL;
  return 0;
}
