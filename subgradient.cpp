#include "subgradient.h"

Mat<unsigned int> Subgradient::fitPredict(Hypergraph *hg, int train_size){
  Mat<unsigned int> result = zeros<Mat<unsigned int>>(hg->lMat.n_rows, hg->lMat.n_cols);
  
  int lRow = hg->lMat.n_rows; int lCol = hg->lMat.n_cols;
  mat* f_list = new mat[lRow];
  for (int i=0; i<lRow; i++){
    f_list[i] = zeros<mat>(hg->lMat.n_rows, hg->lMat.n_cols);
    f_list[i].row(i) = ones<rowvec>(lCol);
    recoverF(hg, f_list[i], train_size);
  }
  
  mat f_a = zeros<mat>(hg->lMat.n_rows, hg->lMat.n_cols);
  for (int i=0; i<lRow; i++){
    f_a = f_a + sgm(f_list[i], hg, train_size);
  }
   
  f_a = f_a / lRow;
  
  for (int i=0; i<lRow; i++){
    double t = mean(f_a.row(i));
    printf("class: %d  mean: %3f", i, t);
    for (int j=0; j<lCol; j++){
      if (f_a(i, j) > t) result(i, j) = 1;
      else result(i, j) = 0;
    }
  }
  
  delete[] f_list;
  return result;
}

// compute the delta value for f in each iteration
mat Subgradient::computeDelta(mat &f, Hypergraph *hg, int train_size){
  int hrow = hg -> hMat.n_rows; // number of edges 117
  int hcol = hg -> hMat.n_cols; // dataset size 8124 
  int lrow = hg -> lMat.n_rows; // number of classes 2
  
  mat f_out = zeros<mat>(hg->lMat.n_rows, hg->lMat.n_cols);
  f_out = recoverF(hg, f_out, train_size);
  
  for (int i=0; i<hrow; i++){ // for each edge
    Row<unsigned int> trow = hg->tail.row(i);
    Row<unsigned int> hrow = hg->head.row(i);
    uvec allTailId = find(trow == 1); // indices of all the tails of this edge
    uvec allHeadId = find(hrow == 1); // indices of all the heads of this edge

    for (int j=0; j<lrow; j++){ // for each class
       rowvec rj = f.row(j); 
       
       rowvec rt = rowvec(allTailId.n_cols);  
       rowvec rh = rowvec(allHeadId.n_cols);
       
       for (int k=0; k<allTailId.n_cols; k++){
         rt(k) = rj(allTailId(k));
       } 
       
       for (int k=0; k<allHeadId.n_cols; k++){
         rh(k) = rj(allHeadId(k));
       } 
       
       double u = rt.max();
       double v = rh.min();
       double d = hg->weight(i) * (u - v);
       //printf("rtmax: %2f  rtmin: %2f  rhmax: %2f  rhmin: %2f\n", rt.max(), rt.min(), rh.max(), rh.min());
       if (u - v > 0){
	 uvec maxList = find(trow == u);
         uword id = maxList[0];
         f_out(j, id) += d;
       }
    }
  }

  f_out = recoverF(hg, f_out, train_size);
  return f_out;
}

// the subgradient method core function
mat Subgradient::sgm(mat f, Hypergraph *hg, int train_size){
  for (int i=0; i<200; i++){
    //printf("Current step: %d\n", i);
    mat gn = computeDelta(f, hg, train_size);
    f = f - (0.9/norm(gn)) * gn;
    f = recoverF(hg, f, train_size);
  }
  return f;
}

// recover the prediction value to the given label for the labeled data
mat Subgradient::recoverF(Hypergraph* hg, mat &f, int train_size){
  int lrow = hg -> lMat.n_rows; // number of classes 2
  for (int i=0; i<lrow; i++){ 
    for (int j=0; j<train_size; j++){
      f(i, j) = hg->lMat(i, j); 
    }
  }
  return f;
}
