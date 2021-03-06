#include <mlpack/core.hpp>
#include <iostream>
#include <armadillo>
#include "hypergraph.h"
#include <thread>
#include <mutex>

#ifndef __SUBGRADIENT_H_INCLUDED
#define __SUBGRADIENT_H_INCLUDED

using namespace arma;
using namespace std;
using namespace mlpack;

class Subgradient{
  public:
    Mat<unsigned int> fitPredict(Hypergraph *hg, int train_size, double precision, char actfunc, char lossfunc);
    double accuracy;        

  private:
    mat computeDelta(mat f, Hypergraph *hg, int train_size, char actfunc, char lossfunc);
    mat sgm(Hypergraph *hg, int train_size, double precision, char actfunc, char lossfunc, int ind, mat& f_a);  
    mat recoverF(Hypergraph* hg, mat &f, int train_size);
    double evalAcc(Mat<unsigned int> target, Mat<unsigned int> prediction, int train_size);
};

#endif
