#include "kmn.h"
#include <string>
#include <iostream>

extern "C" {
  kmn* init_kmn(int c) { return new kmn(c); }
  int* kmeans(kmn *clf, mat *points, int max_iter) { return clf->kmeans(points, max_iter)->labels; }
  void kneigh(kmn *clf, mat *points, int *labels) { clf->kneigh(points, labels); }
  int* predict_knn(kmn *clf, mat *points, int k) { return clf->predict_knn(points, k)->labels; }
  int* predict_cent(kmn *clf, mat *points) { return clf->predict_cent(points)->labels; }

  mat* init_mat(int m, int n, double **values) {
    vec *matrix = new vec[m];
    for (int i = 0; i < m; i++)
      matrix[i] = new vec(n, values[i]);

    return new mat(m, matrix);
  }

}
