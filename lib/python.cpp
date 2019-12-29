#include "knn.h"
#include <string>
#include <iostream>

extern "C" {
  knn* init_knn(int c) {
    return new knn(c);
  }

  mat* init_mat(int m, int n, double **values) {
    vec *matrix = new vec[m];
    for (int i = 0; i < m; i++)
      matrix[i] = new vec(n, values[i]);

    return new mat(m, matrix);
  }

  void fit_knn(knn *clf, mat *points, int max_iter) {
    clf->fit(points, max_iter);
  }

  int* predict_knn(knn *clf, mat *points, int k) {
    return clf->predict_knn(points, k)->labels;
  }

  int* predict_cent(knn *clf, mat *points) {
    return clf->predict_cent(points)->labels;
  }
}
