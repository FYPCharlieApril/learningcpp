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
    //printf("%d: %f ", i, norm(f_list[i]));
    mat temp = sgm(f_list[i], hg, train_size);
    //printf("%f\n", norm(temp));
    //data::Save("log/res_"+to_string(i)+".txt", temp);
    f_a = f_a + temp;
  }
   
  f_a = f_a / lRow; //take the average of the prediction matrices
  recoverF(hg, f_a, train_size); 
  data::Save("log/f_a.txt", f_a);
  //decide the classification for the final result, take the class with largest prediction value
  for (int i=0; i<lCol; i++){
    int res_c = -1;
    double pre_val = -1;
    for (int j=0; j<lRow; j++){
      if (f_a(j, i) > pre_val){
        res_c = j;
        pre_val = f_a(j, i);
      }
    }
    result(res_c, i) = 1;
  }
  evalAcc(hg->lMat, result);
  delete[] f_list;
  return result;
}

// compute the delta value for f in each iteration
mat Subgradient::computeDelta(mat f, Hypergraph *hg, int train_size){
  int hrow = hg -> hMat.n_rows; // number of edges 117
  int hcol = hg -> hMat.n_cols; // dataset size 8124 
  int lrow = hg -> lMat.n_rows; // number of classes 2
  
  mat f_out = zeros<mat>(hg->lMat.n_rows, hg->lMat.n_cols);
  //f_out = recoverF(hg, f_out, train_size);
  
  for (int i=0; i<hrow; i++){ // for each edge
    Row<unsigned int> trow = hg->tail.row(i);
    Row<unsigned int> hrow = hg->head.row(i);
    uvec allTailId = find(trow == 1); // indices of all the tails of this edge
    uvec allHeadId = find(hrow == 1); // indices of all the heads of this edge

    for (int j=0; j<lrow; j++){ // for each class
       rowvec rj = f.row(j); 
       rowvec rt = zeros<rowvec>(allTailId.n_rows);  
       rowvec rh = zeros<rowvec>(allHeadId.n_rows);
       
       for (int k=0; k<allTailId.n_rows; k++){
         rt(k) = rj(allTailId(k));
       } 
       
       for (int k=0; k<allHeadId.n_rows; k++){
         rh(k) = rj(allHeadId(k));
       } 
       
       double u = rt.max();
       double v = rh.min();
       double d = hg->weight(i) * (u - v); // currently derivative for (u - v) ^ 2, might be changed 
       //printf("u: %.2f  v: %.2f\n", u, v);
       //printf("d: %.2f\n", d);
       //need to be changed 
       if (u - v > 0){
         uword id;
	 for (int k=0; k<allTailId.n_rows; k++){
           if (u == rj(allTailId(k))){ 
             id = allTailId(k);
           }
         }
         f_out(j, id) += d;
       }
    }
  }

  //f_out = recoverF(hg, f_out, train_size);
  return f_out;
}

// the subgradient method core function
mat Subgradient::sgm(mat f, Hypergraph *hg, int train_size){
  for (int i=0; i<10000; i++){
    //printf("Current step: %d\n", i);
    mat gn = computeDelta(f, hg, train_size);
    //printf("fnorm: %f ", norm(f));
    f = f - (0.9/norm(gn)) * gn;
    //printf("%f ", norm(f));
    recoverF(hg, f, train_size);
    //printf("%f \n", norm(f));
  }
  return f;
}

// recover the prediction value to the given label for the labeled data
mat Subgradient::recoverF(Hypergraph* hg, mat &f, int train_size){
  int lrow = hg -> lMat.n_rows; // number of classes 2
  //printf("%d %d\n",lrow, train_size);
  for (int i=0; i<lrow; i++){ 
    for (int j=0; j<train_size; j++){
      f(i, j) = hg->lMat(i, j); 
    }
  }
  return f;
}

double Subgradient::evalAcc(Mat<unsigned int> target, Mat<unsigned int> prediction){
  int r = target.n_rows; // 2
  int c = target.n_cols; // 8124
  int coun = 0;
  for (int i=0; i<c; i++){
    bool flag = true;
    for (int j=0; j<r; j++){
      if (target(j, i) != prediction(j, i)) 
        flag = false;
    }
    if (flag) coun ++;
  }
  accuracy = double(coun)/double(c);
  printf("the accuracy is: %f\n", accuracy);
  return 0;

}
