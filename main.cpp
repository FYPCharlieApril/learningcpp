#include <mlpack/core.hpp>
#include <iostream>
#include <armadillo>
#include "hypergraph.h"
#include "subgradient.h"

int main()
{ 
  Hypergraph *hg = new Hypergraph;
  bool res = hg->constructHMat("mushroom.csv");
  mlpack::data::Save("oh_fea.txt", hg->hMat);
  mlpack::data::Save("oh_la.txt", hg->lMat);

  Subgradient *sg = new Subgradient;
  sg->fitPredict(hg, 0);  
  
  
   
  delete hg, sg;
  hg = NULL;
  return 0;
}
