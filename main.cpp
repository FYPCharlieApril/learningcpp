#include <mlpack/core.hpp>
#include <iostream>
#include <armadillo>
#include "hypergraph.h"
#include "subgradient.h"

int main()
{ 
  Hypergraph *hg = new Hypergraph;

  bool res = hg->constructHMat("mushroom.csv", true);
  mlpack::data::Save("log/oh_fea.txt", hg->hMat);
  mlpack::data::Save("log/oh_la.txt", hg->lMat);

  //record the start time
  struct timeval tp;
  gettimeofday(&tp, NULL);
  long int starttime = tp.tv_sec * 1000 + tp.tv_usec / 1000;  
  
  printf("Start time: %ld\n", starttime);
 
  Subgradient *sg = new Subgradient;
  

  //core work here
  Mat<unsigned int> result = sg->fitPredict(hg, 80);

  //record the end time of the task
  gettimeofday(&tp, NULL);
  long int endtime = tp.tv_sec * 1000 + tp.tv_usec / 1000;
  printf("End time: %ld\n", endtime);

  //write result to log
  mlpack::data::Save("log/result.txt", result);
  
  //change the time to minutes and seconds' form
  int sec = (endtime - starttime)/1000;
  int min = sec / 60;
  sec = sec % 60;
  printf("Spent time: %d minutes %d seconds\n", min, sec);
  delete hg, sg;
  hg = NULL;
  return 0;
}
