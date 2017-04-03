#include <stdio.h>
#include <stdlib.h>     
#include <time.h>      
#include <mlpack/core.hpp>
#include <mlpack/core/data/binarize.hpp>
#include <armadillo>
#include <string>

#ifndef __HYPERGRAPH_H_INCLUDED
#define __HYPERGRAPH_H_INCLUDED

using namespace arma;
using namespace std;
using namespace mlpack;

class Hypergraph{
  public:
    Mat<unsigned int> hMat;
    Mat<unsigned int> lMat;
    Mat<unsigned int> head;
    Mat<unsigned int> tail;
    Row<unsigned int> weight;
    bool constructHMat(string filename);
  private:
    Mat<unsigned int> oneHotEncoding(Mat<int> oriData);
    void shuffleMat(int time_to_shuffle);
};

#endif
