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
    void computeDelta(mat &f, mat &f_out, Hypergraph *hg);
    mat markovOp();
    void sgm();
    double evalF(Col<unsigned int> fCol);
};

#endif
