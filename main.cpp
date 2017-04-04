#include <mlpack/core.hpp>
#include <iostream>
#include <armadillo>
#include "hypergraph.h"
#include "subgradient.h"

int main()
{ 
  Hypergraph *hg = new Hypergraph;
  bool res = hg->constructHMat("mushroom.csv");
  mlpack::data::Save("log/oh_fea.txt", hg->hMat);
  mlpack::data::Save("log/oh_la.txt", hg->lMat);
  
  Subgradient *sg = new Subgradient;
  
  mlpack::data::Save("log/result.txt", sg->fitPredict(hg, 200));
  delete hg, sg;
  hg = NULL;
  return 0;
}
