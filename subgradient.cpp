#include "subgradient.h"
#define algcon 0.9
double normc = 0.0;
mat f_a;
mutex mylock;

Mat<unsigned int> Subgradient::fitPredict(Hypergraph *hg, int train_size, double precision){
  Mat<unsigned int> result = zeros<Mat<unsigned int>>(hg->lMat.n_rows, hg->lMat.n_cols);

  int lRow = hg->lMat.n_rows; int lCol = hg->lMat.n_cols;
  mat* f_list = new mat[lRow];
  for (int i=0; i<lRow; i++){
    f_list[i] = zeros<mat>(hg->lMat.n_rows, hg->lMat.n_cols);
    f_list[i].row(i) = ones<rowvec>(lCol);
    recoverF(hg, f_list[i], train_size);
  }
  
  f_a = zeros<mat>(hg->lMat.n_rows, hg->lMat.n_cols);
  //start with original cases with all data point belonging to absolutely one class
  thread sgmWorker[lRow];
  for (int i=0; i<lRow; i++){
    sgmWorker[i] = thread([=] {sgm(f_list[i], hg, train_size, precision);});
    //sgm(f_list[i], hg, train_size, precision);
  }

  for (int i=0; i<lRow; i++){
    sgmWorker[i].join();
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
mat Subgradient::computeDelta(mat &f, Hypergraph *hg, int train_size){
  int hrow = hg -> hMat.n_rows; 
  int hcol = hg -> hMat.n_cols;  
  int lrow = hg -> lMat.n_rows; 
  int aSize = hrow*lrow;
  int ilist[aSize];
  int jlist[aSize];
  double dlist[aSize];
  //mat f_out = zeros<mat>(hg->lMat.n_rows, hg->lMat.n_cols);
  //f_out = recoverF(hg, f_out, train_size);
  normc = 0.0;
  for (int i=0; i<hrow; i++){ // for each edge
    Row<unsigned int> tailrow = hg->tail.row(i);
    Row<unsigned int> headrow = hg->head.row(i);
    uvec allTailId = find(tailrow == 1); // indices of all the tails of this edge
    uvec allHeadId = find(headrow == 1); // indices of all the heads of this edge
    for (int j=0; j<lrow; j++){ // for each class
       rowvec rj = f.row(j); 
       rowvec rt = zeros<rowvec>(allTailId.n_rows);  
       rowvec rh = zeros<rowvec>(allHeadId.n_rows);
       
       //take all tails on this edge
       for (int k=0; k<allTailId.n_rows; k++){
         rt(k) = rj(allTailId(k));
       } 
       
       //take all heads on this edge
       for (int k=0; k<allHeadId.n_rows; k++){
         rh(k) = rj(allHeadId(k));
       } 
       
       double u = max(rt); //may be replaced by heap
       double v = min(rh); //may be replaced by heap
       double d = hg->weight(i) * (u - v); // currently derivative for (u - v) ^ 2, might be changed 
       int ind = j*hrow+i;
       if (u - v > 0){
         uword id;
	 for (int k=0; k<allTailId.n_rows; k++){
           if (u == rj(allTailId(k))){ 
             id = allTailId(k);
           }
         }
         
         ilist[ind] = j;
         jlist[ind] = id;
         dlist[ind] = d;
         //f_out(j, id) += d;
         normc += d*d;
       }else{
         ilist[ind] = -1;
       }
    }
  }
  normc = sqrt(normc);
  for (int i=0; i<aSize; i++){
    double d = algcon * dlist[i] / normc;
    int m = ilist[i];
    int n = jlist[i];
    if (m>-1)
      f(m, n) -= d;
  }
  //return f_out;
  return f;
}

// the subgradient method core function
void Subgradient::sgm(mat f, Hypergraph *hg, int train_size, double precision){
  //double diff = 1000.0;
  //while (diff > precision){
  for (int i=0; i<1/precision; i++){
    //printf("Current step: %d\n", i);
    //mat f_old = f;
    computeDelta(f, hg, train_size);
    recoverF(hg, f, train_size);
    //diff = abs(norm(f-f_old));
  }
  mylock.try_lock();
  f_a += f;
  mylock.unlock();
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
  int r = target.n_rows; 
  int c = target.n_cols; 
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
