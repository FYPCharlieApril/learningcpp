#include "subgradient.h"

Mat<unsigned int> Subgradient::fitPredict(Hypergraph *hg, int train_size){
  Mat<unsigned int> result = zeros<Mat<unsigned int>>(hg->lMat.n_rows, hg->lMat.n_cols);
  
  //computeDelta(temp, hg);
  return result;
}


// compute the delta value for f in each iteration
void Subgradient::computeDelta(mat &f, mat &f_out, Hypergraph *hg){
  int hrow = hg -> hMat.n_rows; // number of edges 117
  int hcol = hg -> hMat.n_cols; // dataset size 8124 
  int lrow = hg -> lMat.n_rows; // number of classes 2
  
  //printf("%d %d %d \n", hrow, hcol, lrow);
  
  for (int i=0; i<hrow; i++){
    uvec allTailId = find(hg->tail.row(i) == 1);
    uvec allHeadId = find(hg->head.row(i) == 1);
    for (int j=0; j<lrow; j++){
       double u = f.row(j)(allTailId).max();
       double v = f.row(j)(allHeadId).min();
       double d = weight(i) * (u - v);
       
       if (u - v > 0){
         uword id = find(hg->tail.row(i) == u)(0);
         f_out(j, id) += d;
       }
    }
  }
}

//  function to compute the Markov Operator
mat Subgradient::markovOp(Hypergraph *hg){
  mat f_out = zeros<mat>(hg->lMat.n_rows, hg->lMat.n_cols);
  int hrow = hg -> hMat.n_rows; // number of edges 117
  int hcol = hg -> hMat.n_cols; // dataset size 8124 
  int lrow = hg -> lMat.n_rows; // number of classes 2

  
}

// the subgradient method core function
void Subgradient::sgm(){
  
}

double evalF(Col<unsigned int> fCol){
  double res = 0.0; 
  
  
  
  return res;
}

