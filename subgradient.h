#include <mlpack/core.hpp>
#include <iostream>
#include <armadillo>
#include "hypergraph.h"

#ifndef __SUBGRADIENT_H_INCLUDED
#define __SUBGRADIENT_H_INCLUDED

using namespace arma;
using namespace std;
using namespace mlpack;

class Subgradient{
  public:
    Mat<unsigned int> fitPredict(Hypergraph *hg, int train_size);
    double accuracy;        

  private:
    mat computeDelta(mat f, Hypergraph *hg, int train_size);
    mat sgm(mat f, Hypergraph *hg, int train_size);
};

#endif