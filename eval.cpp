#include <mlpack/core.hpp>
#include <iostream>
#include <armadillo>
#include "hypergraph.h"
#include "subgradient.h"

int main()
{ 
  Mat<int> target;
  Mat<int> prediction; 
  data::Load("log/oh_la.txt", target, true);
  data::Load("log/result.txt", prediction, true);

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
  double acc = double(coun)/double(c);
  printf("accuracy: %f", acc);
  return 0;
}
