#include "subgradient.h"

Mat<unsigned int> Subgradient::fitPredict(Hypergraph *hg, int train_size){
  Mat<unsigned int> result = zeros<Mat<unsigned int>>(hg->lMat.n_rows, hg->lMat.n_cols);
  
  int lRow = hg->lMat.n_rows; int lCol = hg->lMat.n_cols;
  
  mat* f_list = new mat[lRow];
  for (int i=0; i<lRow; i++){
    f_list[i] = zeros<mat>(hg->lMat.n_rows, hg->lMat.n_cols);
    f_list[i].row(i).ones();
  }
  
  mat f_a = zeros<mat>(hg->lMat.n_rows, hg->lMat.n_cols);
  for (int i=0; i<lRow; i++){
    f_a = f_a + sgm(f_list[i], hg, train_size);
  }
   
  f_a = f_a / lRow;
  
  for (int i=0; i<lRow; i++){
    double t = mean(f_a.row(i));
    for (int j=0; j<lCol; j++){
      result(i, j) = f_a(i, j) > t ? 1 : 0;
    }
  }
  
  delete[] f_list;
  return result;
}

// compute the delta value for f in each iteration
mat Subgradient::computeDelta(mat f, Hypergraph *hg, int train_size){
  int hrow = hg -> hMat.n_rows; // number of edges 117
  int hcol = hg -> hMat.n_cols; // dataset size 8124 
  int lrow = hg -> lMat.n_rows; // number of classes 2
  
  mat f_out = zeros<mat>(hg->lMat.n_rows, hg->lMat.n_cols);
  //printf("%d %d %d \n", hrow, hcol, lrow);
  
  for (int i=0; i<hrow; i++){
    uvec allTailId = find(hg->tail.row(i) == 1);
    uvec allHeadId = find(hg->head.row(i) == 1);
    for (int j=0; j<lrow; j++){
       rowvec rj = f.row(j);
       rowvec rt = rj(allTailId); // fvalue for all tails on this edge
       rowvec rh = rj(allHeadId); // fvalue for all heads on this edgd
       double u = rt.max();
       double v = rh.min();
       double d = hg->weight(i) * (u - v);
       
       if (u - v > 0){
         uvec maxlist = find(hg->tail.row(i) == u);
         uword id = maxlist[0];
         f_out(j, id) += d;
       }
    }
  }
  for (int i=0; i<train_size; i++){
    for (int j=0; j<lrow; j++){
      f_out(j, i) = hg->lMat(j, i); 
    }
  }
  return f_out;
}

// the subgradient method core function
mat Subgradient::sgm(mat f, Hypergraph *hg, int train_size){
  for (int i=0; i<1000; i++){
    mat gn = computeDelta(f, hg, train_size);
    f = f - (0.9/norm(gn)) * gn;
  }
  return f;
}
