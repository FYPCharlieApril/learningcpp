#include "subgradient.h"
#define algcon 0.9
#include <math.h>
#define alpha 1.0
#include <unistd.h>
//mutex lock for parallel computing
mutex mylock;

//activation function to activate the raw prediction result
double actFunc(double x, char method){
  x = 2 * x - 1;
  switch (method){
    case 's':  //sigmoid function
      return 1.0 / (1.0 + exp(-4 * x));
    case 'p':  //soft plus function
      return 1.75 * log(1+exp(2 * x));
    case 'n':  //soft sign function
      return 1.5 * x / (1 + abs(2 * x)) + 0.5;
    case 'e':  //exponential linear unit (ELU) function
      if (x < 0)
        return alpha*(exp(x)-1);
      else
        return x;
    default:
      return (x + 1) / 2;
  }
}

//alternatives for the loss function
double lossGradient(double u, double v, char method){
  double diff = u-v;
  switch (method){
    case 'g': // gradient for gaussian function
      return 1.5*diff*exp(-3*diff*diff);
    default: // default function: quadratic
      return diff;
  }
}

Mat<unsigned int> Subgradient::fitPredict(Hypergraph *hg, int train_size, double precision, char actfunc, char lossfunc ){
  Mat<unsigned int> result = zeros<Mat<unsigned int>>(hg->lMat.n_rows, hg->lMat.n_cols);
  
  int lRow = hg->lMat.n_rows; int lCol = hg->lMat.n_cols;
  
  //start with original cases with all data point belonging to absolutely one class
  thread sgmWorker[lRow];
  mat f_a = zeros<mat>(hg->lMat.n_rows, hg->lMat.n_cols);

  for (int i=0; i<lRow; i++){
    sgmWorker[i] = thread(&Subgradient::sgm, this, hg, train_size, precision, actfunc, lossfunc, i, ref(f_a));
  }

  for (int i=0; i<lRow; i++){
    sgmWorker[i].join();
  }  

  f_a = f_a / lRow; //take the average of the prediction matrices

  recoverF(hg, f_a, train_size); 
  data::Save("log/f_a.txt", f_a);
  //decide the classification for the final result, take the class with largest prediction value
  for (int i=0; i<lCol; i++){
    int res_c = 0;
    double pre_val = -std::numeric_limits<double>::infinity();
    for (int j=0; j<lRow; j++){
      if (f_a(j, i) > pre_val){
        res_c = j;
        pre_val = f_a(j, i);
      }
    }
    result(res_c, i) = 1;
  }
  evalAcc(hg->lMat, result);
  return result;
}

// compute the delta value for f in each iteration
mat Subgradient::computeDelta(mat f, Hypergraph *hg, int train_size, char actfunc, char lossfunc){
  int hrow = hg -> hMat.n_rows; // number of edges 
  int hcol = hg -> hMat.n_cols; // dataset size 
  int lrow = hg -> lMat.n_rows; // number of classes 
  
  mat f_out = zeros<mat>(hg->lMat.n_rows, hg->lMat.n_cols);

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
       double d = hg->weight(i) * lossGradient(u, v, lossfunc); 
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
  recoverF(hg, f_out, train_size);
  return f_out;
}

// the subgradient method core function
mat Subgradient::sgm(Hypergraph *hg, int train_size, double precision, char actfunc, char lossfunc, int ind, mat& f_a){
  int lRow = hg->lMat.n_rows; 
  int lCol = hg->lMat.n_cols;
  mat f = zeros<mat>(hg->lMat.n_rows, hg->lMat.n_cols);
  f.row(ind) = ones<rowvec>(lCol);
  f = recoverF(hg, f, train_size);
  for (int i=0; i<1000; i++){
    mat gn = computeDelta(f, hg, train_size, actfunc, lossfunc);
    f = f - (0.9/norm(gn)) * gn;
    recoverF(hg, f, train_size);
  }
  
  for (int i=0; i<f.n_rows; i++)
    for (int j=0; j<f.n_cols; j++)
      f(i, j) = actFunc(f(i, j), actfunc);
  
  mylock.lock();
  f_a += f;
  mylock.unlock();
  return f;
}

// recover the prediction value to the given label for the labeled data
mat Subgradient::recoverF(Hypergraph* hg, mat &f, int train_size){
  int lrow = hg -> lMat.n_rows; 
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
